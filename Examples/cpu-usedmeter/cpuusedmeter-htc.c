//******************************************************************************
// "charlcd" LCD kijelzõ frissítésének processzorhasználat mérõ program, htc, xc8 forditókhoz
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
// - ne az 1.timert használjuk (LCDTIMERNUM)
//
// letesztelhetjük:
// - LCDFRAMEPERSEC változtatásának a hatását
// - 4/8 bites üzemmód hatását
// - villogtatás ki/be hatását (LCDBLINKCHAR)
// - a különbözõ CPU órajel hatását
//******************************************************************************

#if defined(__XC__)
#include <xc.h>
#else
#include <htc.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "charlcd.h"

// led lábak definiálása
#define  LED1       B, 0
#define  LED2       B, 1

#if (!defined LCDMODECONTIRQ) || LCDTIMERNUM == 1
#error "Rossz beállítások az charlcd.h-ban"
#endif

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
  // 1. idõzítõ inicializálása
  T1CONbits.T1OSCEN = 1;                // órakvarc eng
  T1CONbits.TMR1CS = 1;                 // forrás órajel = CPU clk
  T1CONbits.T1CKPS = 0;                 // elõosztó : 1:1
  T1CONbits.TMR1ON = 0;                 // kikapcs

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
  static unsigned long cycle1, cycle2;  // 32 bites ciklusszámláló
  unsigned char c;

  // ha szükség van AD láb digitális I/O lábra történõ átkapcsolásra
  #ifdef InitAD
  InitAD();
  #endif

  LcdInit();                            // müködésre bírjuk az LCD-t

  #if (1UL * LCDWIDTH * LCDLINES) < 80 
  // 2x16 karakteres -------------------
  //                         "01020304050607080910111213141516"
  // memcpy((char *)LcdText, " LCD CPU used       meter       ", 32); // kezdõképernyõ feltöltése
  #ifdef LCDBLINKCHAR
  for(c = 24; c < 32; c++)
    LcdBlinkChar(c);
  #endif // LCDBLINKCHAR
  #else  // (1UL * LCDWIDTH * LCDLINES) < 80
  // 4x40 karakteres -------------------
  //                                   "0204060810121416182022242628303234363840"
  memcpy((char *)LcdText +   0, "****************************************", 40); // kezdõképernyõ feltöltése
  memcpy((char *)LcdText +  40, "*          LCD CPU used meter          *", 40); // kezdõképernyõ feltöltése
  memcpy((char *)LcdText +  80, "*                                      *", 40); // kezdõképernyõ feltöltése
  memcpy((char *)LcdText + 120, "****************************************", 40); // kezdõképernyõ feltöltése
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
    LcdRefreshStart();                  // kijelzõfrissítés menjen
    #ifdef  LED1
    CLR(LED1);
    #endif
    #ifdef  LED2
    SET(LED2);
    #endif
    TMR1H = 0; TMR1L = 0;               // timer1 (óra) nullázás
    PIR1bits.TMR1IF = 0;                // timer1 túlcsordulásjelzés törlés
    T1CONbits.TMR1ON = 1;               // TMR1 start
    while(!PIR1bits.TMR1IF) cycle1++;   // T1 túlcsordulásáig számol
    LcdRefreshStop();                   // kijelzõfrissítés álj
    
    // mérés 2 ---------------------------
    #ifdef  LED1
    SET(LED1);
    #endif
    #ifdef  LED2
    CLR(LED2);
    #endif
    TMR1H = 0; TMR1L = 0;               // timer1 (óra) nullázás
    PIR1bits.TMR1IF = 0;                // timer1 túlcsordulásjelzés törlés
    T1CONbits.TMR1ON = 1;               // TMR1 start
    while(!PIR1bits.TMR1IF) cycle2++;   // T1 túlcsordulásáig számol

    // megjelenítés -----------------------

    #if (1UL * LCDWIDTH * LCDLINES) < 80 
    // 2x16 karakteres -------------------
    memcpy((char *)LcdText, "                        -CpuSpd-", 32); // kezdõképernyõ feltöltése
    ltoa((char *)LcdText +  0, cycle1, 10);
    ltoa((char *)LcdText +  8, cycle2, 10);
    ltoa((char *)LcdText + 16, 100-((100*cycle1+50)/cycle2), 10);
    #else  // (1UL * LCDWIDTH * LCDLINES) < 80
    // 4x40 karakteres -------------------
    //                            "0204060810121416182022242628303234363840"
    memcpy((char *)LcdText +  80, "*With:        Without:              %  *", 40); // kezdõképernyõ feltöltése
    ltoa((char *)LcdText +  86, cycle1, 10);
    ltoa((char *)LcdText + 102, cycle2, 10);
    ltoa((char *)LcdText + 113, 100-((100*cycle1+50)/cycle2), 10);
    #endif // else (1UL * LCDWIDTH * LCDLINES) < 80 
  }
}

//******************************************************************************
// PIC16 megszakítás kiszolgálás
static void interrupt isr(void)
{
  #if (!defined LCDUSERTIMER) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif
}
