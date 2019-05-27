//******************************************************************************
// "charlcd" LCD kijelzõ frissítésének processzorhasználat mérõ program
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
// - 4/8 bites üzemmód (LCD4BITMODE) hatását
// - villogtatás ki/be hatását (LCDBLINKCHAR)
// - a különbözõ CPU órajel hatását
//******************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/ina90.h>
#include <string.h>
#include <stdlib.h>
#include "charlcd.h"

// led lábak definiálása
// #define  LEDS
#define  LED1ON     PORTB |=  (1 << 2)
#define  LED2ON     PORTB |=  (1 << 3)
#define  LED1OFF    PORTB &= ~(1 << 2)
#define  LED2OFF    PORTB &= ~(1 << 3)
#define  LED1IO     DDRB |=  (1 << 2)
#define  LED2IO     DDRB |=  (1 << 3)
#define  LED1       (PORTB & (1 << 2))
#define  LED2       (PORTB & (1 << 3))

// 1 megszakítás/sec
#define  TIMECLK    1

// órajel forrás frekvencia
#define  TimerSrcClock (1UL*SystemClock)

#if (!defined LCDMODECONTIRQ)
#error "A tesztprogram LCDMODECONTIRQ üzemmódban mûködik!"
#endif

//******************************************************************************
void init(void)
{
  // idõzítõ inicializálása
  // timer 1
  #define  TIMEDIV ((TimerSrcClock+TIMECLK/2)/TIMECLK-1)
  #if TIMEDIV < 65536
  #define TMCLKDIV         1
  #define TMCLKSEL         1
  #elif TIMEDIV < (65536*8)
  #define TMCLKDIV         8
  #define TMCLKSEL         2
  #elif TIMEDIV < (65536*64)
  #define TMCLKDIV        64
  #define TMCLKSEL         3
  #elif TIMEDIV < (65536*256)
  #define TMCLKDIV       256
  #define TMCLKSEL         4
  #elif TIMEDIV < (65536*1024)
  #define TMCLKDIV      1024
  #define TMCLKSEL         5
  #endif // TIMEDIV

  #define TMCOMP (((TimerSrcClock/TMCLKDIV)+TIMECLK/2)/TIMECLK-1)
  OCR1AH = TMCOMP >> 8;                 // comparator HI
  OCR1AL = (unsigned char)TMCOMP;       // comparator LO
  TCCR1B = (0<<WGM12)|(TMCLKSEL<<CS10); // mode0, Clk = ClkIO/1..8..64..256..1024

  #ifdef  LEDS
  LED1OFF; LED2OFF; LEDIO1; LEDIO2;
  #endif

}
//******************************************************************************

int main(void)
{
  static unsigned long cycle1, cycle2;  // 32 bites ciklusszámláló
  unsigned char c;

  LcdInit();                            // müködésre bírjuk az LCD-t

  #if (1UL * LCDWIDTH * LCDLINES) < 80 
  // 2x16 karakteres -------------------
  #ifdef LCDBLINKCHAR
  for(c = 24; c < 32; c++)
    LcdBlinkChar(c);
  #endif // LCDBLINKCHAR
  #else  // (1UL * LCDWIDTH * LCDLINES) < 80
  // 4x40 karakteres -------------------
  memcpy((char *)LcdText +   0, "****************************************", 40); // kezdõképernyõ feltöltése
  memcpy((char *)LcdText +  40, "*          LCD CPU used meter          *", 40); // kezdõképernyõ feltöltése
  memcpy((char *)LcdText +  80, "*                                      *", 40); // kezdõképernyõ feltöltése
  memcpy((char *)LcdText + 120, "****************************************", 40); // kezdõképernyõ feltöltése
  #ifdef LCDBLINKCHAR
  for(c = 41; c < 78; c++)
    LcdBlinkChar(c);                    // második sor szöveg villog
  #endif // LCDBLINKCHAR
  #endif // else (1UL * LCDWIDTH * LCDLINES) < 80

  init();
  while(1)
  {
    cycle1 = 0; cycle2 = 0;             // számlálók nullázása
    
    // mérés 1 ---------------------------
    LcdRefreshStart();                  // kijelzõfrissítés menjen
    #ifdef  LEDS
    LED1OFF; LED2ON;
    #endif
    TCNT1H = 0; TCNT1L = 0;             // timer1 (óra) nullázás
	#ifdef TIFR
    TIFR = (1 << OCF1A);                // timer1 túlcsordulásjelzés törlés
    while(!(TIFR & (1<<OCF1A))) cycle1++;// idõ leteltéig számol
	#else
    TIFR1 = (1 << OCF1A);               // timer1 túlcsordulásjelzés törlés
    while(!(TIFR1 & (1<<OCF1A))) cycle1++;// idõ leteltéig számol
	#endif
    LcdRefreshStop();
    
    // mérés 2 ---------------------------
    #ifdef  LEDS
    LED1ON; LED2OFF;
    #endif
    TCNT1H = 0; TCNT1L = 0;             // timer1 (óra) nullázás
	#ifdef TIFR
    while(!(TIFR & (1<<OCF1A))) cycle2++;// idõ leteltéig számol
    TIFR = (1 << OCF1A);                // timer1 túlcsordulásjelzés törlés
	#else
    TIFR1 = (1 << OCF1A);               // timer1 túlcsordulásjelzés törlés
    while(!(TIFR1 & (1<<OCF1A))) cycle2++;// idõ leteltéig számol
	#endif

    // megjelenítés -----------------------
    #if (1UL * LCDWIDTH * LCDLINES) < 80 
    // 2x16 karakteres -------------------
    memcpy((char *)LcdText, "                                ", 32);
    ltoa(cycle1,  (char *)LcdText +  0, 10);
    ltoa(cycle2,  (char *)LcdText +  8, 10);
    ltoa(100-(100*cycle1/cycle2), (char *)LcdText + 16, 10);
    #else  // (1UL * LCDWIDTH * LCDLINES) < 80
    // 4x40 karakteres -------------------
    //                            "0204060810121416182022242628303234363840"
    memcpy((char *)LcdText +  80, "*With:        Without:              %  *", 40);
    ltoa(cycle1,  (char *)LcdText +  86, 10);
    ltoa(cycle2,  (char *)LcdText + 102, 10);
    ltoa(100-(100*cycle1/cycle2), (char *)LcdText + 113, 10);
    #endif // else (1UL * LCDWIDTH * LCDLINES) < 80 
  }
}
