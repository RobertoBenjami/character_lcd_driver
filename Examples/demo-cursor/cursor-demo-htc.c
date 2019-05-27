//******************************************************************************
// Ez a "charlcd" LCD kijelzõ kurzorhasználati lehetõségét mutatja be
// 16fxxx, 18fxxx, htc, htc18, xc8 forditókhoz
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

#if defined(__XC__)
#include <xc.h>
#else
#include <htc.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "charlcd.h"

// led lábak definiálása (ha nincs, töröljük a LEDS definíciót)
#define LED1                  B, 0
#define LED2                  B, 1

#define TRISBIT(p, m)         TRIS ## p ## bits.TRIS ## p ## m
#define LATBIT(p, m)          LAT ## p ## bits.LAT ## p ## m
#define PORTBIT(p, m)         PORT ## p ## bits.R ## p ## m

#define GPIOX_MODEPININ(x)    TRISBIT(x) = 1
#define GPIOX_MODEPINOUT(x)   TRISBIT(x) = 0
#if (defined(_PIC14E) || defined(__PICC18__))
#define GPIOX_SETBIT(x)       LATBIT(x) = 1
#define GPIOX_CLRBIT(x)       LATBIT(x) = 0
#else
#define GPIOX_SETBIT(x)       PORTBIT(x) = 1
#define GPIOX_CLRBIT(x)       PORTBIT(x) = 0
#endif
#define GPIOX_GETBIT(x)       PORTBIT(x)

//******************************************************************************
void mwait(void)
{
  #define DELAYNUM (CpuClock/1000/14)
  static unsigned char cc;
  volatile unsigned int dh, dl;

  #ifdef  LED1
  if(GPIOX_GETBIT(LED1)) 
  {
    GPIOX_CLRBIT(LED1);
    #ifdef  LED2
    GPIOX_SETBIT(LED2);
    #endif
  }
  else 
  {
    GPIOX_SETBIT(LED1);
    #ifdef  LED2
    GPIOX_CLRBIT(LED2);
    #endif
  }
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
  #define LED3 B, 2
  #define LED4 B, 3

  // ledek beállítása
  #ifdef  LED1
  GPIOX_CLRBIT(LED1); GPIOX_MODEPINOUT(LED1);
  #endif
  #ifdef  LED2
  GPIOX_CLRBIT(LED2); GPIOX_MODEPINOUT(LED2);
  GPIOX_CLRBIT(LED3); GPIOX_MODEPINOUT(LED3);
  GPIOX_CLRBIT(LED4); GPIOX_MODEPINOUT(LED4);
  #endif

  LcdInit();                            // müködésre bírjuk az LCD-t

  while(1)
  {
  	if(LcdCursorPos >= LCDLINES * LCDWIDTH) LcdCursorPos = 0;
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
  }
}

//******************************************************************************

#if defined(__PICC18__)
// PIC18 megszakítás kiszolgálás
void interrupt low_priority irql(void)
{
  #if (!defined LCDUSERTIMER) && (LCDTIMERPR18 == 0) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif
}

void interrupt irqh(void)
{
  #if (!defined LCDUSERTIMER) && ((LCDTIMERPR18 == -1) || (LCDTIMERPR18 == 1)) && (defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ)
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif
}

#else
// PIC16 megszakítás kiszolgálás
static void interrupt isr(void)
{
  #if (!defined LCDUSERTIMER) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
  LcdIntProcess();                      // interrupt mód esetén frissítjük a kijelzõ tartalmát
  #endif
}
#endif

