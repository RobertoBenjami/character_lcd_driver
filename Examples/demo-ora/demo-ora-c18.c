//******************************************************************************
// 8 bites 18Fxxx PIC óra demo az charlcd teszteléséhez, c18 forditóhoz
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

#include <p18cxxx.h>
#include <string.h>
#include <stdlib.h>
#include <timers.h>
#include "charlcd.h"

// bootloader nélkül: 0x0000, MCHPUSB bootloader: 0x0800, HID bootloader: 0x1000
#define RESET_VECTOR_ADDRESS             0x0

// órajel forrás frekvencia
#define  TimerSrcClock (1UL*SystemClock/4)

// 1000 megszakítás/sec
#define  TIMECLK    1000

// led lábak definiálása (ha nincs, töröljük a LEDS definíciót)
#define  LEDS
#define  LED1       LATBbits.LATB0
#define  LED2       LATBbits.LATB1
#define  LEDIO1     TRISBbits.TRISB0
#define  LEDIO2     TRISBbits.TRISB1

// melyik idõzítõn járjon az óra ? (hogy ne ütközzön az lcdchar által felhasználtal)
#if (LCDTIMERNUM == 0)
#define  CLOCKTIMERNUM   1
#else
#define  CLOCKTIMERNUM   0
#endif

unsigned int ev = 2013;
unsigned char honap = 12;
unsigned char nap = 24;
unsigned char ora = 23;
unsigned char perc = 58;
unsigned char masodperc = 0;
rom unsigned char honaphosszak[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

unsigned char temp;                     // timer komparátor segédváltozója

#ifdef USERCHARSETCHANGE
USERCHARSETARRAY userchars;
#endif

#define TMCOMP ((TimerSrcClock+TIMECLK/2)/TIMECLK)

// komparátor korrekciós érték
#define  TMCOMPCOR0     7 
#define  TMCOMPCOR1     5 

// timer0 komparátor hiányának pótlása
#define TIMER0COMPLESS  _asm \
  movlb  temp \
  movlw  65536+TMCOMPCOR0-TMCOMP \
  addwf  TMR0L, 0, 0 \
  movwf  temp, 1 \
  movlw  (65536+TMCOMPCOR0-TMCOMP) >> 8 \
  addwfc TMR0H, 1, 0 \
  movff  temp, TMR0L \
_endasm

// timer1 komparátor hiányának pótlása
#define TIMER1COMPLESS  _asm \
  movlb  temp \
  movlw  65536+TMCOMPCOR1-TMCOMP \
  addwf  TMR1L, 0, 0 \
  movwf  temp, 1 \
  movlw  (65536+TMCOMPCOR1-TMCOMP) >> 8 \
  addwfc TMR1H, 1, 0 \
  movff  temp, TMR1L \
_endasm

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
  #ifdef  LEDS
  LED1 = 0; LED2 = 0; LEDIO1 = 0; LEDIO2 = 0;
  #endif
}
//******************************************************************************
void main(void)
{
  unsigned char lastsec = 255;
  #ifdef USERCHARSETCHANGE
  unsigned char i, j;
  // felhasználói karakterkészlet másolása RAM-ba
  memcpypgm2ram((char *)userchars, userromcharset, sizeof(USERCHARSETARRAY)); 
  #endif

  // ha szükség van AD láb digitális I/O lábra történõ átkapcsolásra
  #ifdef InitAD
  InitAD();
  #endif

  LcdInit();                            // müködésre bírjuk az LCD-t
  //                             "01020304050607080910111213141516"
  memcpypgm2ram((char *)LcdText, "   eeee.hh.nn       oo:pp:mm    ", 32); // kezdõképernyõ feltöltése

  init();
  while(1)
  {
    // folyamatos frissítési üzemmód megszakítás nélkül
    #ifdef LCDMODECONTBUSY
    LcdProcess();
    #endif

    if(lastsec != masodperc)
    {
      // 2 led felváltva villogjon
      #ifdef  LEDS
      if(LED1)
        {LED1 = 0; LED2 = 1;}
      else
        {LED1 = 1; LED2 = 0;}
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
      itoa(ev, (char *)LcdText + 3);
      btoa(honap, (char *)LcdText + 8);
      btoa(nap, (char *)LcdText + 11);
      btoa(ora, (char *)LcdText + 20);
      btoa(perc, (char *)LcdText + 23);
      btoa(masodperc, (char *)LcdText + 26);

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

#pragma interruptlow YourLowPriorityISRCode
void YourLowPriorityISRCode()
{
  #if ((!defined LCDUSERTIMER) && (LCDTIMERPR18 == 0) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ)))
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif
}

#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode()
{
  static unsigned int msec = 0;

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

  #if ((LCDTIMERPR18 == -1) || (LCDTIMERPR18 == 1)) && (!defined LCDUSERTIMER)  && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif
}

#if RESET_VECTOR_ADDRESS == 0x1000
  #define HIGH_INTERRUPT_VECTOR_ADDRESS  0x1008
  #define LOW_INTERRUPT_VECTOR_ADDRESS   0x1018
#elif RESET_VECTOR_ADDRESS == 0x800
  #define HIGH_INTERRUPT_VECTOR_ADDRESS   0x808
  #define LOW_INTERRUPT_VECTOR_ADDRESS    0x818
#else
  #define HIGH_INTERRUPT_VECTOR_ADDRESS    0x08
  #define LOW_INTERRUPT_VECTOR_ADDRESS     0x18
#endif

#if RESET_VECTOR_ADDRESS != 0x0
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code RESET_VECTOR = RESET_VECTOR_ADDRESS
void _reset (void)
{
  _asm goto _startup _endasm
}
#endif

#pragma code HIGH_INTERRUPT_VECTOR = HIGH_INTERRUPT_VECTOR_ADDRESS
void High_ISR (void)
{
  _asm goto YourHighPriorityISRCode _endasm
}

#pragma code LOW_INTERRUPT_VECTOR = LOW_INTERRUPT_VECTOR_ADDRESS
void Low_ISR (void)
{
  _asm goto YourLowPriorityISRCode _endasm
}
