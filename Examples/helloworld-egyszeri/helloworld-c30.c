//******************************************************************************
// 16 bites PIC helloworld az charlcd teszteléséhez
// készítõ: Roberto Benjami (robertodebenjami(at)gmail(dot)com)
//
// charlcd.h szükséges beállításai:
// - LCDMODEONCEBUSY vagy LCDMODEONCEDELAY üzemmód
// - a többi beállítás bármi lehet, csak a felhasznált mikrovezérlõ is támogassa
//   (pl.a beállított I/O láb és a TIMER létezzen)
// - adott esetben az I/O lábak analógról -> digitálisra történõ átállításáról gondoskodjunk!
// - két kijelzõ (LCDSTEREO) használata esetén, csak az elsõ kijelzõn fog megjelenni az felirat
//   (ha hosszabb szöveget írunk az LcdText-be, akkor a második kijelzõre is át fog lógni)
//******************************************************************************

#include <string.h>
#include "charlcd.h"

int main(void)
{
  LcdInit();                            // müködésre bírjuk az LCD-t
  //                     "01020304050607080910111213141516"
  memcpy((char*)LcdText, "   Hello world                  ", 32);
  LcdRefreshAll();
  while(1)
  {
  }
}
