//******************************************************************************
// "charlcd" LCD kijelzõ frissítésének processzorhasználat mérõ program
// pic18fxxx vezérlöhöz, c18 forditóhoz
// készítõ: Roberto Benjami (robertodebenjami(at)gmail(dot)com)
//
// Mûködése:
// - két számlálót növel, az egyiket LCD frissítés közben,
//   a másikat az LCD frissítésének kikapcsolása után
//   kiírja a két beszámolt értéket, valamint azt, hogy az elsõ mérés eredménye 
//   hány százalékkal kevesebb a másodiknál.
//
// charlcd.h szükséges beállításai:
// - megszakításos folyamatos frissítési üzemmód (LCDMODECONTIRQ)
//
// letesztelhetjük:
// - LCDFRAMEPERSEC változtatásának a hatását
// - 4/8 bites üzemmód (LCD4BITMODE) hatását
// - villogtatás ki/be hatását (LCDBLINKCHAR)
// - a különbözõ CPU órajel hatását
//******************************************************************************

#include <p18cxxx.h>
#include <string.h>
#include <stdlib.h>
#include <timers.h>
#include "charlcd.h"

// bootloader nélkül: 0x0000, MCHPUSB bootloader: 0x0800, HID bootloader: 0x1000
#define RESET_VECTOR_ADDRESS             0x0

// led lábak definiálása
#define  LED1(f)    f(B, 0)
#define  LED2(f)    f(B, 1)

#if (!defined LCDMODECONTIRQ)
#error "A tesztprogram LCDMODECONTIRQ üzemmódban mûködik!"
#endif

#if LCDTIMERNUM == 1
#error "Az LCD-t ne az 1. timerrel használjuk!"
#endif

#define TRISBIT(p, m)         TRIS ## p ## bits.TRIS ## p ## m
#define LATBIT(p, m)          LAT ## p ## bits.LAT ## p ## m
#define PORTBIT(p, m)         PORT ## p ## bits.R ## p ## m
#define TRISX(p, m)           TRIS ## p
#define LATX(p, m)            LAT ## p
#define PORTX(p, m)           PORT ## p

#define GPIOX_MODEPININ(x)    x(TRISBIT) = 1
#define GPIOX_MODEPINOUT(x)   x(TRISBIT) = 0
#define GPIOX_SETBIT(x)       x(LATBIT) = 1
#define GPIOX_CLRBIT(x)       x(LATBIT) = 0
#define GPIOX_GETBIT(x)       x(PORTBIT)

//******************************************************************************
void init(void)
{
  // 1. idõzítõ inicializálása
  T1CONbits.RD16 = 1;                   // 16bites írás/olv
  T1CONbits.T1OSCEN = 1;                // órakvarc eng
  T1CONbits.TMR1CS = 1;                 // forrás órajel = órakvarc
  T1CONbits.T1CKPS = 0;                 // elõosztó : 1:1
  T1CONbits.TMR1ON = 0;                 // kikapcs

  // ledek beállítása
  #ifdef  LED1
  GPIOX_CLRBIT(LED1); GPIOX_MODEPINOUT(LED1);
  #endif
  #ifdef  LED2
  GPIOX_CLRBIT(LED2); GPIOX_MODEPINOUT(LED2);
  #endif
}
//******************************************************************************

void main(void)
{
  static unsigned long cycle1, cycle2;  // 32 bites ciklusszámláló
  unsigned char c;

  // ha szükség van AD láb digitális I/O lábra történö átkapcsolásra
  #ifdef InitAD
  InitAD();
  #endif

  LcdInit();                            // müködésre bírjuk az LCD-t

  #if (1UL * LCDWIDTH * LCDLINES) < 80
  // 2x16 karakteres -------------------
  //                         "01020304050607080910111213141516"
  // memcpy((char *)LcdText, " LCD CPU used       meter       ", 32); // kezdöképernyö feltöltése
  #ifdef LCDBLINKCHAR
  for(c = 24; c < 32; c++)
    LcdBlinkChar(c);
  #endif // LCDBLINKCHAR
  #else  // (1UL * LCDWIDTH * LCDLINES) < 80
  // 4x40 karakteres -------------------
  //                                   "0204060810121416182022242628303234363840"
  memcpy((char *)LcdText +   0, "****************************************", 40); // kezdöképernyö feltöltése
  memcpy((char *)LcdText +  40, "*          LCD CPU used meter          *", 40); // kezdöképernyö feltöltése
  memcpy((char *)LcdText +  80, "*                                      *", 40); // kezdöképernyö feltöltése
  memcpy((char *)LcdText + 120, "****************************************", 40); // kezdöképernyö feltöltése
  #ifdef LCDBLINKCHAR
  for(c = 41; c < 78; c++)
    LcdBlinkChar(c);                    // második sor szöveg villog
  #endif // LCDBLINKCHAR
  #endif // else (1UL * LCDWIDTH * LCDLINES) < 80

  // megszakítás beállítások
  INTCONbits.PEIE = 1;                  // periféria IRQ eng
  INTCONbits.GIE = 1;                   // globális IRQ eng

  init();
  while(1)
  {
    cycle1 = 0; cycle2 = 0;             // számlálók nullázása
    
    // mérés 1 ---------------------------
    LcdRefreshStart();                  // kijelz?frissítés menjen
    #ifdef  LED1
    GPIOX_CLRBIT(LED1);
    #endif
    #ifdef  LED2
    GPIOX_SETBIT(LED2);
    #endif
    TMR1H = 0; TMR1L = 0;               // timer1 (óra) nullázás
    PIR1bits.TMR1IF = 0;                // timer1 túlcsordulásjelzés törlés
    T1CONbits.TMR1ON = 1;               // TMR1 start
    while(!PIR1bits.TMR1IF) cycle1++;   // T1 túlcsordulásáig számol
    LcdRefreshStop();                   // kijelz?frissítés álj
    
    // mérés 2 ---------------------------
    #ifdef  LED1
    GPIOX_SETBIT(LED1);
    #endif
    #ifdef  LED2
    GPIOX_CLRBIT(LED2);
    #endif
    TMR1H = 0; TMR1L = 0;               // timer1 (óra) nullázás
    PIR1bits.TMR1IF = 0;                // timer1 túlcsordulásjelzés törlés
    T1CONbits.TMR1ON = 1;               // TMR1 start
    while(!PIR1bits.TMR1IF) cycle2++;   // T1 túlcsordulásáig számol

    // megjelenítés -----------------------

    #if (1UL * LCDWIDTH * LCDLINES) < 80 
    // 2x16 karakteres -------------------
    memcpypgm2ram((char *)LcdText, "                        -CpuSpd-", 32); // kezdöképernyö feltöltése
    ltoa(cycle1, (char *)LcdText +  0);
    ltoa(cycle2, (char *)LcdText +  8);
    ltoa(100-((100*cycle1+50)/cycle2), (char *)LcdText + 16);
    #else  // (1UL * LCDWIDTH * LCDLINES) < 80
    // 4x40 karakteres -------------------
    //                            "0204060810121416182022242628303234363840"
    memcpypgm2ram((char *)LcdText +  80, "*With:        Without:              %  *", 40); // kezdöképernyö feltöltése
    ltoa(cycle1, (char *)LcdText +  86);
    ltoa(cycle2, (char *)LcdText + 102);
    ltoa(100-((100*cycle1+50)/cycle2), (char *)LcdText + 113);
    #endif // else (1UL * LCDWIDTH * LCDLINES) < 80 
  }
}

#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode()
{
  #if (LCDTIMERPR18 == -1) || (LCDTIMERPR18 == 1)
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif
}

#pragma interruptlow YourLowPriorityISRCode
void YourLowPriorityISRCode()
{
  #if LCDTIMERPR18 == 0
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
