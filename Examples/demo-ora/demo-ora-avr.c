//******************************************************************************
// 8 bites AVR óra demo az charlcd teszteléséhez
// készítõ: Roberto Benjami (robertodebenjami(at)gmail(dot)com)
//
// charlcd.h szükséges beállításai:
// - a beállítás bármi lehet, csak a felhasznált mikrovezérlõ is támogassa,
//   (pl. a beállított I/O láb és a TIMER létezzen)
//   és az egymástól függõ beállításoknál ne legyen összeférhetetlénség
//   (pl. kurzor csak egyszeri frissítési módban legyen),
// - adott esetben a felhasznált I/O lábak analógról -> digitálisra történõ átállításáról gondoskodjunk!
//   (ezt vagy a hardvareprofile.h InitAD() definíciójába, vagy akár ide a main után is beírhatjuk)
// - legalább 32 karakter legyen a kijelzõ(kö)n, (legalább 2x16, LCDSTEREO esetén 1x16)
//******************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "charlcd.h"

// órajel forrás frekvencia
#define  TimerSrcClock (1UL*SystemClock)

// 100 megszakítás/sec
#define  TIMECLK    100

// melyik idõzítõn járjon az óra ? (hogy ne ütközzön az lcdchar által felhasználtal)
#if (LCDTIMERNUM == 1)
#define  CLOCKTIMERNUM   0
#else
#define  CLOCKTIMERNUM   1
#endif

// led lábak definiálása (ha nincs, töröljük a LEDS definíciót)
#define  LEDS
#define  LED1ON     PORTB |=  (1 << 4)
#define  LED2ON     PORTB |=  (1 << 5)
#define  LED1OFF    PORTB &= ~(1 << 4)
#define  LED2OFF    PORTB &= ~(1 << 5)
#define  LED1IO     DDRB |=  (1 << 4)
#define  LED2IO     DDRB |=  (1 << 5)
#define  LED1       (PORTB & (1 << 4))
#define  LED2       (PORTB & (1 << 5))

unsigned int ev = 2014;
unsigned char honap = 12;
unsigned char nap = 24;
unsigned char ora = 23;
unsigned char perc = 58;
volatile unsigned char masodperc = 0;
unsigned char honaphosszak[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#ifdef USERCHARSETCHANGE
USERCHARSETARRAY userchars;
#endif

//******************************************************************************
void init(void)
{
  // idõzítõ inicializálása
  #define  TIMEDIV ((TimerSrcClock+TIMECLK/2)/TIMECLK-1)
  // timer 0
  #if CLOCKTIMERNUM == 0
  #if TIMEDIV < 256
  #define TMCLKDIV         1
  #define TMCLKSEL         1
  #elif TIMEDIV < (256*8)
  #define TMCLKDIV         8
  #define TMCLKSEL         2
  #elif TIMEDIV < (256*64)
  #define TMCLKDIV        64
  #define TMCLKSEL         3
  #elif TIMEDIV < (256*256)
  #define TMCLKDIV       256
  #define TMCLKSEL         4
  #elif TIMEDIV < (256*1024)
  #define TMCLKDIV      1024
  #define TMCLKSEL         5
  #endif // TIMEDIV
  #define TMCOMP (((TimerSrcClock/TMCLKDIV)+TIMECLK/2)/TIMECLK-1)

  #if defined OCR0 || defined OCR0A
  #ifdef  OCR0A
  OCR0A = TMCOMP;
  #ifdef TCCR0B // (A/B regiszteres TIMER0)
  TCCR0A = (1<<WGM01);                  // MODE1
  TCCR0B = (TMCLKSEL<<CS00);            // frekvencia osztás
  #else  // TCCR0B (A regiszteres TIMER0)
  TCCR0A = (1<<CTC0)|(TMCLKSEL<<CS00);  // CTC mode, frekvencia osztás
  #endif // TCCR0B
  #else  // OCR0A
  OCR0  = TMCOMP;
  TCCR0 = (1<<WGM01)|(TMCLKSEL<<CS00);  // CTC mode, frekvencia osztás
  #endif // else OCR0A
  
  #ifdef TIMSK0
  TIMSK0 = (1<<OCIE0A);
  #else  // TIMSK0
  TIMSK |= (1<<OCIE0);
  #endif // TIMSK0

  #else  // defined OCR0 || defined OCR0A
  TCNT0 = 255 - TMCOMP;                 // comparator helyett kezdõérték
  TCCR0 = TMCLKSEL<<CS00;               // frekvencia osztás
  TIMSK |= (1<<TOIE0);                  // overflow IRQ eng
  #endif // defined OCR0 || defined OCR0A

  // timer 1
  #else
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
  TCCR1A = (0<<WGM10);                  // mode4 (CTC)
  TCCR1B = (1<<WGM12)|(TMCLKSEL<<CS10); // mode4, Clk = ClkIO/1..8..64..256..1024
  #ifdef TIMSK1
  TIMSK1 = (1<<OCIE1A);
  #else  // TIMSK1
  TIMSK |= (1<<OCIE1A);
  #endif // TIMSK1
  #endif

  sei();
  
  // ledek beállítása
  #ifdef LEDS
  LED1OFF; LED2OFF; LED1IO; LED2IO;
  #endif
}

int main(void)
{
  unsigned char lastsec = 255;
  #ifdef USERCHARSETCHANGE
  unsigned char i, j;
  memcpy_P(userchars, &userromcharset, 64);
  #endif

  LcdInit();                            // müködésre bírjuk az LCD-t
  //                      "01020304050607080910111213141516"
  memcpy((char *)LcdText, "   eeee.hh.nn       oo:pp:mm    ", 32); // kezdõképernyõ feltöltése

  init();
  while(1)
  {
    // polling mód, folyamatos frissítési üzemmód 
    #ifdef LCDMODECONTBUSY
    LcdProcess();                       // polling mód esetén frissítjük a kijelzõ tartalmát
    #endif

    if(lastsec != masodperc)
    {
      // 2 led másodpercenkéánt felváltva villogjon
      #ifdef LEDS
      if(LED1) 
        {LED1OFF; LED2ON;}
      else 
        {LED1ON; LED2OFF;}
      #endif
    
      // Ha nincs automata villogtatás 1sec a villogási idõzítése
      #if (defined LCDBLINKCHAR) && (!defined LCDMODECONTIRQ) || (LCDBLINKSPEED == 0)
      BlinkPhase = !BlinkPhase;
      #endif

      // jár az óra jár
      if(masodperc == 60)
      {
        masodperc = 0;
        if(++perc == 60)
        {
          perc = 0;
          if(++ora == 24)
          {
            ora = 0;
            if(++nap > honaphosszak[honap - 1])
            {
              nap = 1;
              if(++honap == 13)
              {
                honap = 1;
                ev++;
      } } } } }
      lastsec = masodperc;

      // számjegyek kiírása
      itoa(ev, (char *)LcdText + 3, 10);
      itoa(honap, (char *)LcdText + 8, 10);
      itoa(nap, (char *)LcdText + 11, 10);
      itoa(ora, (char *)LcdText + 20, 10);
      itoa(perc, (char *)LcdText + 23, 10);
      itoa(masodperc, (char *)LcdText + 26, 10);

      // egyszámjegyû számok kétszámjegyüvé alakítása
      if(!LcdText[9]) {LcdText[9]  = LcdText[8];  LcdText[8]  = '0';}
      if(!LcdText[12]){LcdText[12] = LcdText[11]; LcdText[11] = '0';}
      if(!LcdText[21]){LcdText[21] = LcdText[20]; LcdText[20] = '0';}
      if(!LcdText[24]){LcdText[24] = LcdText[23]; LcdText[23] = '0';}
      if(!LcdText[27]){LcdText[27] = LcdText[26]; LcdText[26] = '0';}

      // itoa által felülírt írásjelek pótlása
      LcdText[7] = '.';
      LcdText[10] = '.';
      LcdText[22] = ':';
      LcdText[25] = ':';

      // számok közötti kettõspont villogtatás
      #ifdef LCDBLINKCHAR
      LcdBlinkChar(22);
      LcdBlinkChar(25);
      #endif

       // felhasználó által definiált karakterek a sarkokban
      #ifdef USERCHARSET
      LcdText[0] = 8;
      LcdText[1] = 10;
      LcdText[16] = 9;
      LcdText[17] = 11;
      LcdText[14] = 12;
      LcdText[15] = 14;
      LcdText[30] = 13;
      LcdText[31] = 15;
      #endif

     // kurzor léptetése és kurzortípus váltogatása
      #ifdef LCDCURSOR
      LcdCursorPos++;
      if(LcdCursorPos >= (1UL * LCDWIDTH * LCDLINES))
        LcdCursorPos = 0;
      if((LcdCursorPos & 0b11) == 0b00) LcdCursorOff();
      else if((LcdCursorPos & 0b11) == 0b01) {LcdCursorUnBlink(); LcdCursorOn();}
      else if((LcdCursorPos & 0b11) == 0b10) {LcdCursorBlink(); LcdCursorOn();}
      else if((LcdCursorPos & 0b11) == 0b11) {LcdCursorUnBlink(); LcdCursorOn();}
      #endif

      // karakterkészlet shiftelése és feltöltése (csak ha karakterkészlet változtatásának lehetõsége engedve van)
      #ifdef USERCHARSETCHANGE
      j = userchars[63];
      i = 63;
      while(i-- > 0)
        userchars[i + 1] = userchars[i];
      userchars[0] = j;
      LcdChangeCharset(userchars);
      #endif // USERCHARSETCHANGE

      // egyszeri frissítési üzemmódban megjelenítés
      #if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEWAIT) || (defined LCDMODEONCEIRQ)) 
      LcdRefreshAll();
      #endif
    }
  }
}

//******************************************************************************
// AVR megszakítás kiszolgálás
#define TIMER0COMPLESS     ;
#if CLOCKTIMERNUM == 0
#if defined OCR0 || defined OCR0A
#ifdef  TIMER0_COMP_vect
ISR (TIMER0_COMP_vect)
#else  // TIMER0_COMP_vect
ISR (TIMER0_COMPA_vect)
#endif // else TIMER0_COMP_vect
#else  // defined OCR0 || defined OCR0A
ISR (TIMER0_OVF_vect)
#undef  TIMER0COMPLESS
#define TIMER0COMPLESS     TCNT0 += (255 - TMCOMP)
#endif // else defined OCR0 || defined OCR0A
#else  // CLOCKTIMERNUM == 0
ISR (TIMER1_COMPA_vect)
#endif // else CLOCKTIMERNUM == 0
{
  static unsigned char msec10 = TIMECLK;
  TIMER0COMPLESS;
  #ifdef LCDUSERTIMER
  LcdIntProcess();
  #endif
  msec10--;
  if(!msec10)
  {
    masodperc++;
    msec10 = TIMECLK;
  }
}
