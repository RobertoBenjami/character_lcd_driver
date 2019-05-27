//******************************************************************************
// 8 bites 16Fxxx PIC óra demo a charlcd teszteléséhez, htc, xc8 forditókhoz
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
#include "charlcd.h"

// órajel forrás frekvencia
#define TimerSrcClock (1UL*SystemClock/4)

// 100 megszakítás/sec
#define  TIMECLK  100

// led lábak definiálása (ha nincs, kommenteljük ki)
#define  LED1       B, 0
#define  LED2       B, 1

// melyik idõzítõn járjon az óra ? (hogy ne ütközzön az lcdchar által felhasználtal)
#if (LCDTIMERNUM == 1)
#define  CLOCKTIMERNUM   2
#else
#define  CLOCKTIMERNUM   1
#endif

unsigned int ev = 2013;
unsigned char honap = 12;
unsigned char nap = 24;
unsigned char ora = 23;
unsigned char perc = 58;
volatile unsigned char masodperc = 0;
unsigned char honaphosszak[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#ifdef USERCHARSETCHANGE
USERCHARSETARRAY userchars;
#endif

// osztásarány
#define  TIMEDIV ((TimerSrcClock+TIMECLK/2)/TIMECLK)

#define GLUE(a, b)             a ## b
#define SET_(what, p, m)       GLUE(what, p) |= (1 << (m))
#define CLR_(what, p, m)       GLUE(what, p) &= ~(1 << (m))
#define GET_(p, m)             GLUE(PORT, p) & (1 << (m))
#define IOIN(x)                SET_(TRIS, x)
#define IOOUT(x)               CLR_(TRIS, x)
#ifdef _PIC14E
#define SET(x)                 SET_(LAT, x)
#define CLR(x)                 CLR_(LAT, x)
#else
#define SET(x)                 SET_(PORT, x)
#define CLR(x)                 CLR_(PORT, x)
#endif
#define GET(x)                 GET_(x)

//******************************************************************************
void init(void)
{
  // idõzítõ inicializálása
  #if CLOCKTIMERNUM == 1
  #if TIMEDIV < 65536
  #define TMCOMP (((TimerSrcClock/1)+TIMECLK/2)/TIMECLK)
  #define TMCOMPCOR     65540
  T1CONbits.T1CKPS = 0;
  #elif TIMEDIV < (65536*8)
  #define TMCOMP (((TimerSrcClock/8)+TIMECLK/2)/TIMECLK)
  #define TMCOMPCOR     65536
  T1CONbits.T1CKPS = 3;
  #else
  #error "PIC16: ez a frekvencia túl alacsony"
  #endif // TIMEDIV

  T1CONbits.TMR1CS = 0;                 // forrás órajel = systemclock
  PIE1bits.TMR1IE = 1;                  // megszakítás engedélyezés
  TMR1 = TMCOMPCOR-TMCOMP;              // timer kezdõérték feltöltése bitidõre
  T1CONbits.TMR1ON = 1;                 // bekapcs
  #else  // CLOCKTIMERNUM == 1

  #if TIMEDIV < 256
  T2CONbits.T2CKPS = 0; T2CONbits.TOUTPS = 0;
  PR2 = (((TimerSrcClock/1)+TIMECLK/2)/TIMECLK-1);
  #elif TIMEDIV < (256*16)
  T2CONbits.T2CKPS = 2; T2CONbits.TOUTPS = 0;
  PR2 = (((TimerSrcClock/16)+TIMECLK/2)/TIMECLK-1);
  #elif TIMEDIV < (256*16*16)
  T2CONbits.T2CKPS = 2; T2CONbits.TOUTPS = 15;
  PR2 = (((TimerSrcClock/256)+TIMECLK/2)/TIMECLK-1);
  #else
  #error "PIC16: ez a frekvencia túl alacsony"
  #endif
  PIE1bits.TMR2IE = 1;                  /* megszakítás engeélyezés */\
  T2CONbits.TMR2ON = 1;                 // Timer2 ON
  #endif // else CLOCKTIMERNUM == 1

  INTCONbits.PEIE = 1;                  // periféria IRQ eng
  INTCONbits.GIE = 1;                   // globális IRQ eng
  
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
      // 2 led másodpercenkéánt felváltva villogjon
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

       // Ha nincs automata villogtatásn 1sec a villogási idõzítése
      #if (defined LCDBLINKCHAR) && (!defined LCDMODECONTIRQ) || (LCDBLINKSPEED == 0)
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

      // utoa által #0-val felülírt írásjelek pótlása
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


//******************************************************************************
// PIC16 megszakítás kiszolgálás
static void interrupt isr(void)
{
  static unsigned char msec10 = 0;
  
  #if CLOCKTIMERNUM == 1
  if(PIR1bits.TMR1IF)                   // timer1 interrupt ?
  {
    TMR1 += (TMCOMPCOR-TMCOMP);         // timer1 komparátor hiányának pótlása
    PIR1bits.TMR1IF = 0;                // nyugtázás
    msec10++;
    if(msec10 >= 100)
    {
      masodperc++;
      msec10 = 0; 
    }
  }
  #else
  if(PIR1bits.TMR2IF)                   // timer2 interrupt ?
  {
    PIR1bits.TMR2IF = 0;                // nyugtázás
    msec10++;
    if(msec10 >= 100)
    {
      masodperc++;
      msec10 = 0; 
    }

    #if (defined LCDUSERTIMER)
    LcdIntProcess();                    // saját megszakításkezelõvel és idõzítéskezelõvel használva
    #endif
  }
  #endif

  #if (!defined LCDUSERTIMER) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif
}
