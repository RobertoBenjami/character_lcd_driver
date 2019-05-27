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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/ina90.h>
#include <string.h>
#include <stdlib.h>
#include "charlcd.h"

// led lábak definiálása (ha nincs, töröljük a LEDS definíciót)
#define  LEDS
#define  LED1ON     PORTB |=  (1 << 5)
#define  LED1OFF    PORTB &= ~(1 << 5)
#define  LED1IO     DDRB  |=  (1 << 5)
#define  LED1       (PORTB & (1 << 5))

//******************************************************************************
void mwait(void)
{
  #define DELAYNUM (CpuClock/1000/14)
  static unsigned char cc;
  volatile unsigned int dh, dl;

  #ifdef LEDS
  if(LED1) LED1ON; else LED1OFF;
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
int main(void)
{
  unsigned char c;
  #ifdef LEDS
  LED1IO;
  #endif

  LcdInit();                            // müködésre bírjuk az LCD-t

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
