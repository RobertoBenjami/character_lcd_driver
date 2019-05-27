//******************************************************************************
// Ez a "charlcd" LCD kijelzõ kurzorhasználati lehetõségét mutatja be.
// készítõ: Roberto Benjami (robertodebenjami(at)gmail(dot)com)
//
// Mûködése:
// - A kijelzõn a karakterkészletet visszafelé gördíti, miközben a kurzort elõre viszi.
//   A kurzort e közben ki-be kapcsolgatja, és a formáját is váltogatja.
//
// charlcd.h szükséges beállításai:
// - egyszeri frissítési üzemmód (LCDMODEONCEBUSY vagy LCDMODEONCEDELAY vagy LCDMODEONCEIRQ)
// - kurzor (LCDCURSOR be)
// készítõ: Roberto Benjami (robertodebenjami(at)gmail(dot)com)
//******************************************************************************

#include <p18cxxx.h>
#include <string.h>
#include <stdlib.h>
#include <timers.h>
#include "charlcd.h"
#include "bootloaders.h"

// led lábak definiálása (ha nincs, töröljük a LEDS definíciót)
#define  LEDS
#define LED1       B, 0
#define LED2       B, 1

#define GLUE(a, b)   a##b
#define SET_(what, p, m)       GLUE(what, p) |= (1 << (m))
#define CLR_(what, p, m)       GLUE(what, p) &= ~(1 << (m))
#define GET_(/* PORT, */ p, m) GLUE(PORT, p) & (1 << (m))
#define IOIN(x)                SET_(TRIS, x)
#define IOOUT(x)               CLR_(TRIS, x)
#define SET(x)                 SET_(LAT, x)
#define CLR(x)                 CLR_(LAT, x)
#define GET(/* PORT, */ x)     GET_(x)

//******************************************************************************
void mwait(void)
{
  #define CpuClock 4000000
  #define DELAYNUM (CpuClock/1000/14)
  static unsigned char cc;
  volatile unsigned int dh, dl;

  #ifdef  LEDS
  if(GET(LED1)) CLR(LED1); else SET(LED2);
  #endif

  dh = 1000;
  while(dh--)
  {
    dl = DELAYNUM;
    while(dl--);
  }

  LcdText[0]++;
  for(cc = 0; cc < sizeof(LcdText) - 2; cc++)
    LcdText[cc + 1] = LcdText[cc] + 1;
}

//******************************************************************************

void main(void)
{
  unsigned char c;

  // ha szükség van AD láb digitális I/O lábra történõ átkapcsolásra
  #ifdef InitAD
  InitAD();
  #endif

  // ledek beállítása
  #ifdef  LEDS
  CLR(LED1); CLR(LED2); IOOUT(LED1); IOOUT(LED2);
  #endif

  LcdInit();                            // müködésre bírjuk az LCD-t

  // megszakítás beállítások
  RCONbits.IPEN = 1;                    // kétszintû IRQ eng
  INTCONbits.GIEL = 1;                  // globális LO IRQ eng
  INTCONbits.GIEH = 1;                  // globális IRQ eng

  while(1)
  {
    LcdRefreshAll();
    mwait();
    LcdCursorPos++;
    LcdCursorOff();

    LcdRefreshAll();
    mwait();
    LcdCursorPos++;
    LcdCursorOn();

    LcdRefreshAll();
    mwait();
    LcdCursorPos++;
    LcdCursorBlink();

    LcdRefreshAll();
    mwait();
    LcdCursorPos++;
    LcdCursorUnBlink();

    if(LcdCursorPos >= sizeof(LcdText) - 2) LcdCursorPos = 0;
  }
}

#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode()
{
}

#pragma interruptlow YourLowPriorityISRCode
void YourLowPriorityISRCode()
{
  #ifdef LCDTIMERINT
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif
}
