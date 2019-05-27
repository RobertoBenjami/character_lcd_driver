//******************************************************************************
// 16 bites PIC óra demo az charlcd teszteléséhez, c30, xc16 forditóhoz
// készítö: Roberto Benjami (robertodebenjami(at)gmail(dot)com)
//
// charlcd.h szükséges beállításai:
// - a beállítás bármi lehet, csak a felhasznált mikrovezérlö is támogassa,
//   (pl. a beállított I/O láb és a TIMER létezzen)
//   és az egymástól függö beállításoknál ne legyen összeférhetetlénség
//   (pl. kurzor csak egyszeri frissítési módban legyen),
// - adott esetben a felhasznált I/O lábak analógról -> digitálisra történö átállításáról gondoskodjunk!
//   (ezt vagy a hardvareprofile.h InitAD() definíciójába, vagy akár ide a main után is beírhatjuk)
// - legalább 32 karakter legyen a kijelzö(kö)n, (legalább 2x16, LCDSTEREO esetén 1x16)
//******************************************************************************

// include, órajel forrás frekvencia
#if defined(__dsPIC30F__)
#include <p30Fxxxx.h>
#define TimerSrcClock (1UL*SystemClock/4)
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#define TimerSrcClock (1UL*SystemClock/2)
#elif defined(__PIC24F__)
#include <p24Fxxxx.h>
#define TimerSrcClock (1UL*SystemClock/2)
#elif defined(__PIC24FK__)
#include <p24Fxxxx.h>
#define TimerSrcClock (1UL*SystemClock/2)
#elif defined(__PIC24H__)
#include <p24Hxxxx.h>
#define TimerSrcClock (1UL*SystemClock/2)
#endif

#include <timer.h>
#include <string.h>
#include <stdlib.h>
#include "charlcd.h"

// 1000 megszakítás/sec
#define  TIMECLK  1000

// melyik idözitön járjon az óra? (hogy ne ütközzön az lcdchar által felhasználtal)
#if (LCDTIMERNUM == 1)
#define  CLOCKTIMERNUM   2
#else
#define  CLOCKTIMERNUM   1
#endif

// 1msec megszakítás
#define  TMCOMP (((TimerSrcClock)+1000/2)/1000-1)

unsigned int ev = 2013;
unsigned char honap = 12;
unsigned char nap = 27;
unsigned char ora = 23;
unsigned char perc = 58;
volatile unsigned char masodperc = 0;
unsigned char honaphosszak[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#ifdef USERCHARSETCHANGE
USERCHARSETARRAY userchars;
#endif

// osztásarány
#define  TIMEDIV ((TimerSrcClock+TIMECLK/2)/TIMECLK-1)

//******************************************************************************
void init(void)
{
  // PIC24 TIMER2 idözítö inicializálása
  #if CLOCKTIMERNUM == 1
  OpenTimer1(T1_ON & T1_GATE_OFF & T1_PS_1_1 & T1_SOURCE_INT & T1_INT_ON, TIMEDIV);
  SetPriorityIntT1(1);                  // legkisebb prioritás
  EnableIntT1;
  #else
  OpenTimer2(T2_ON & T2_GATE_OFF & T2_PS_1_1 & T2_32BIT_MODE_OFF & T2_SOURCE_INT & T2_INT_ON, TIMEDIV);
  SetPriorityIntT2(1);                  // legkisebb prioritás
  EnableIntT2;
  #endif
}
//******************************************************************************

int main(void)
{
  static unsigned char lastsec;
  #ifdef USERCHARSETCHANGE
  unsigned char i, j;
  // felhasználói karakterkészlet másolása RAM-ba (csak ha engedélyezve van)
  memcpy((char *)userchars, userromcharset, sizeof(USERCHARSETARRAY)); 
  #endif

  // ha szükség van AD láb digitális I/O lábra történö átkapcsolásra
  #ifdef InitAD
  InitAD();
  #endif

  LcdInit();                            // müködésre bírjuk az LCD-t
  init();

  //                     "01020304050607080910111213141516"
  memcpy((char*)LcdText, "   eeee.hh.nn       oo:pp:mm    ", 32);
  lastsec = 255;
  while(1)
  {
    // folyamatos frissítési üzemmód megszakítás nélkül
    #ifdef LCDMODECONT
    LcdProcess();
    #endif

    if(lastsec != masodperc)
    {
      // Ha nincs automata villogtatás 1sec a villogási idözítése
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

      itoa((char*)LcdText + 3, ev, 10);
      itoa((char*)LcdText + 8, honap, 10);
      itoa((char*)LcdText + 11, nap, 10);
      itoa((char*)LcdText + 20, ora, 10);
      itoa((char*)LcdText + 23, perc, 10);
      itoa((char*)LcdText + 26, masodperc, 10);

      // egyszámjegyü számok kétszámjegyüvé alakítása
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

      // számok közötti kettöspont villogtatás
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

      // karakterkészlet shiftelése és feltöltése (csak ha karakterkészlet változtatásának lehet?sége engedve van)
      #ifdef USERCHARSETCHANGE
      j = userchars[63];
      i = 63;
      while(i--)
        userchars[i + 1] = userchars[i];
      userchars[0] = j;
      LcdChangeCharset(userchars);
      #endif // USERCHARSETCHANGE

      // egyszeri frissítési üzemmódban megjelenítés
      #if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY) || (defined LCDMODEONCEIRQ)) 
      LcdRefreshAll();
      #endif
    }
  }
}

//******************************************************************************
// PIC24 megszakítás kiszolgálás
#if CLOCKTIMERNUM == 1
void __attribute__ ((interrupt, auto_psv)) _T1Interrupt(void)
{
  volatile static unsigned int msec = 0;
  IFS0bits.T1IF = 0;
  msec++;
  if(msec == 1000)
  {
    masodperc++;
    msec = 0; 
  }
  #if (defined LCDUSERTIMER)
  LcdIntProcess();                      // saját megszakításkezelövel és idözítéskezel?vel használva
  #endif
}
#else
void __attribute__ ((interrupt, auto_psv)) _T2Interrupt(void)
{
  volatile static unsigned int msec = 0;
  IFS0bits.T2IF = 0;
  msec++;
  if(msec == 1000)
  {
    masodperc++;
    msec = 0; 
  }
  #if (defined LCDUSERTIMER)
  LcdIntProcess();                      // saját megszakításkezelövel és idözítéskezelövel használva
  #endif
}
#endif
