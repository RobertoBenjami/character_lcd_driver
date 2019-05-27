//******************************************************************************
// 8 bites 18Fxxx helloworld demo az charlcd teszteléséhez
// készítõ: Roberto Benjami (robertodebenjami(at)gmail(dot)com)
//
// charlcd.h szükséges beállításai:
// - LCDMODEONCEIRQ üzemmód
// - a többi beállítás bármi lehet, csak a felhasznált mikrovezérlõ is támogassa
//   (pl.a beállított I/O láb és a TIMER létezzen)
// - adott esetben az I/O lábak analógról -> digitálisra történõ átállításáról gondoskodjunk!
// - két kijelzõ (LCDSTEREO) használata esetén, csak az elsõ kijelzõn fog megjelenni az felirat
//   (ha hosszabb szöveget írunk az LcdText-be, akkor a második kijelzõre is át fog lógni)
//******************************************************************************

#include <htc.h>
#include <string.h>
#include "charlcd.h"

void main(void)
{
  LcdInit();                            // müködésre bírjuk az LCD-t
  //                      "01020304050607080910111213141516"
  memcpy((char *)LcdText, "   Hello world                  ", 32); // kezdõképernyõ feltöltése
  LcdRefreshAll();
  while(1)
  {
  }
}

#if LCDTIMERPR18 == 0
void interrupt low_priority irql(void)
{
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
}
#endif

#if (LCDTIMERPR18 == -1) || (LCDTIMERPR18 == 1)
void interrupt irqh(void)
{
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
}
#endif
