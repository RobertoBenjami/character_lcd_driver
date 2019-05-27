//******************************************************************************
// 8 bites 18Fxxx PIC óra demo az charlcd teszteléséhez, htc18, xc8 forditóhoz
// készítõ: Roberto Benjami (robertodebenjami(at)gmail(dot)com)
//
// charlcd.h szükséges beállításai:
// - a beállítás bármi lehet, csak a felhasznált mikrovezérlõ is támogassa,
//   (pl. a beállított I/O láb és a TIMER létezzen)
//   és az egymástól függõ beállításoknál ne legyen összeférhetetlénség
//   (pl. kurzor csak egyszeri frissítési módban legyen),
// - adott esetben a felhasznált I/O lábak analógról -> digitálisra történõ átállításáról gondoskodjunk!
//   (ezt vagy a hardvareprofile.h InitAD() definíciójába, vagy akár ide a main után is beírhatjuk)
// - legalább 32 karakter legyen a kijelzõ(kö)n, (legalább 2x16, LCDSTEREO esetén 1x16)
//******************************************************************************

#if defined(__XC__)
#include <xc.h>
#else
#include <htc.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <timers.h>
#include "charlcd.h"

// órajel forrás frekvencia
#define  TimerSrcClock (1UL*SystemClock/4)

// 1000 megszakítás/sec
#define  TIMECLK 1000

// led lábak definiálása (ha nincs, kommenteljük ki)
#define  LED1       B, 0
#define  LED2       B, 1

// melyik idõzítõn járjon az óra ? (hogy ne ütközzön az lcdchar által felhasználtal)
#if (LCDTIMERNUM == 0)
#define  CLOCKTIMERNUM   1
#else
#define  CLOCKTIMERNUM   0
#endif

unsigned short ev = 2013;
unsigned char honap = 12;
unsigned char nap = 24;
unsigned char ora = 23;
unsigned char perc = 58;
unsigned char masodperc = 0;
const unsigned char honaphosszak[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

unsigned char temp;                     // timer komparátor segédváltozója

#ifdef USERCHARSETCHANGE
USERCHARSETARRAY userchars;
#endif

#define TMCOMP ((TimerSrcClock+TIMECLK/2)/TIMECLK)

// komparátor korrekciós érték
#define  TMCOMPCOR0     7 
#define  TMCOMPCOR1     5 

// timer0 komparátor hiányának pótlása
#define TIMER0COMPLESS { \
  temp = (65535+TMCOMPCOR0-TMCOMP) + TMR0L; \
  WREG = ((65535+TMCOMPCOR0-TMCOMP) >> 8); \
  asm("addwfc (TMR0H), 1, 0"); \
  TMR0L = temp; \
}

// timer1 komparátor hiányának pótlása
#define TIMER1COMPLESS { \
  temp = (65535+TMCOMPCOR1-TMCOMP) + TMR1L; \
  WREG = ((65535+TMCOMPCOR1-TMCOMP) >> 8); \
  asm("addwfc (TMR1H), 1, 0"); \
  TMR1L = temp; \
}

#define GLUE(a, b)   a##b
#define SET_(what, p, m)       GLUE(what, p) |= (1 << (m))
#define CLR_(what, p, m)       GLUE(what, p) &= ~(1 << (m))
#define GET_(/* PORT, */ p, m) GLUE(PORT, p) & (1 << (m))
#define IOIN(x)                SET_(TRIS, x)
#define IOOUT(x)               CLR_(TRIS, x)
#define SET(x)                 SET_(LAT, x)
#define CLR(x)                 CLR_(LAT, x)
#define GET(x)                 GET_(x)

//******************************************************************************
void init(void)
{
  // idõzítõ inicializálása
  #if CLOCKTIMERNUM == 0
  T0CONbits.T08BIT = 0;                 // TMR0 16bites
  T0CONbits.T0CS = 0;                   // órajel forrás
  T0CONbits.PSA = 1;                    // elõosztó tiltás
  INTCON2bits.TMR0IP = 1;               // magas prioritás
  INTCONbits.TMR0IE = 1;                // TMR0 megszakítás eng
  T0CONbits.TMR0ON = 1;                 // bekapcs
  TMR0L = 65536+TMCOMPCOR0-TMCOMP;      // timerL kezdõérték feltöltése bitidõre
  TMR0H = (65536+TMCOMPCOR0-TMCOMP) >> 8;// timerH kezdõérték feltöltése bitidõre
  #else
  T1CONbits.RD16 = 1;                   // 16bites írás/olv
  T1CONbits.TMR1CS = 0;                 // forrás órajel = systemclock
  T1CONbits.T1CKPS1 = 0; T1CONbits.T1CKPS0 = 0; // osztó 1:1
  IPR1bits.TMR1IP = 1;                  // magas prioritás
  PIE1bits.TMR1IE = 1;                  // megszakítás engeélyezés
  T1CONbits.TMR1ON = 1;                 // bekapcs
  TMR1L = 65536+TMCOMPCOR1-TMCOMP;      // timerL kezdõérték feltöltése bitidõre
  TMR1H = (65536+TMCOMPCOR1-TMCOMP) >> 8;// timerH kezdõérték feltöltése bitidõre
  #endif

  // megszakítás beállítások
  #if LCDTIMERPR18 == -1
  RCONbits.IPEN = 0;                    // kétszintû IRQ tiltás
  #else
  RCONbits.IPEN = 1;                    // kétszintû IRQ eng
  #endif
  INTCONbits.GIEL = 1;                  // globális LO IRQ eng
  INTCONbits.GIEH = 1;                  // globális IRQ eng

  // ledek beállítása
  #ifdef  LED1
  CLR(LED1); IOOUT(LED1);
  #endif
  #ifdef  LED2
  CLR(LED2); IOOUT(LED2);
  #endif
}
//******************************************************************************
void main(void)
{
  unsigned char lastsec = 255;

  #ifdef USERCHARSETCHANGE
  unsigned char i, j;
  #endif

  // ha szükség van AD láb digitális I/O lábra történõ átkapcsolásra
  #ifdef InitAD
  InitAD();
  #endif

  // felhasználói karakterkészlet másolása RAM-ba (csak ha engedélyezve van)
  #ifdef USERCHARSETCHANGE
  memcpy((char *)userchars, userromcharset, sizeof(USERCHARSETARRAY)); 
  #endif
  LcdInit();                            // müködésre bírjuk az LCD-t
  //                      "01020304050607080910111213141516"
  memcpy((char *)LcdText, "   eeee.hh.nn       oo:pp:mm    ", 32); // kezdõképernyõ feltöltése

  init();
  while(1)
  {
    // folyamatos frissítési üzemmód megszakítás nélkül
    #ifdef LCDMODECONT
    LcdProcess();
    #endif

    if(lastsec != masodperc)
    {
      // 2 led felváltva villogjon
      #ifdef  LED1
      if(GET(LED1)) 
      {
        CLR(LED1);
        #ifdef  LED1
        SET(LED2);
        #endif
      }
      else 
      {
        SET(LED1);
        #ifdef  LED1
        CLR(LED2);
        #endif
      }
      #endif

      // Ha nincs automata villogtatás 1sec a villogási idõzítése
      #if (defined LCDBLINKCHAR) && ((!defined LCDMODECONTIRQ) || (LCDBLINKSPEED == 0))
      BlinkPhase = !BlinkPhase;
      #endif

      // jár az óra jár
      if(masodperc == 60)
      {
        masodperc = 0;
        if(++perc == 60)
        {
          perc = 0;
          if(++ora == 24)
          {
            ora = 0;
            if(++nap > honaphosszak[honap - 1])
            {
              nap = 1;
              if(++honap == 13)
              {
                honap = 1;
                ev++;
      } } } } }
      lastsec = masodperc;

      // számjegyek kiírása
      utoa((char *)LcdText + 3, ev, 10);
      utoa((char *)LcdText + 8, honap, 10);
      utoa((char *)LcdText + 11, nap, 10);
      utoa((char *)LcdText + 20, ora, 10);
      utoa((char *)LcdText + 23, perc, 10);
      utoa((char *)LcdText + 26, masodperc, 10);

      // egyszámjegyû számok kétszámjegyüvé alakítása
      if(!LcdText[9]) {LcdText[9]  = LcdText[8];  LcdText[8]  = '0';}
      if(!LcdText[12]){LcdText[12] = LcdText[11]; LcdText[11] = '0';}
      if(!LcdText[21]){LcdText[21] = LcdText[20]; LcdText[20] = '0';}
      if(!LcdText[24]){LcdText[24] = LcdText[23]; LcdText[23] = '0';}
      if(!LcdText[27]){LcdText[27] = LcdText[26]; LcdText[26] = '0';}

      // *toa által felülírt írásjelek pótlása
      LcdText[7] = '.';
      LcdText[10] = '.';
      LcdText[22] = ':';
      LcdText[25] = ':';

      // számok közötti kettõspont villogtatás
      #ifdef LCDBLINKCHAR
      LcdBlinkChar(22);
      LcdBlinkChar(25);
      #endif

      // felhasználó által definiált karakterek a sarkokban
      #ifdef USERCHARSET
      LcdText[0] = 8;
      LcdText[1] = 10;
      LcdText[16] = 9;
      LcdText[17] = 11;
      LcdText[14] = 12;
      LcdText[15] = 14;
      LcdText[30] = 13;
      LcdText[31] = 15;
      #endif

      // kurzor léptetése és kurzortípus váltogatása
      #ifdef LCDCURSOR
      LcdCursorPos++;
      if(LcdCursorPos >= (1UL * LCDWIDTH * LCDLINES))
        LcdCursorPos = 0;
      if((LcdCursorPos & 0b11) == 0b00) LcdCursorOff();
      else if((LcdCursorPos & 0b11) == 0b01) {LcdCursorUnBlink(); LcdCursorOn();}
      else if((LcdCursorPos & 0b11) == 0b10) {LcdCursorBlink(); LcdCursorOn();}
      else if((LcdCursorPos & 0b11) == 0b11) {LcdCursorUnBlink(); LcdCursorOn();}
      #endif

      // karakterkészlet shiftelése és feltöltése (csak ha karakterkészlet változtatásának lehetõsége engedve van)
      #ifdef USERCHARSETCHANGE
      j = userchars[63];
      i = 63;
      while(i-- > 0)
        userchars[i + 1] = userchars[i];
      userchars[0] = j;
      LcdChangeCharset(userchars);
      #endif // USERCHARSETCHANGE

      // egyszeri frissítési üzemmódban megjelenítés
      #if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEWAIT) || (defined LCDMODEONCEIRQ)) 
      LcdRefreshAll();
      #endif
    }
  }
}

void interrupt low_priority irql(void)
{
  #if (!defined LCDUSERTIMER) && (LCDTIMERPR18 == 0) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif
}

void interrupt irqh(void)
{
  static unsigned int msec = 0;

  #if (!defined LCDUSERTIMER) && ((LCDTIMERPR18 == -1) || (LCDTIMERPR18 == 1)) && (defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ)
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif

  #if CLOCKTIMERNUM == 0
  if(INTCONbits.TMR0IF)                 // timer0 interrupt ?
  {
    TIMER0COMPLESS;                     // timer0 komparátor hiányának pótlása
    INTCONbits.TMR0IF = 0;              // nyugtázás
    msec++;
    if(msec >= 1000)
    {
      masodperc++;
      msec = 0; 
    }
    #if (defined LCDUSERTIMER)
    LcdIntProcess();                    // saját megszakításkezelõvel és idõzítéskezelõvel használva
    #endif
  }
  #else
  if(PIR1bits.TMR1IF)                   // timer1 interrupt ?
  {
    TIMER1COMPLESS;                     // timer1 komparátor hiányának pótlása
    PIR1bits.TMR1IF = 0;                // nyugtázás
    msec++;
    if(msec >= 1000)
    {
      masodperc++;
      msec = 0; 
    }
    #if (defined LCDUSERTIMER)
    LcdIntProcess();                    // saját megszakításkezelõvel és idõzítéskezelõvel használva
    #endif
  }
  #endif
}
