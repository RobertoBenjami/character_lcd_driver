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

#include <htc.h>
#include <string.h>
#include <stdlib.h>
#include "GenericTypeDefs.h"
#include "charlcd.h"

#if (1UL * LCDWIDTH * LCDLINES < 80)
#define  FPSPOZ     0
#define  CPSPOZ     16
#else  // (1UL * LCDWIDTH * LCDLINES) < 80
#define  FPSPOZ     82
#define  CPSPOZ     98
#endif // else (1UL * LCDWIDTH * LCDLINES) < 80

#ifndef LCDMODEONCEBUSY
#error "Rossz beállítások az lcdchar.h-ban"
#endif

//******************************************************************************
unsigned int t1cycle;                   // 16 bites TIMER1 értéke ide kerül

//----------------------------------------------------------------------------
// TIMER 1 írása, olvasása (paraméter: írás konstans vagy 16bites változó, olvasás 16bites változó)
#define SETTIMER1(t1)   {TMR1H = t1 >> 8; TMR1L = t1;}
#define CPTIMER1(t1)    {TMR1H = ((WORD_VAL*)&t1)->byte.HB; TMR1L = t1;}
#define GETTIMER1(t1)   {(((WORD_VAL*)&t1)->byte.LB) = TMR1L; (((WORD_VAL*)&t1)->byte.HB) = TMR1H;}

//----------------------------------------------------------------------------
void lcdspeedscreen(void)
{
  static unsigned long ul;
  if(!PIR1bits.TMR1IF)
  {
    memcpy((char *)LcdText + FPSPOZ, "FPS:      ", 10);
    memcpy((char *)LcdText + CPSPOZ, "CPS:      ", 10);
    ul = (SystemClock/4UL) / ((unsigned long)t1cycle << 3);
    ltoa((char *)LcdText +  FPSPOZ + 4, ul, 10);
    ltoa((char *)LcdText +  CPSPOZ + 4, ul * (1UL * LCDLINES * LCDWIDTH), 10);
  }
  else
  {
    memcpy((char *)LcdText + FPSPOZ, "FPS: ovf  ", 10);
    memcpy((char *)LcdText + CPSPOZ, "CPS: ovf  ", 10);
    PIR1bits.TMR1IF = 0;
  }
}

//----------------------------------------------------------------------------
void init(void)
{
  // 1. idõzítõ inicializálása
  T1CONbits.T1OSCEN = 0;                // órakvarc tiltás
  T1CONbits.TMR1CS = 0;                 // forrás órajel = CPU clk
  T1CONbits.T1CKPS = 3;                 // elõosztó : 1:8
  T1CONbits.TMR1ON = 0;                 // kikapcs
}

//******************************************************************************
void main(void)
{
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
    T1CONbits.TMR1ON = 1;               // bekapcs
    LcdRefreshAll();
    T1CONbits.TMR1ON = 0;               // kikapcs
    GETTIMER1(t1cycle);

    // megjelenítés -----------------------
    lcdspeedscreen();
  }
}
