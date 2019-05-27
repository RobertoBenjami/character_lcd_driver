//******************************************************************************
// LCD kijelzõ sebességét mérõ program
// készítõ: Roberto Benjami (robertodebenjami(at)gmail(dot)com)
//
// Mûködése:
// - TIMER1 segítségével megméri mennyi ideig tart az LCD frissítése BUSY flag figyeléses üzemmódban.
//   Kiírja az ebbõl adódó képkocka/másodperc (FPS) és a karakter/másodperc (CPS) értékét.
//
// charlcd.h szükséges beállításai:
// - Egyszeri frissítési üzemmód megszakítás nélkül, BUSY flag figyeléssel (LCDMODEONCEBUSY)
//******************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "charlcd.h"

#if (1UL * LCDWIDTH * LCDLINES < 80)
#define  FPSPOZ     0
#define  CPSPOZ     16
#else  // (1UL * LCDWIDTH * LCDLINES) < 80
#define  FPSPOZ     82
#define  CPSPOZ     98
#endif // else (1UL * LCDWIDTH * LCDLINES) < 80

#ifndef LCDMODEONCEBUSY
//#error "Rossz beállítások az lcdchar.h-ban"
#endif

//******************************************************************************
unsigned int t1cycle;                   // 16 bites TIMER1 értéke ide kerül

//----------------------------------------------------------------------------
// TIMER 1 írása, olvasása (paraméter: írás konstans vagy 16bites változó, olvasás 16bites változó)
#define SETTIMER1(t1)   {TCNT1H = t1 >> 8; TCNT1L = t1;}
#define GETTIMER1(t1)   {t1 = TCNT1L; t1 += TCNT1H << 8;}

//----------------------------------------------------------------------------
#define DELAYNUM (CpuClock/1000/14)
void delaymms(unsigned char ms)
{
  volatile unsigned int dm;

  while(ms--)
  {
    dm = DELAYNUM;
    while(dm--);
  }
}

//----------------------------------------------------------------------------
void lcdspeedscreen(void)
{
  static unsigned long ul;
  if(!(TIFR1 & (1 << TOV1)))
  {
    memcpy((char *)LcdText + FPSPOZ, "FPS:      ", 10);
    memcpy((char *)LcdText + CPSPOZ, "CPS:      ", 10);
    ul = (SystemClock/64UL) / ((unsigned long)t1cycle);
    ultoa(ul, (char *)LcdText +  FPSPOZ + 4, 10);
    ultoa(ul * (1UL * LCDLINES * LCDWIDTH), (char *)LcdText +  CPSPOZ + 4, 10);
  }
  else
  {
    memcpy((char *)LcdText + FPSPOZ, "FPS: ovf  ", 10);
    memcpy((char *)LcdText + CPSPOZ, "CPS: ovf  ", 10);
    TIFR1 &= ~(1 << TOV1);
  }
}

//----------------------------------------------------------------------------
void init(void)
{
  // 1. idõzítõ inicializálása
  // 64-es osztás:
  #define TMCLKSEL         3
  TCCR1B = (TMCLKSEL<<CS10);            // Clk = ClkIO/1..8..64..256..1024
}

//******************************************************************************
int main(void)
{
  // ha szükség van AD láb digitális I/O lábra történõ átkapcsolásra
  #ifdef InitAD
  InitAD();
  #endif

  LcdInit();                            // müködésre bírjuk az LCD-t

  #if (1UL * LCDWIDTH * LCDLINES) < 80 
  // 2x16 karakteres -------------------
  //                         "01020304050607080910111213141516"
  // memcpy((char *)LcdText, " LCD CPU used       meter       ", 32); // kezdõképernyõ feltöltése
  #else  // (1UL * LCDWIDTH * LCDLINES) < 80
  // 4x40 karakteres -------------------
  //                                   "0204060810121416182022242628303234363840"
  memcpy((char *)LcdText +   0, "****************************************", 40); // kezdõképernyõ feltöltése
  memcpy((char *)LcdText +  40, "*            LCD speed meter           *", 40); // kezdõképernyõ feltöltése
  memcpy((char *)LcdText +  80, "*                                      *", 40); // kezdõképernyõ feltöltése
  memcpy((char *)LcdText + 120, "****************************************", 40); // kezdõképernyõ feltöltése
  #endif // else (1UL * LCDWIDTH * LCDLINES) < 80

  init();

  while(1)
  {
    // mérés ---------------------------
    SETTIMER1(0);
    TIFR1 &= ~(1 << TOV1);
    //T1CONbits.TMR1ON = 1;               // bekapcs
    LcdRefreshAll();
    //T1CONbits.TMR1ON = 0;               // kikapcs
    GETTIMER1(t1cycle);

    // megjelenítés -----------------------
    lcdspeedscreen();
	delaymms(100);                        // 100ms várakozás
  }
}
