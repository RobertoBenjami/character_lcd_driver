//------------------------------------------------------------------------------
// Készítö: Roberto Benjami (robertodebenjami(kukac)gmail(pont)com)
// Verzió:  v0.65 (2017.11)

//------------------------------------------------------------------------------
// Módosítások:
// 0.4 elsö közzétett verzió

// 0.41:
// - jav: Dupla vezérlö felhasználói karakterkészlet feltöltés javítása (SETDDRAMADDR -> SETCGRAMADDR).
// - opt: LcdWrite2()-ben felesleges RS-t újra beállítani, mikor LcdWrite()-ban már beállításra került.
// - opt: LcdWrite2()-ben felesleges az adatiránylábakat újra beállítani, mikor LcdWrite()-ban már beállításra került.

// 0.42:
// - opt: Veremmüveletek csökkentése miatt LcdWrite() paraméter nélkülivé alakítva.
//        (ch úgy is ott van globális változóként, RS láb beállítása, meg a hívás elött történik)
// - jav: Egysoros kijelzö esetén már egy sorosként is inicializál (LCD_LINES -> LCDLINES).

// 0.43
// - jav: C18 fordító "LCDWIDTH * LCDLINES + 1" kifejezést, ha > 127 rosszul értékeli ki -> 1UL szorzóval javítva
// - jav: C18 fordító "LCDWIDTH * LCDLINES + 7) / 8" kifejezést rosszul értékelheti ki -> 1UL szorzóval javítva

// 0.44
// - új:  Megszakításos módban lehetöség van a frissítést ki/bekapcsolni (pl. ha pontos idözítésre van szükség)
//        a LcdRefreshStart(), LcdRefreshStop() függvényhívással.
//        (ekkor az esetlegesen villogó karakterek is az aktuális állapotban maradnak)

// 0.45
// - új:  Lehetöség van a folyamatos mellett, egyszeri frissítési üzemmódra is.
//        Ekkor LcdRefreshAll() hívásával lehet a kijelzö tartalmát újraírni.
//        Megszakításos módban LcdRefreshed() visszatérö értékéböl megállapítható befejezte-e a frissítést.
//        (megszakítás nélküli módban a függvényben marad a frissítés végéig, így erre nincs szükség)
// - jav: AVR timer2: #ifdef TIMER0_COMP_vect -> #ifdef TIMER2_COMP_vect
// - új:  ATMEGA8 comparátor nélküli TIMER0-nak a használati lehetösége
// - jav: FPS->CPS timer számításához a DDRAM állítási ciklust is bekalkulálja
// - új:  LCDZEROCHANGETEXT definícióval (ez a charlcd.c -ben van kivételesen) az LcdText-ben is szóközre cseréli a #0 karaktereket.

// 0.46
// - új:  LCDSTEREO definíciót használva 2db kijelzöt is használhatunk.

// 0.47
// - új:  LCDCURSOR definíciót használva lehetöség van a kurzor használatára (csak egyszeri frissítési üzemmódban).

// 0.48
// - új:  USERCHARSETCHANGE definíciót használva lehetöség van futás alatt felhasználói karakterkészletet cserélni.

// 0.49
// - új:  HI-TECH C18 fordító hozzáadása
// - új:  Újabb üzemmód (megszakítás nélkül egyszeri frissítési üzemmódban BUSY flag figyelés helyett beállítható várakozással)
// - új:  Az 5 üzemmód között az elnevezésével lehet választani

// 0.50
// - új:  Összeférhetetlen beállítások kiszürése
// - új:  PIC18: választható prioritás nélki, alacsony, magas prioritás használata megszakításos módban
// - új:  Megszakítás módban saját megszakításkezeléssel megoldani az LCD frissítési eljárásának hivását
// - új:  TOUTPS - T2OUTPS fordítási hibaüzenet esetén könnyü átállás a másik verzióra
// - jav: Karakterkészlet nem 5x7, hanem 5x8 pixeles
// - jav: USERCHARSET és USERCHARSETCHANGE függetlenítése egymástól

// 0.51
// - jav: PIC18: TIME2DIV osztó javítása 2-es  oszási arány esetén
// - jav: T2OUTPS elnevezési eltérések miatti hibaüzenet megszüntetése direkt regiszter írással

// 0.52
// - opt: AVR saját karakterkészletet ne másolja át a ROM-ból a RAM-ba.
// - jav: AVR Timer0 LcdRefreshStart - Stop dupla definiálás megszüntetése
// - jav: PIC18: Timer1 inicializálásnál T1 számláló helyett T3-ba töltötte a kezdöértéket

// 0.60
// - új:  Lábhozzárendelések megadásának megváltoztatása (egy sorban a PORT és a láb, pl: #define LCDE B, 3)

// 0.61
// - jav: A 0.60-asban elrontott inicialási hiba javítása

// 0.62
// - opt: E láb magasra állítása elött nem szükséges várakozási idö (kivéve)
// - új:  LCDDTINm LCDDTOUT, LCDDT0TO3, LCDDT4TO7, LCDDT0TO7 optimalizálási lehetösége charlcd.h-ban
// - új:  #define LCDNOP ... a fordító optimalizálásának megtartása miatt

// 0.63
// - jav: PIC18 TimecCompless ASM -> C, hogy a fordító ne kapcsolja le az optimalizációt
// - jav: LCDRSPIN makro használat javitása GPIOX_CLRBIT(LCDRS) illetve GPIOX_SETBIT(LCDRS)-re
// - új:  #define LCDGLOBALINTAUTOSTART inicializáláskor bekapcsolja-e az INT-et
// - új:  karakterkészlet csere ROM területröl (#define USERCHARSETCHANGEROM)

// 0.64
// - új:  XC8 fordito felvétele
// - új:  automatikus optimalizálás, ha egymást követö lábakhoz rendeljük az adatlábakat

// 0.65
// - mod: LCDE2 definicio megadása jelenti a két kijelzö használatát, LCDSTEREO definicio számüzve
//        (a 80 karakteresnél nagyobb kijelzö is kettö kijelzöként müködik)
// - mod: LCDRW definicio megléte határozza meg, hogy az a láb használva lesz-e (LCDRWUSED számüzve)
// - mod: LCDDT0..LCDDT3 definicio megléte esetén automatikusan 8 bites modban fog müködni (LCD4BITMODE definicio megléte határozza meg, hogy az a láb használva lesz-e (LCDRWUSED számüzve)
// - jav: LcdRefreshStart: a timer alapállapotbol induljon, hogy az elsõ körben ne legyen rövidebb az idözités

//------------------------------------------------------------------------------
// Ha ez definiálva van, a frissítés során az Lcdtext[] tömbben is kicserélgeti a #0 karaktereket szóközre
// (általában nincs erre szükség, csak ha más célra is fel szeretnénk használni a szöveget)
// #define LCDZEROCHANGETEXT
//------------------------------------------------------------------------------
// Szoftver szimulátorhoz: kihagyja az LcdBusy függvényt (figyelem: engedélyezve a kijelzö nem fog müködni!)
// #define SOFTSIMBUSY
// Szoftver szimulátorhoz: inicializáláskor a várakozási ciklusok kihagyása (figyelem: engedélyezve a kijelzö nem fog müködni!)
// #define SOFTSIMFAST
//------------------------------------------------------------------------------

// processzorcsalád include
#if defined(__AVR__)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/ina90.h>
#elif (defined(_PIC14) || defined(_PIC14E))
#include <pic.h>
#elif defined(__XC__)
#include <xc.h>
#elif defined(__PICC18__)
#include <htc.h>
#elif defined(__18CXX)
#include <p18cxxx.h>
#elif defined(__C30__)
#if defined(__dsPIC30F__)
#include <p30Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__PIC24FK__)
#include <p24Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24Hxxxx.h>
#else
#error "ismeretlen processzorcsalád vagy forditoprogram"
#endif
#endif // defined(xxx proci)

#include "charlcd.h"

// ha LCDDT0..LCDDT3 létezik, akkor 8 bites mod
#if (defined(LCDDT0) && defined(LCDDT1) && defined(LCDDT2) && defined(LCDDT3))
#define LCD8BITMODE
#else
#define LCD4BITMODE
#endif

//------------------------------------------------------------------------------
// Összeférhetetlen beállítások kiszürése
#if !((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY) || (defined LCDMODEONCEIRQ) || (defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ)) 
#error "Valamelyik üzemmódot válaszd ki!"
#endif

#ifdef LCDMODEONCEBUSY
#if ((defined LCDMODEONCEDELAY) || (defined LCDMODEONCEIRQ) || (defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ)) 
#error "Csak 1 üzemmódot állíts be!"
#endif
#ifndef LCDRW
#error "LCD MODEONCEBUSY üzemmódban kötelezö használni az RW lábat!"
#endif
#ifdef LCDUSERTIMER
#error "LCD USERTIMER csak megszakításos módban használható"
#endif
#endif

#ifdef LCDMODEONCEDELAY
#if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEIRQ) || (defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ))
#error "Csak 1 üzemmódot állíts be!"
#endif
#ifndef LCDEXECUTIONTIME
#error "LCD MODEONCEDELAY üzemmódban LCDEXECUTIONTIME értékét is meg kell adni!"
#endif
#ifdef LCDUSERTIMER
#error "LCD USERTIMER csak megszakításos módban használható"
#endif
#endif

#ifdef LCDMODEONCEIRQ
#if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY) || (defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ))
#error "Csak 1 üzemmódot állíts be!"
#endif
#endif

#ifdef LCDMODECONTBUSY
#if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY) || (defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
#error "Csak 1 üzemmódot állíts be!"
#endif
#ifndef LCDRW
#error "LCD MODECONTBUSY üzemmódban kötelezö használni az RW lábat!"
#endif
#ifdef LCDCURSOR
#error "LCD MODECONTBUSY üzemmódban LCD CURSOR nem használható!"
#endif
#ifdef LCDUSERTIMER
#error "LCD USERTIMER csak megszakításos módban használható"
#endif
#endif

#ifdef LCDMODECONTIRQ
#if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY) || (defined LCDMODEONCEIRQ) || (defined LCDMODECONTBUSY))
#error "Csak 1 üzemmódot állíts be!"
#endif
#ifdef LCDCURSOR
#error "LCD MODECONTIRQ üzemmódban LCD CURSOR nem használható!"
#endif
#endif

#if (LCDLINES != 1) && (LCDLINES != 2) && (LCDLINES != 4)
#error "A driver csak 1, 2, 4 soros kijelzövel használható!"
#endif

#if (LCDWIDTH > 40)
#error "A driver maximum 40 karakter széles kijelzövel használható!"
#endif

//------------------------------------------------------------------------------
// rendszerórajel
#ifndef SystemClock
#error  "Nincs rendszerórajel frekvencia definiálva!"
#endif

// ha CPU órajel frekvencia nincs megadva akkor a rendszerórajellel megegyezik
#ifndef CpuClock
#define CpuClock SystemClock
#endif

// processzorcsalád szerinti órajel osztások
#if defined(__AVR__)
#define Cpu8bit
#define CpuMips (1UL*CpuClock)
#define DELAYNUM (CpuMips/1000/14)
#define LCDPROCCYCLENULL   109
#define LCDPROCCYCLENULLSTEREO   192
#define TimerSrcClock (1UL*SystemClock)
#elif (defined(_PIC14) || defined(_PIC14E))
#define Cpu8bit
#define CpuMips (1UL*CpuClock/4)
#define DELAYNUM (CpuMips/1000/10)
#define LCDPROCCYCLENULL   61
#define LCDPROCCYCLENULLSTEREO   100
#define TimerSrcClock (1UL*SystemClock/4)
#elif defined(__18CXX)
#define Cpu8bit
#define CpuMips (1UL*CpuClock/4)
#define DELAYNUM (CpuMips/1000/8)
#define LCDPROCCYCLENULL   67
#define LCDPROCCYCLENULLSTEREO   107
#define TimerSrcClock (1UL*SystemClock/4)
#elif (defined(__XC__) || defined(__PICC18__))
#define Cpu8bit
#define CpuMips (1UL*CpuClock/4)
#define DELAYNUM (CpuMips/1000/8)
#define LCDPROCCYCLENULL   61
#define LCDPROCCYCLENULLSTEREO   100
#define TimerSrcClock (1UL*SystemClock/4)
#elif defined(__C30__)
#if defined(__dsPIC30F__)
#define Cpu16bit
#define CpuMips (1UL*CpuClock/4)
#define DELAYNUM (CpuMips/1000/7)
#define LCDPROCCYCLENULL   90
#define LCDPROCCYCLENULLSTEREO   100
#define TimerSrcClock (1UL*SystemClock/4)
#elif defined(__dsPIC33F__)
#define Cpu16bit
#define CpuMips (1UL*CpuClock/2)
#define DELAYNUM (CpuMips/1000/7)
#define LCDPROCCYCLENULL   90
#define LCDPROCCYCLENULLSTEREO   159
#define TimerSrcClock (1UL*SystemClock/2)
#elif defined(__PIC24F__)
#define Cpu16bit
#define CpuMips (1UL*CpuClock/2)
#define DELAYNUM (CpuMips/1000/7)
#define LCDPROCCYCLENULL   90
#define LCDPROCCYCLENULLSTEREO   159
#define TimerSrcClock (1UL*SystemClock/2)
#elif defined(__PIC24FK__)
#define Cpu16bit
#define CpuMips (1UL*CpuClock/2)
#define DELAYNUM (CpuMips/1000/7)
#define LCDPROCCYCLENULL   90
#define LCDPROCCYCLENULLSTEREO   159
#define TimerSrcClock (1UL*SystemClock/2)
#elif defined(__PIC24H__)
#define Cpu16bit
#define CpuMips (1UL*CpuClock/2)
#define DELAYNUM (CpuMips/1000/7)
#define LCDPROCCYCLENULL   90
#define LCDPROCCYCLENULLSTEREO   159
#define TimerSrcClock (1UL*SystemClock/2)
#else
#error "ismeretlen processzortípus"
#endif
#endif // defined(xxx proci)

#define GPIOX_PORTTONUM_A     1
#define GPIOX_PORTTONUM_B     2
#define GPIOX_PORTTONUM_C     3
#define GPIOX_PORTTONUM_D     4
#define GPIOX_PORTTONUM_E     5
#define GPIOX_PORTTONUM_F     6
#define GPIOX_PORTTONUM_G     7
#define GPIOX_PORTTONUM_H     8
#define GPIOX_PORTTONUM_J     9
#define GPIOX_PORTTONUM_K     10
#define GPIOX_PORTTONUM_L     11
#define GPIOX_PORTTONUM_M     12
#define GPIOX_PORTNUM_(p, m)  GPIOX_PORTTONUM_ ## p
#define GPIOX_PORTNAME_(p, m) p
#define GPIOX_PIN_(p, m)      m

#if defined(__18CXX)
#define GPIOX_PORTNUM(x)      x(GPIOX_PORTNUM_)
#define GPIOX_PORTNAME(x)     x(GPIOX_PORTNAME_)
#define GPIOX_PIN(x)          x(GPIOX_PIN_)
#else
#define GPIOX_PORTNUM(x)      GPIOX_PORTNUM_(x)
#define GPIOX_PORTNAME(x)     GPIOX_PORTNAME_(x)
#define GPIOX_PIN(x)          GPIOX_PIN_(x)
#endif

#if defined(__AVR__)
#define GLUE(a, b)            a ## b
#define DDRBITSET(p, m)       GLUE(DDR, p) |= (1 << m)
#define DDRBITCLR(p, m)       GLUE(DDR, p) &= ~(1 << m)
#define PORTBITSET(p, m)      GLUE(PORT, p) |= (1 << m)
#define PORTBITCLR(p, m)      GLUE(PORT, p) &= ~(1 << m)
#define PINBIT(p, m)          GLUE(PIN, p) & (1 << m)
#define PORTX(p, m)           GLUE(PORT, p)
#define DDRX(p, m)            GLUE(DDR, p)

#define GPIOX_MODEPININ(x)    DDRBITCLR(x)
#define GPIOX_MODEPINOUT(x)   DDRBITSET(x)
#define GPIOX_SETBIT(x)       PORTBITSET(x)
#define GPIOX_CLRBIT(x)       PORTBITCLR(x)
#define GPIOX_GETBIT(x)       (PINBIT(x))
#define GPIOX_MODEPORT(x)     DDRX(x)
#define GPIOX_WRITEPORT(x)    PORTX(x)

#else // #if defined(__AVR__)

#define TRISBIT(p, m)         TRIS ## p ## bits.TRIS ## p ## m
#define LATBIT(p, m)          LAT ## p ## bits.LAT ## p ## m
#define PORTBIT(p, m)         PORT ## p ## bits.R ## p ## m
#define TRISX(p, m)           TRIS ## p
#define LATX(p, m)            LAT ## p
#define PORTX(p, m)           PORT ## p

// PIC18Fxxx (C18)
#if defined(__18CXX)
#define GPIOX_MODEPININ(x)    x(TRISBIT) = 1
#define GPIOX_MODEPINOUT(x)   x(TRISBIT) = 0
#define GPIOX_SETBIT(x)       x(LATBIT) = 1
#define GPIOX_CLRBIT(x)       x(LATBIT) = 0
#define GPIOX_GETBIT(x)       x(PORTBIT)
#define GPIOX_MODEPORT(x)     x(TRISX)
#define GPIOX_WRITEPORT(x)    x(LATX)
#ifdef  LCDNOP
#undef  NOP
#define NOP() LCDNOP
#endif
#endif  // #if defined(__18CXX)

// PIC16Fxx (HTC, XC)
#if (defined(_PIC14) || defined(_PIC14E))
#define GPIOX_MODEPININ(x)    TRISBIT(x) = 1
#define GPIOX_MODEPINOUT(x)   TRISBIT(x) = 0
#define GPIOX_SETBIT(x)       PORTBIT(x) = 1
#define GPIOX_CLRBIT(x)       PORTBIT(x) = 0
#define GPIOX_GETBIT(x)       PORTBIT(x)
#define GPIOX_MODEPORT(x)     TRISX(x)
#define GPIOX_WRITEPORT(x)    PORTX(x)
#endif  // #if (defined(_PIC14) || defined(_PIC14E))

// PIC16F1xxx, PIC18F, PIC24x, DSPIC (XC8, XC16, C30, HTC18)
#if (defined(__PICC18__) || defined(__C30__))
#define GPIOX_MODEPININ(x)    TRISBIT(x) = 1
#define GPIOX_MODEPINOUT(x)   TRISBIT(x) = 0
#define GPIOX_SETBIT(x)       LATBIT(x) = 1
#define GPIOX_CLRBIT(x)       LATBIT(x) = 0
#define GPIOX_GETBIT(x)       PORTBIT(x)
#define GPIOX_MODEPORT(x)     TRISX(x)
#define GPIOX_WRITEPORT(x)    LATX(x)
#endif  // #if (defined(__PICC18__) || defined(__C30__))

#endif // #else defined(__AVR__)

//------------------------------------------------------------------------------
// karakterenkénti frissítési frekvencia (csak interrupt módban)
// (ha 80 karakteresnél nagyobb, akkor a fele, mert egyszerre 2 karaktert frissít)
#if (1UL * LCDLINES * LCDWIDTH > 160)
#error "Maximum 2x80 karakteres lehet a kijelzö"
#endif

#if (1UL * LCDLINES * LCDWIDTH > 80)

#ifndef  LCDE2
#error  "80 karakteresnél nagyobb kijelzö esetén E2 lábat is meg kell adni!"
#endif

#if     LCDLINES != 4
#error  "80 karakteresnél nagyobb kijelzö, csak 4 sorosban létezik!"
#endif

// a 80 karakteresnél nagyobbat, 2db 2 sorosnak tekintjük
#undef  LCDLINES
#define LCDLINES        2
#endif  // #if (1UL * LCDLINES * LCDWIDTH > 80)

#define LCDCHARPERMODUL (1UL * LCDLINES * LCDWIDTH)
#define LCDCHARPERSEC   (1UL * LCDFRAMEPERSEC * (LCDLINES * LCDWIDTH + LCDLINES))

// LCD memóriacímek (1..4 sorok kezdete)
#define SETDDRAMADDR1  0x80
#define SETDDRAMADDR2  0xC0
#define SETDDRAMADDR3  (0x80 + LCDWIDTH)
#define SETDDRAMADDR4  (0xC0 + LCDWIDTH)

#define SETCGRAMADDR   0x40

// megszakítás üzemmódban a timer beállítások
#if ((!defined LCDUSERTIMER) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ)))

#include "charlcd-timers.h"

#endif // (!defined LCDUSERTIMER) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))

//==============================================================================
// általános változók

// az LcdWrite függvény használja átmeneti tárolásra (csak a felesleges veremmüveletek elkerülése miatt)
volatile union
{
  struct
  {
    unsigned bit0: 1;
    unsigned bit1: 1;
    unsigned bit2: 1;
    unsigned bit3: 1;
    unsigned bit4: 1;
    unsigned bit5: 1;
    unsigned bit6: 1;
    unsigned bit7: 1;
  };
  struct
  {
    unsigned nibble03: 4;
    unsigned nibble47: 4;
  };
  unsigned char chr;
}ch;

// ebben a tömbben tároljuk a megjelenítendö karaktereket
#ifdef LCDE2
volatile char LcdText[2UL * LCDWIDTH * LCDLINES + 1];
#else
volatile char LcdText[1UL * LCDWIDTH * LCDLINES + 1];
#endif
volatile unsigned char LcdPos;                   // Frissítésnél az aktuális karakter pozíciója

// LcdStatus lehetséges értékei:
// - HOME: DDRAM = 0 állítási fázis
// - LCHAR: karakterek kiírása fázis
// - DDR: DDRAM állítás fázis
// - CURTYPE: kurzor típus állítása fázis
// - CURPOS: kurzorpozíció beállítási fázis
// - CGR: CGRAM állítási fázis RAM-ból
// - CHARGEN: felhasználói karakterkészlet feltöltése RAM-ból fázis
// - CGRROM: CGRAM állítási fázis ROM-ból
// - CHARGENROM: felhasználói karakterkészlet feltöltése ROM-ból fázis
// - REFREND: frissítés befejezve
enum LS {HOME, LCHAR, DDR, CURTYPE, CURPOS, CGR, CHARGEN, CGRROM, CHARGENROM, REFREND} LcdStatus = LCHAR;

// A villogó karaktereket ebben a tömbben 1 értékü bit jelzi
#ifdef LCDBLINKCHAR
#ifdef LCDE2
volatile char LcdBlink[(2UL * LCDWIDTH * LCDLINES + 7) / 8];
#else  // LCDE2
volatile char LcdBlink[(1UL * LCDWIDTH * LCDLINES + 7) / 8];
#endif // LCDE2

// Villogási fázis tárolója
volatile unsigned char BlinkPhase = 0;  // 0 = villogó karakterek látszanak, 1 = nem látszik
#endif // LCDBLINKCHAR

// Kurzor pozíció és kurzor típus
#ifdef LCDCURSOR
volatile unsigned char LcdCursorPos = 0;
volatile unsigned char LcdCursorType;
#endif

// saját megszakításkezelövel és idözítéskezelövel használva a frissítési állapot ebben a változóban lesz
#if (defined LCDUSERTIMER) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
unsigned char LcdIrqStatus = 0;
#endif

// Felhasználói karakterkészlet RAM-ból
#ifdef USERCHARSETCHANGE
char*  uchp;
#endif

// Felhasználói karakterkészlet ROM-ból
#ifdef USERCHARSETCHANGEROM
rom char*  uchpr;
#endif

//==============================================================================
// C18 fordito a nem forditando területen is hibát jelez, ha nincs definiálva LCDDT0..3
#if defined(LCD4BITMODE) && defined(__18CXX)
#undef LCDDT0
#undef LCDDT1
#undef LCDDT2
#undef LCDDT3
#define LCDDT0 LCDDT4
#define LCDDT1 LCDDT5
#define LCDDT2 LCDDT6
#define LCDDT3 LCDDT7
#endif

#if ((GPIOX_PORTNUM(LCDDT4) == GPIOX_PORTNUM(LCDDT5))\
  && (GPIOX_PORTNUM(LCDDT5) == GPIOX_PORTNUM(LCDDT6))\
  && (GPIOX_PORTNUM(LCDDT6) == GPIOX_PORTNUM(LCDDT7)))
#if ((GPIOX_PIN(LCDDT4) + 1 == GPIOX_PIN(LCDDT5))\
  && (GPIOX_PIN(LCDDT5) + 1 == GPIOX_PIN(LCDDT6))\
  && (GPIOX_PIN(LCDDT6) + 1 == GPIOX_PIN(LCDDT7)))
// LCD adatlábai folyamotosan vannak beállitva (pl. B2,B3,B4,B5)
#ifdef LCD8BITMODE
#if ((GPIOX_PORTNUM(LCDDT0) == GPIOX_PORTNUM(LCDDT1))\
  && (GPIOX_PORTNUM(LCDDT1) == GPIOX_PORTNUM(LCDDT2))\
  && (GPIOX_PORTNUM(LCDDT2) == GPIOX_PORTNUM(LCDDT3))\
  && (GPIOX_PORTNUM(LCDDT3) == GPIOX_PORTNUM(LCDDT4)))
#if ((GPIOX_PIN(LCDDT0) + 1 == GPIOX_PIN(LCDDT1))\
  && (GPIOX_PIN(LCDDT1) + 1 == GPIOX_PIN(LCDDT2))\
  && (GPIOX_PIN(LCDDT2) + 1 == GPIOX_PIN(LCDDT3))\
  && (GPIOX_PIN(LCDDT3) + 1 == GPIOX_PIN(LCDDT4)))
// LCD adatlábai folyamotosan vannak beállitva (pl. B0,B1,B2,B3,B4,B5,B6,B7)
#define LCDDTAUTOOPT
#endif // D0..D4 portláb folytonosság ?
#endif // D0..D4 port azonosság ?
#else  // #ifdef LCD8BITMODE
// LCD adatlábai folyamotosan vannak beállitva (pl. B2,B3,B4,B5)
#define LCDDTAUTOOPT
#endif // #else LCD8BITMODE
#endif // D4..D7 portláb folytonosság ?
#endif // D4..D7 port azonosság ?

//------------------------------------------------------------------ 4 bites mod

#ifdef LCD4BITMODE

// LCD adatlábak LCD -> CPU (Lcd olvasása)
#ifndef LCDDTIN
#ifdef  LCDDTAUTOOPT
#if defined(__AVR__)
#define LCDDTIN GPIOX_MODEPORT(LCDDT4) &= ~(0x0F << GPIOX_PIN(LCDDT4))
#else
#define LCDDTIN GPIOX_MODEPORT(LCDDT4) |= (0x0F << GPIOX_PIN(LCDDT4))
#endif
#else
#define LCDDTIN GPIOX_MODEPININ(LCDDT4); GPIOX_MODEPININ(LCDDT5); GPIOX_MODEPININ(LCDDT6); GPIOX_MODEPININ(LCDDT7);
#endif
#endif

// LCD adatlábak CPU -> LCD (Lcd irása)
#ifndef LCDDTOUT
#ifdef  LCDDTAUTOOPT
#if defined(__AVR__)
#define LCDDTOUT GPIOX_MODEPORT(LCDDT4) |= (0x0F << GPIOX_PIN(LCDDT4))
#else
#define LCDDTOUT GPIOX_MODEPORT(LCDDT4) &= ~(0x0F << GPIOX_PIN(LCDDT4))
#endif
#else
#define LCDDTOUT GPIOX_MODEPINOUT(LCDDT4); GPIOX_MODEPINOUT(LCDDT5); GPIOX_MODEPINOUT(LCDDT6); GPIOX_MODEPINOUT(LCDDT7);
#endif
#endif

// LCD adatlábakra 4..7 bit kirakása (4 bites módban)
#ifndef LCDDT4TO7
#ifdef  LCDDTAUTOOPT
#define LCDDT4TO7 GPIOX_WRITEPORT(LCDDT4) = (GPIOX_WRITEPORT(LCDDT4) & ~(0b00001111 << GPIOX_PIN(LCDDT4)))\
        | (((ch.chr >> 4) & 0b00001111) << GPIOX_PIN(LCDDT4))
#else  
#define LCDDT4TO7 \
  if(!ch.bit4) GPIOX_CLRBIT(LCDDT4); if(ch.bit4) GPIOX_SETBIT(LCDDT4);\
  if(!ch.bit5) GPIOX_CLRBIT(LCDDT5); if(ch.bit5) GPIOX_SETBIT(LCDDT5);\
  if(!ch.bit6) GPIOX_CLRBIT(LCDDT6); if(ch.bit6) GPIOX_SETBIT(LCDDT6);\
  if(!ch.bit7) GPIOX_CLRBIT(LCDDT7); if(ch.bit7) GPIOX_SETBIT(LCDDT7);
#endif  // else LCDDTAUTOOPT
#endif

// LCD adatlábakra 0..3 bit kirakása (4 bites módban)
#ifndef LCDDT0TO3
#ifdef  LCDDTAUTOOPT
#define LCDDT0TO3 GPIOX_WRITEPORT(LCDDT4) = (GPIOX_WRITEPORT(LCDDT4) & ~(0b00001111 << GPIOX_PIN(LCDDT4)))\
        | ((ch.chr & 0b00001111) << GPIOX_PIN(LCDDT4))
#else  // LCDDTAUTOOPT 
#define LCDDT0TO3 \
  if(!ch.bit0) GPIOX_CLRBIT(LCDDT4); if(ch.bit0) GPIOX_SETBIT(LCDDT4);\
  if(!ch.bit1) GPIOX_CLRBIT(LCDDT5); if(ch.bit1) GPIOX_SETBIT(LCDDT5);\
  if(!ch.bit2) GPIOX_CLRBIT(LCDDT6); if(ch.bit2) GPIOX_SETBIT(LCDDT6);\
  if(!ch.bit3) GPIOX_CLRBIT(LCDDT7); if(ch.bit3) GPIOX_SETBIT(LCDDT7);
#endif // else LCDDTAUTOOPT
#endif // ifndef LCDDT0TO3
#endif // #ifdef LCD4BITMODE

//------------------------------------------------------------------ 8 bites mod
#ifdef  LCD8BITMODE
// LCD adatlábak LCD -> CPU (Lcd olvasása)
#ifndef LCDDTIN
#ifdef  LCDDTAUTOOPT
#if defined(__AVR__)
#define LCDDTIN GPIOX_MODEPORT(LCDDT0) = 0b00000000
#else
#ifdef  Cpu8bit
#define LCDDTIN GPIOX_MODEPORT(LCDDT0) = 0b11111111
#else
#define LCDDTIN GPIOX_MODEPORT(LCDDT0) |= (0b11111111 << GPIOX_PIN(LCDDT0))
#endif
#endif
#else
#define LCDDTIN {GPIOX_MODEPININ(LCDDT0); GPIOX_MODEPININ(LCDDT1); GPIOX_MODEPININ(LCDDT2); GPIOX_MODEPININ(LCDDT3);\
                 GPIOX_MODEPININ(LCDDT4); GPIOX_MODEPININ(LCDDT5); GPIOX_MODEPININ(LCDDT6); GPIOX_MODEPININ(LCDDT7);}
#endif
#endif

// LCD adatlábak CPU -> LCD (Lcd irása)
#ifndef LCDDTOUT
#ifdef  LCDDTAUTOOPT
#if defined(__AVR__)
#define LCDDTOUT GPIOX_MODEPORT(LCDDT0) = 0b11111111
#else
#ifdef  Cpu8bit
#define LCDDTOUT GPIOX_MODEPORT(LCDDT0) = 0b00000000
#else
#define LCDDTOUT GPIOX_MODEPORT(LCDDT0) &= ~(0b11111111 << GPIOX_PIN(LCDDT0))
#endif
#endif
#else
#define LCDDTOUT {GPIOX_MODEPINOUT(LCDDT0); GPIOX_MODEPINOUT(LCDDT1); GPIOX_MODEPINOUT(LCDDT2); GPIOX_MODEPINOUT(LCDDT3);\
                  GPIOX_MODEPINOUT(LCDDT4); GPIOX_MODEPINOUT(LCDDT5); GPIOX_MODEPINOUT(LCDDT6); GPIOX_MODEPINOUT(LCDDT7);}
#endif
#endif

  // LCD adatlábakra 0..3 bit kirakása (8 bites módban)
#ifndef LCDDT0TO7
#ifdef  LCDDTAUTOOPT
#ifdef  Cpu8bit
#define LCDDT0TO7 GPIOX_WRITEPORT(LCDDT0) = ch.chr
#else
#define LCDDT0TO7 GPIOX_WRITEPORT(LCDDT0) = (GPIOX_WRITEPORT(LCDDT0) & ~(0b11111111 << GPIOX_PIN(LCDDT0)))\
        | (ch.chr << GPIOX_PIN(LCDDT0))
#endif
#else
#define LCDDT0TO7 \
  if(!ch.bit0) GPIOX_CLRBIT(LCDDT0); if(ch.bit0) GPIOX_SETBIT(LCDDT0);\
  if(!ch.bit1) GPIOX_CLRBIT(LCDDT1); if(ch.bit1) GPIOX_SETBIT(LCDDT1);\
  if(!ch.bit2) GPIOX_CLRBIT(LCDDT2); if(ch.bit2) GPIOX_SETBIT(LCDDT2);\
  if(!ch.bit3) GPIOX_CLRBIT(LCDDT3); if(ch.bit3) GPIOX_SETBIT(LCDDT3);\
  if(!ch.bit4) GPIOX_CLRBIT(LCDDT4); if(ch.bit4) GPIOX_SETBIT(LCDDT4);\
  if(!ch.bit5) GPIOX_CLRBIT(LCDDT5); if(ch.bit5) GPIOX_SETBIT(LCDDT5);\
  if(!ch.bit6) GPIOX_CLRBIT(LCDDT6); if(ch.bit6) GPIOX_SETBIT(LCDDT6);\
  if(!ch.bit7) GPIOX_CLRBIT(LCDDT7); if(ch.bit7) GPIOX_SETBIT(LCDDT7);
#endif
#endif
#endif // else LCD4BITMODE

//==============================================================================
#ifdef SOFTSIMFAST
#define EDELAY   ;
#define EDELAYCYCLE 0
#else  // SOFTSIMFAST


// E láb tartási idö
// AVR
#if defined(__AVR__)
#if   CpuMips >= 30000000
#define EDELAY  _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP()
#define EDELAYCYCLE 12
#elif CpuMips >= 20000000
#define EDELAY  _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP()
#define EDELAYCYCLE 8
#elif CpuMips >= 10000000
#define EDELAY  _NOP(); _NOP(); _NOP(); _NOP(); _NOP()
#define EDELAYCYCLE 4
#elif CpuMips >= 7500000
#define EDELAY  _NOP(); _NOP(); _NOP()
#define EDELAYCYCLE 3
#elif CpuMips >= 5000000
#define EDELAY  _NOP(); _NOP()
#define EDELAYCYCLE 2
#else
#define EDELAY  _NOP()
#define EDELAYCYCLE 1
#endif // CpuMips
//------------------------------------------------------------------------------
// PIC16, PIC18
#elif (defined(_PIC14) || defined(_PIC14E) || defined(__18CXX) || defined(__PICC18__))
#if   CpuMips >= 10000000
#define EDELAY  NOP(); NOP(); NOP(); NOP()
#define EDELAYCYCLE 4
#elif CpuMips >= 7500000
#define EDELAY  NOP(); NOP(); NOP()
#define EDELAYCYCLE 3
#elif CpuMips >= 5000000
#define EDELAY  NOP(); NOP()
#define EDELAYCYCLE 2
#else
#define EDELAY  NOP()
#define EDELAYCYCLE 1
#endif // CpuMips
//------------------------------------------------------------------------------
// PIC24
#elif defined(__C30__)
#if   CpuMips >= 40000000
#define EDELAY  Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop()
#define EDELAYCYCLE 16
#elif CpuMips >= 30000000
#define EDELAY  Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop()
#define EDELAYCYCLE 12
#elif CpuMips >= 20000000
#define EDELAY  Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop()
#define EDELAYCYCLE 8
#elif CpuMips >= 10000000
#define EDELAY  Nop(); Nop(); Nop(); Nop()
#define EDELAYCYCLE 4
#elif CpuMips >= 7500000
#define EDELAY  Nop(); Nop(); Nop()
#define EDELAYCYCLE 3
#elif CpuMips >= 5000000
#define EDELAYCYCLE 2
#define EDELAY  Nop(); Nop()
#else
#define EDELAY  Nop()
#define EDELAYCYCLE 1
#endif // CpuMips
#endif // else defined(__AVR__)

#endif // else SOFTSIMFAST

//==============================================================================
// Processzorfüggetlen részek
//==============================================================================

//==============================================================================
// DelayMs
// - 1..255msec várakozás (nem teljesen pontos de az LCD inicializáláshoz elegendö)
//==============================================================================
void DelayMs(unsigned char ms)
{
  volatile unsigned int dl;

  #ifndef SOFTSIMFAST
  while(ms--)
  {
    dl = DELAYNUM;
    while(dl--);
  }
  #endif
}

#ifdef LCDMODEONCEDELAY
//==============================================================================
// DelayLcd
// - Lcd írás várakozás várakozás (nem teljesen pontos)
//==============================================================================
// LCD írások között ennyi utasításciklusnak kell legalább lenni:
#define LCDEXECUTIONCYCLE ((CpuMips * LCDEXECUTIONTIME) / 1000000)

// ennyi a valós végrehajtási ideje az LCD író függvénynek
#ifdef LCDE2
#ifdef LCD4BITMODE
#define LCDPROCCYCLE (LCDPROCCYCLENULLSTEREO + 8 * EDELAYCYCLE + 4)
#else  // LCD4BITMODE
#define LCDPROCCYCLE (LCDPROCCYCLENULLSTEREO + 4 * EDELAYCYCLE)
#endif // else LCD4BITMODE
#else  // LCDE2
#ifdef LCD4BITMODE
#define LCDPROCCYCLE (LCDPROCCYCLENULL + 4 * EDELAYCYCLE + 2)
#else  // LCD4BITMODE
#define LCDPROCCYCLE (LCDPROCCYCLENULL + 2 * EDELAYCYCLE)
#endif // else LCD4BITMODE
#endif // else LCDE2

// ha LcdProc végrehajtási ideje >= LCDEXECUTIONTIME -> nincs szükség késleltetö ciklusra
#if LCDPROCCYCLE >= LCDEXECUTIONCYCLE
#define DelayLcd() ;
#else  // LCDPROCCYCLE >= LCDEXECUTIONCYCLE

// ennyit kell még várakozni az LCDEXECUTIONTIME-hoz
#define LCDDELAYCYCLE (LCDEXECUTIONCYCLE - LCDPROCCYCLE)

void DelayLcd(void)
{
  #if defined(__AVR__)
  static volatile unsigned char cl;
  #if (LCDDELAYCYCLE / 10) > 255
  #error "LCD EXECUTIONTIME túl nagy!"
  #else
  cl = LCDDELAYCYCLE / 10;
  #endif
  #elif defined(__C30__)
  static volatile unsigned int cl;
  #if (LCDDELAYCYCLE / 4) > 65535
  #error "LCD EXECUTIONTIME túl nagy!"
  #else
  cl = LCDDELAYCYCLE / 4;
  #endif
  #else // defined(__C30__)
  volatile unsigned char cl;
  #if (LCDDELAYCYCLE / 3) > 255
  #error "LCD EXECUTIONTIME túl nagy!"
  #else
  cl = LCDDELAYCYCLE / 3;
  #endif
  #endif // else defined(__C30__)
  while(--cl);
}

#endif // else LCDPROCCYCLE >= LCDEXECUTIONCYCLE

#endif // LCDMODEONCEDELAY

//==============================================================================
// LcdBusy (csak LCDMODEONCEBUSY módban)
// - LCD vezérlö foglaltságának ellenörzése
//   (nem várja meg míg szabad lesz)
// visszatérö érték:
//   1 ha a Busy flag is be van állítva (foglalt)
//   0 ha szabad (lehet adatot küldeni)
//==============================================================================
#if (defined LCDMODEONCEBUSY) || (defined LCDMODECONTBUSY)
char LcdBusy(void)
{
  LCDDTIN;                              // adatlábak bemenetek
  GPIOX_SETBIT(LCDRW);                  // adatirány: LCD -> mikrovezérlö (read)
  GPIOX_CLRBIT(LCDRS);                  // RS = 0
  EDELAY;
  GPIOX_SETBIT(LCDE);
  EDELAY;

  if(GPIOX_GETBIT(LCDDT7))              // busy flag
  {
    GPIOX_CLRBIT(LCDE);
    #ifdef LCD4BITMODE
    EDELAY;
    GPIOX_SETBIT(LCDE); EDELAY; GPIOX_CLRBIT(LCDE);
    EDELAY;
    #endif // LCD4BITMODE
    return 1;                           // foglalt
  }
  GPIOX_CLRBIT(LCDE);
  #ifdef LCD4BITMODE
  EDELAY;
  GPIOX_SETBIT(LCDE); EDELAY; GPIOX_CLRBIT(LCDE);
  #endif // LCD4BITMODE

  #ifdef LCDE2
  EDELAY;
  GPIOX_SETBIT(LCDE2);
  EDELAY;
  if(GPIOX_GETBIT(LCDDT7))              // busy flag
  {
    GPIOX_CLRBIT(LCDE2);
    #ifdef LCD4BITMODE
    EDELAY;
    GPIOX_SETBIT(LCDE2); EDELAY; GPIOX_CLRBIT(LCDE2);
    #endif // LCD4BITMODE
    return 1;                           // foglalt
  }
  LCDE2PIN0;
  #ifdef LCD4BITMODE
  EDELAY;
  GPIOX_SETBIT(LCDE2); EDELAY; GPIOX_CLRBIT(LCDE2);
  EDELAY;
  #endif // LCD4BITMODE
  #endif // LCDE2

  return 0;                             // szabad
}
#endif // (defined LCDMODEONCEBUSY) || (defined LCDMODECONTBUSY)

//==============================================================================
// LcdWrite (I/O lábakon keresztül egy karaktert ír az LCD-re)
// - RS láb = 0: config regiszter (parancs)
// - RS láb = 1: RAM (karakter)
// - ch globális báltozó = adat
//==============================================================================
void LcdWrite(void)
{
  // ha nem használunk BUSY módot, mindig kimenet marad az összes adatláb, ezért nem kell állítgatni
  #if (defined LCDMODEONCEBUSY) || (defined LCDMODECONTBUSY)
  GPIOX_CLRBIT(LCDRW);                  // adatirány: mikrovezérlö -> LCD
  LCDDTOUT;
  #endif // !((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))

  #ifdef LCD4BITMODE
  LCDDT4TO7;
  GPIOX_SETBIT(LCDE); EDELAY; GPIOX_CLRBIT(LCDE);
  LCDDT0TO3;
  #else
  LCDDT0TO7;
  #endif
  GPIOX_SETBIT(LCDE); EDELAY; GPIOX_CLRBIT(LCDE);
}

//==============================================================================
#ifdef LCDE2
void LcdWrite2(void)
{
  #ifdef LCD4BITMODE
  LCDDT4TO7;
  GPIOX_SETBIT(LCDE2); EDELAY; GPIOX_CLRBIT(LCDE2);
  LCDDT0TO3;
  #else
  LCDDT0TO7;
  #endif
  GPIOX_SETBIT(LCDE2); EDELAY; GPIOX_CLRBIT(LCDE2);
}
#else
#define LcdWrite2() ;
#endif // #else LCDE2

//------------------------------------------------------------------------------
#ifdef USERCHARSET

#if defined(__AVR__)
#define userromcharset(n) pgm_read_dword_near(&userromcharset[n])
const USERCHARSETARRAY userromcharset PROGMEM =
#elif defined(__18CXX)
rom USERCHARSETARRAY userromcharset = 
#else
const USERCHARSETARRAY userromcharset = 
#endif
{
  USER0CHAR0, USER0CHAR1, USER0CHAR2, USER0CHAR3, USER0CHAR4, USER0CHAR5, USER0CHAR6, USER0CHAR7,
  USER1CHAR0, USER1CHAR1, USER1CHAR2, USER1CHAR3, USER1CHAR4, USER1CHAR5, USER1CHAR6, USER1CHAR7,
  USER2CHAR0, USER2CHAR1, USER2CHAR2, USER2CHAR3, USER2CHAR4, USER2CHAR5, USER2CHAR6, USER2CHAR7,
  USER3CHAR0, USER3CHAR1, USER3CHAR2, USER3CHAR3, USER3CHAR4, USER3CHAR5, USER3CHAR6, USER3CHAR7,
  USER4CHAR0, USER4CHAR1, USER4CHAR2, USER4CHAR3, USER4CHAR4, USER4CHAR5, USER4CHAR6, USER4CHAR7,
  USER5CHAR0, USER5CHAR1, USER5CHAR2, USER5CHAR3, USER5CHAR4, USER5CHAR5, USER5CHAR6, USER5CHAR7,
  USER6CHAR0, USER6CHAR1, USER6CHAR2, USER6CHAR3, USER6CHAR4, USER6CHAR5, USER6CHAR6, USER6CHAR7,
  USER7CHAR0, USER7CHAR1, USER7CHAR2, USER7CHAR3, USER7CHAR4, USER7CHAR5, USER7CHAR6, USER7CHAR7
};
#endif // USERCHARSET

#ifdef LCDE2
#define  EIMPULSE  {GPIOX_SETBIT(LCDE); GPIOX_SETBIT(LCDE2); EDELAY; GPIOX_CLRBIT(LCDE); GPIOX_CLRBIT(LCDE2); }
#else
#define  EIMPULSE  {GPIOX_SETBIT(LCDE); EDELAY; GPIOX_CLRBIT(LCDE);}
#endif

//==============================================================================
// LcdInit
// - I/O lábak, TRIS regiszterek beállítása 
// - LCD kijelzö bekapcsolása és alapállapotba állítása
//==============================================================================
void LcdInit(void)
{
  unsigned char i;
  // I/O lábak beállítása
  GPIOX_CLRBIT(LCDE);         // E = 0
  GPIOX_MODEPINOUT(LCDE);     // E = kimenet

  #ifdef LCDE2
  GPIOX_CLRBIT(LCDE2);        // E2 = 0
  GPIOX_MODEPINOUT(LCDE2);    // E2 = kimenet
  #endif

  GPIOX_CLRBIT(LCDRS);        // RS = 0
  GPIOX_MODEPINOUT(LCDRS);    // RS = kimenet

  // ha R/W lábat is használjuk
  #ifdef LCDRW
  GPIOX_CLRBIT(LCDRW);        // RW = 0
  GPIOX_MODEPINOUT(LCDRW);    // RW = kimenet
  #endif // LCDRW

  #ifdef LCD4BITMODE
  ch.chr = 0b00000000;
  LCDDT4TO7;
  LCDDTOUT;
  DelayMs(100);
  ch.chr = 0b00110000;
  LCDDT4TO7;
  EIMPULSE;                             // 0011
  DelayMs(5);
  EIMPULSE;                             // 0011
  DelayMs(5);
  EIMPULSE;                             // 0011
  DelayMs(5);
  ch.chr = 0b00100000;
  LCDDT4TO7;
  EIMPULSE;                             // 0010
  DelayMs(5);
  #if  LCDLINES > 1
  ch.chr = 0b00101000;                  // 2 soros esetén 00101000
  #endif
  LcdWrite();
  LcdWrite2();

  #else  // #ifdef LCD4BITMODE
  ch.chr = 0b00000000;
  LCDDT0TO7;
  LCDDTOUT;
  DelayMs(100);
  ch.chr = 0b00110000;
  LCDDT0TO7;
  EIMPULSE;
  DelayMs(5);
  EIMPULSE;
  DelayMs(5);
  EIMPULSE;
  DelayMs(5);
  #if  LCDLINES > 1
  ch.chr = 0b00111000;                  // 2 soros esetén 00111000
  #endif
  LcdWrite();
  LcdWrite2();
  #endif // #else LCD4BITMODE

  DelayMs(2);
  ch.chr = 0b00001100;                  // kijelzö bekapcs, kurzor ki, kurzor villogás ki
  LcdWrite();
  LcdWrite2();
  
  DelayMs(2);
  ch.chr = 0b00000001;                  // display off
  LcdWrite();
  LcdWrite2();

  DelayMs(2);
  ch.chr = 0b00000110;                  // 
  LcdWrite();
  LcdWrite2();
  DelayMs(2);

  #ifdef USERCHARSET
  DelayMs(2);
  ch.chr = SETCGRAMADDR;                // CGRAM = 0
  LcdWrite();
  LcdWrite2();

  GPIOX_SETBIT(LCDRS);
  for(i = 0; i < 64; i++)
  {
    DelayMs(2);
    #if defined(__AVR__)
    ch.chr = userromcharset(i);
    #else
    ch.chr = userromcharset[i];
    #endif
    LcdWrite();
    LcdWrite2();
  }

  DelayMs(2);
  GPIOX_CLRBIT(LCDRS);
  ch.chr = SETDDRAMADDR1;               // DDRAM = elsö sor eleje
  LcdWrite();
  LcdWrite2();
  #endif // USERCHARSET
  GPIOX_SETBIT(LCDRS);

  // szöveg törlése
  #ifdef LCDE2
  for(i = 0; i < (2UL * LCDWIDTH * LCDLINES); i++) LcdText[i] = ' ';
  #else
  for(i = 0; i < (1UL * LCDWIDTH * LCDLINES); i++) LcdText[i] = ' ';
  #endif

  // villogás tömb törlése
  #ifdef LCDBLINKCHAR
  #ifdef LCDE2
  for(i = 0; i < ((2UL * LCDWIDTH * LCDLINES + 7) / 8); i++) LcdBlink[i] = 0;
  #else  // LCDE2
  for(i = 0; i < ((1UL * LCDWIDTH * LCDLINES + 7) / 8); i++) LcdBlink[i] = 0;
  #endif // LCDE2
  #endif // LCDBLINKCHAR

  LcdPos = 0;

  #if (!defined LCDUSERTIMER) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
  TIMERINIT;
  #ifdef LCDMODECONTIRQ
  LcdRefreshStart();
  #endif // LCDMODECONTIRQ
  #endif // (!defined LCDUSERTIMER) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
}


//==============================================================================
// Blinker (villogtató, csak megszakításos folyamatos frissítési üzemmódban)
#if (defined LCDMODECONTIRQ) && (defined LCDBLINKCHAR) && (LCDBLINKSPEED > 0)
#define BLINKER() {             \
  if(!BlinkTimer--)             \
  {                             \
    BlinkPhase = !BlinkPhase;   \
    BlinkTimer = LCDBLINKSPEED; \
  }                             \
}
#define AUTOBLINKER
#else  // (defined LCDMODECONTIRQ) && (defined LCDBLINKCHAR) && (LCDBLINKSPEED > 0)
#define BLINKER() ;
#endif // else (defined LCDMODECONTIRQ) && (defined LCDBLINKCHAR) && (LCDBLINKSPEED > 0)

//==============================================================================
// LcdProcess (egy karaktert frissít a kijelzön)
// Elöfeltétel: LcdInit() inicializálni kell
// Input:           LcdText[], villogó módban LcdBlink[]
// Áttekintés:      Ha szabad az LCD, egy karaktert átmásol az LcdText[] tömbböl 
//                  az LCD kijelzöre 
// Megjegyzés:      Nincs várokozási idö, ezért USB-s alkalmazásnál ezzel frissíthetö 
//                  a kijelzö.
//==============================================================================
#ifdef TIMERINTPROCESS
TIMERINTPROCESS
#else  // TIMERINTPROCESS
void LcdProcess(void)
#endif // else TIMERINTPROCESS
{
  // Automatikus blinker frame számlálója a villogás ütemezéséhez
  #ifdef AUTOBLINKER
  static char BlinkTimer = LCDBLINKSPEED;
  #endif

//------------------------------------------------------------------------------
  // megszakítás mód specialitásai
  #if (!defined LCDUSERTIMER) && ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))
  // komparátor nélküli idözítö átállítása
  TIMERCOMPLESS;
  // megszakítás nyugtázása
  TIMERIRQACK;
  #endif

//------------------------------------------------------------------------------
  // LCD foglaltságfigyeléses üzemmód
  #if (((defined LCDMODEONCEBUSY) || (defined LCDMODECONTBUSY)) && (!defined SOFTSIMBUSY))
  if(LcdBusy()) return;                 // LCD foglalt ?
  #endif

  //----------------------------------------------------------------------------
  if(LcdStatus == LCHAR)
  {
    GPIOX_SETBIT(LCDRS);                // karakter megy majd a kijelzöre
    // ----------------------------------- egy karakter fissítés
    ch.chr = LcdText[LcdPos];
    
    #ifdef LCDZEROCHANGE
    if (ch.chr == 0)
    {
      ch.chr = ' ';                     // #0 kódú karakter -> szóköz karakter
      #ifdef LCDZEROCHANGETEXT
      LcdText[LcdPos] = ' ';            // LcdText-ben is #0 -> 'SPACE' csere
      #endif
    }
    #endif // LCDZEROCHANGE

    #ifdef LCDBLINKCHAR
    if((BlinkPhase) && (LcdBlink[LcdPos >> 3] & (1 << (LcdPos & 7))))
      ch.chr = ' ';                     // ha villogási fázis == 1, és az adott karakter is villogtatva van -> szóköz
    #endif // LCDBLINKCHAR

    LcdWrite();

    // ----------------------------------- stereo kijelzö esetén, a másik is rögtön írható
    #ifdef LCDE2

    ch.chr = LcdText[LcdPos + LCDLINES * LCDWIDTH];
    #ifdef LCDZEROCHANGE
    if (ch.chr == 0)
    {
      ch.chr = ' ';
      #ifdef LCDZEROCHANGETEXT
      LcdText[LcdPos + LCDLINES * LCDWIDTH] = ' '; // LcdText-ben is #0 -> 'SPACE' csere
      #endif
    }
    #endif // LCDZEROCHANGE

    #ifdef LCDBLINKCHAR
    if((BlinkPhase) && (LcdBlink[(LcdPos + LCDLINES * LCDWIDTH) >> 3] & (1 << ((LcdPos + LCDLINES * LCDWIDTH) & 7))))
      ch.chr = ' ';
    #endif // LCDBLINKCHAR

    LcdWrite2();
    #endif // LCDE2
    //------------------------------------

    LcdPos++;

    //------------------------------------ 1 soros
    #if LCDLINES == 1
    if(LcdPos == LCDWIDTH)              // 1 soros 1.sor vége?
    {
      #ifdef LCDCURSOR
      LcdStatus = CURPOS;               // kurzor pozíció beállítás
      #else // LCDCURSOR
      #ifdef LCDMODEONCEIRQ
      LcdRefreshStop();                 // kész az LCD frissítése, leállítható
      #else  // LCDMODEONCEIRQ
      LcdStatus = REFREND;              // kész az LCD frissítése, LcdRefresh while ciklusa befejezödhet
      #endif // else LCDMODEONCEIRQ
      #endif // LCDCURSOR

      #if ((defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ))
      LcdStatus = DDR;                  // folyamatos frissítési üzemmódban DDRAM állítás lesz majd
      #endif
    }
    //------------------------------------ 2 soros
    #elif LCDLINES == 2
    if(LcdPos == LCDWIDTH)              // 2 soros, 1.sor vége?
    {
      LcdStatus = DDR;                  // DDRAM állítás lesz majd
    }
    else if(LcdPos == LCDWIDTH * 2)     // 2 soros, 2.sor vége?
    {
      #ifdef LCDCURSOR
      LcdStatus = CURPOS;               // kurzor pozíció beállítás
      #else // LCDCURSOR
      #ifdef LCDMODEONCEIRQ
      LcdRefreshStop();                 // kész az LCD frissítése, leállítható
      #else  // LCDMODEONCEIRQ
      LcdStatus = REFREND;              // kész az LCD frissítése, LcdRefresh while ciklusa befejezödhet
      #endif // else LCDMODEONCEIRQ
      #endif // LCDCURSOR

      #if ((defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ))
      LcdStatus = DDR;                  // folyamatos frissítési üzemmódban DDRAM állítás lesz majd
      #endif
    }
    //------------------------------------ 4 soros
    #elif LCDLINES == 4
 
    //------------------------------------ 4 soros szimpla vezérlös
    if(LcdPos == LCDWIDTH)              // 4 soros, 1.sor vége?
    {
      LcdStatus = DDR;                  // DDRAM állítás lesz majd
    }
    else if(LcdPos == LCDWIDTH * 2)     // 4 soros, 2.sor vége?
    {
      LcdStatus = DDR;                  // DDRAM állítás lesz majd
    }
    else if(LcdPos == LCDWIDTH * 3)     // 4 soros, 3.sor vége?
    {
      LcdStatus = DDR;                  // DDRAM állítás lesz majd
    }
    else if(LcdPos == LCDWIDTH * 4)     // 4 soros, 4.sor vége?
    {
      #ifdef LCDCURSOR
      LcdStatus = CURPOS;               // kurzor pozíció beállítás
      #else // LCDCURSOR
      #ifdef LCDMODEONCEIRQ
      LcdRefreshStop();                 // kész az LCD frissítése, leállítható
      #else  // LCDMODEONCEIRQ
      LcdStatus = REFREND;              // kész az LCD frissítése, LcdRefresh while ciklusa befejezödhet
      #endif // else LCDMODEONCEIRQ
      #endif // LCDCURSOR

      #if ((defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ))
      LcdStatus = DDR;                  // folyamatos frissítési üzemmódban DDRAM állítás lesz majd
      #endif
    }
    #endif // LCDLINES

  } // LcdStatus

  //----------------------------------------------------------------------------
  else if(LcdStatus == DDR)             // DDRAM állítgatás
  {
    // ----------------------------------- DDRAM állítgatás (sorok végein), villogtatási ütemezés
    GPIOX_CLRBIT(LCDRS);                // parancs megy majd a kijelzöre (RS = 0)

    // ----------------------------------- 1 soros kijelzö
    #if LCDLINES == 1
    ch.chr = SETDDRAMADDR1;             // 1 soros, 1.sor vége -> DDRAM = 1.sor eleje
    LcdWrite();
    LcdWrite2();
    LcdPos = 0;
    BLINKER();

    // ----------------------------------- 2 soros kijelzö
    #elif LCDLINES == 2
    if(LcdPos == LCDWIDTH)
    {                                   // 2 soros, 1.sor vége
      ch.chr = SETDDRAMADDR2;           // DDRAM = 2.sor eleje
      LcdWrite();
      LcdWrite2();
    }
    else
    {                                   // 2 soros, 2.sor vége
    ch.chr = SETDDRAMADDR1;             // DDRAM = 1.sor eleje
      LcdWrite();
      LcdWrite2();
      LcdPos = 0;
      BLINKER();
    }

    // ----------------------------------- 4 soros kijelzö
    #elif LCDLINES == 4

    // 4 soros szimpla vezérlös (max 80 karakteres)
    if(LcdPos == LCDWIDTH)              // 1. sor vége ?
    { 
      ch.chr = SETDDRAMADDR2;           // DDRAM = 2.sor eleje
      LcdWrite();
      LcdWrite2();
    }
    else if(LcdPos == (LCDWIDTH * 2))   // 2. sor vége ?
    {
      ch.chr = SETDDRAMADDR3;           // DDRAM = 3.sor eleje
      LcdWrite();
      LcdWrite2();
    }
    else if(LcdPos == (LCDWIDTH * 3))   // 3. sor vége ?
    {
      ch.chr = SETDDRAMADDR4;           // DDRAM = 4.sor eleje
      LcdWrite();
      LcdWrite2();
    }
    else if(LcdPos == (LCDWIDTH * 4))   // 4. sor vége ?
    {
      ch.chr = SETDDRAMADDR1;           // DDRAM = 1.sor eleje
      LcdWrite(); 
      LcdWrite2();
      LcdPos = 0;
      BLINKER();
    }
    #endif // LCDLINES
    LcdStatus = LCHAR;                  // a következö hívásnál már karakter megy a kijelzöre
  }

  //----------------------------------------------------------------------------
  else if(LcdStatus == HOME)            // DDRAM = 0
  {
    GPIOX_CLRBIT(LCDRS);                // parancs megy majd a kijelzöre (RS = 0)
    ch.chr = SETDDRAMADDR1;             // DDRAM = 1.sor eleje
    LcdWrite();
    LcdWrite2();
    LcdPos = 0;
    LcdStatus = LCHAR;                  // a következö hívásnál már karakter megy a kijelzöre
  }

  #ifdef LCDCURSOR
  //----------------------------------------------------------------------------
  else if(LcdStatus == CURPOS)          // kurzor pozíció állítási fázis
  {
    GPIOX_CLRBIT(LCDRS);                // parancs megy majd a kijelzöre (RS = 0)

    // ----------------------------------- 1 soros kijelzö
    #if LCDLINES == 1
    ch.chr = SETDDRAMADDR1 + LcdCursorPos;// DDRAM = 1.sor + kurzor pozíció
    LcdWrite();

    #elif LCDLINES == 2
    // ----------------------------------- 2db 2 soros kijelzö
    #ifdef LCDE2
    if(LcdCursorPos < LCDCHARPERMODUL)
    {                                   // a kurzor az 1.modulban van
      if (LcdCursorPos < LCDWIDTH)
        ch.chr = SETDDRAMADDR1 + LcdCursorPos;// DDRAM = 1.sor + kurzor pozíció
      else
        ch.chr = SETDDRAMADDR2 - LCDWIDTH + LcdCursorPos;// DDRAM = 2.sor - 1*sorhossz + kurzor pozíció
    }
    else
    {                                   // a kurzor az 2.modulban van
      if (LcdCursorPos < LCDCHARPERMODUL + LCDWIDTH)
        ch.chr = SETDDRAMADDR1 - LCDCHARPERMODUL + LcdCursorPos;// DDRAM = 1.sor - modulkarakterszám + kurzor pozíció
      else
        ch.chr = SETDDRAMADDR2 - LCDCHARPERMODUL - LCDWIDTH + LcdCursorPos;// DDRAM = 2.sor - modulkarakterszám - 1*sorhossz + kurzor pozíció
    }
    LcdWrite();
    LcdWrite2();
    #else  // LCDE2

    // ----------------------------------- 2 soros kijelzö
    if (LcdCursorPos < LCDWIDTH)
      ch.chr = SETDDRAMADDR1 + LcdCursorPos;// DDRAM = 1.sor + kurzor pozíció
    else
      ch.chr = SETDDRAMADDR2 - LCDWIDTH + LcdCursorPos;// DDRAM = 2.sor - 1 * sorhossz + kurzor pozíció
    LcdWrite();
    #endif // else LCDE2

    // ----------------------------------- 2db 4 soros kijelzö
    #elif LCDLINES == 4
    #ifdef LCDE2

    if(LcdCursorPos < LCDCHARPERMODUL)
    {                                   // a kurzor az 1.modulban van
      if(LcdCursorPos < LCDWIDTH)
      {
        ch.chr = SETDDRAMADDR1 + LcdCursorPos;// DDRAM = 1.sor + kurzor pozíció
      }
      else if(LcdCursorPos < 2 * LCDWIDTH)
      {
        ch.chr = SETDDRAMADDR2 - LCDWIDTH + LcdCursorPos;// DDRAM = 2.sor - 1*sorhossz + kurzor pozíció
      }
      else if(LcdCursorPos < 3 * LCDWIDTH)
      {
        ch.chr = SETDDRAMADDR3 - 2*LCDWIDTH + LcdCursorPos;// DDRAM = 3.sor - 2*sorhossz + kurzor pozíció
      }
      else
      {
        ch.chr = SETDDRAMADDR4 - 3*LCDWIDTH + LcdCursorPos;// DDRAM = 4.sor - 3*sorhossz + kurzor pozíció
      }
    }
    else
    {                                   // a kurzor az 2.modulban van
      if(LcdCursorPos < LCDWIDTH + LCDCHARPERMODUL)
      {
        ch.chr = SETDDRAMADDR1 - LCDCHARPERMODUL + LcdCursorPos;// DDRAM = 1.sor + kurzor pozíció
      }
      else if(LcdCursorPos < 2 * LCDWIDTH + LCDCHARPERMODUL)
      {
        ch.chr = SETDDRAMADDR2 - LCDCHARPERMODUL - LCDWIDTH + LcdCursorPos;// DDRAM = 2.sor - 1*sorhossz + kurzor pozíció
      }
      else if(LcdCursorPos < 3 * LCDWIDTH + LCDCHARPERMODUL)
      {
        ch.chr = SETDDRAMADDR3 - LCDCHARPERMODUL - 2*LCDWIDTH + LcdCursorPos;// DDRAM = 3.sor - 2*sorhossz + kurzor pozíció
      }
      else
      {
        ch.chr = SETDDRAMADDR4 - LCDCHARPERMODUL - 3*LCDWIDTH + LcdCursorPos;// DDRAM = 4.sor - 3*sorhossz + kurzor pozíció
      }
    }
    LcdWrite();
    LcdWrite2();
  
    // ----------------------------------- 4 soros dupla vezérlös kijelzö (2db 2soros)
    #else  // LCDE2
    if(LcdCursorPos < LCDWIDTH)
    {
      ch.chr = SETDDRAMADDR1 + LcdCursorPos;// DDRAM = 1.sor + kurzor pozíció
    }
    else if(LcdCursorPos < 2 * LCDWIDTH)
    {
      ch.chr = SETDDRAMADDR2 - LCDWIDTH + LcdCursorPos;// DDRAM = 2.sor - 1*sorhossz + kurzor pozíció
    }
    else if(LcdCursorPos < 3 * LCDWIDTH)
    {
      ch.chr = SETDDRAMADDR3 - 2*LCDWIDTH + LcdCursorPos;// DDRAM = 3.sor - 2*sorhossz + kurzor pozíció
    }
    else
    {
      ch.chr = SETDDRAMADDR4 - 3*LCDWIDTH + LcdCursorPos;// DDRAM = 4.sor - 3*sorhossz + kurzor pozíció
    }
    LcdWrite();

    #endif // else LCDE2
    #endif // LCDLINES 

    LcdStatus = CURTYPE;                // a következö hívásnál kurzortípus beállítás
  }

  //----------------------------------------------------------------------------
  else if(LcdStatus == CURTYPE)         // kurzor típus állítási fázis
  {
    GPIOX_CLRBIT(LCDRS);                // parancs megy majd a kijelzöre (RS = 0)

    #ifdef LCDE2
    if(LcdCursorPos < LCDCHARPERMODUL)
    { // elsö modulban van
      ch.chr = LcdCursorType | 0b00001100;
      LcdWrite();                       // kurzor a beállítás
      ch.chr = 0b00001100;
      LcdWrite2();                      // kurzor kikapcs
    }
    else
    { // második modulban van a kurzor
      ch.chr = 0b00001100;
      LcdWrite();                       // kurzor kikapcs
      ch.chr = LcdCursorType | 0b00001100;
      LcdWrite2();                      // kurzor a beállítás
    }

    #else  // LCDE2
    ch.chr = LcdCursorType | 0b00001100;
    LcdWrite();
    #endif // LCDE2

    #ifdef LCDMODEONCEIRQ
    LcdRefreshStop();                   // kész az LCD frissítése, leállítható
    #else  // LCDMODEONCEIRQ
    LcdStatus = REFREND;                // kész az LCD frissítése, LcdRefresh while ciklusa befejezödhet
    #endif // else LCDMODEONCEIRQ
  }
  #endif // LCDCURSOR

  //----------------------------------------------------------------------------
  // Karakterkészlet frissítés RAM területröl
  #ifdef USERCHARSETCHANGE
  else if(LcdStatus == CGR)             // CGRAM cím beállítási fázis
  {
    GPIOX_CLRBIT(LCDRS);                // parancs megy a kijelzöre (RS = 0)
    ch.chr = SETCGRAMADDR;              // CGRAM = 0
    LcdWrite();
    LcdWrite2();
    LcdPos = 0;                         // karakter generátor karakter számláló
    LcdStatus = CHARGEN;                // karakter generátor feltöltése következik
  }

  //----------------------------------------------------------------------------
  else if(LcdStatus == CHARGEN)         // karakter generátor feltöltése
  {
    GPIOX_SETBIT(LCDRS);                // karakter megy a kijelzöre (RS = 1)
    ch.chr = *uchp++;                   // karakter leíró tömb
    LcdWrite();
    LcdWrite2();
    LcdPos++;
    if(LcdPos >= 64)
    {                                   // karaktergenerátor feltöltés befejezödött
      #if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY))
      #ifdef LCDCURSOR
      LcdStatus = CURPOS;               // egyszeri frissítési mód, megszakítás nélkül kurzorral: kurzor pozíció beállítás
      #else  // LCDCURSOR
      LcdStatus = REFREND;              // egyszeri frissítési mód, megszakítás nélkül: kész az LCD karaktergenerátor frissítése -> LcdRefresh while ciklusa befejezödhet
      #endif // else LCDCURSOR
      #endif // ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY))

      #ifdef LCDMODEONCEIRQ
      LcdStatus = HOME;                 // egyszeri frissítési mód megszakítással: DDRAM = 0 lesz a következö körben
      #endif // LCDMODEONCEIRQ

      #if ((defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ))
      LcdStatus = HOME;                 // folyamatos frissítési mód: DDRAM = 0 lesz a következö körben
      #ifdef AUTOBLINKER
      #if LCDCHARPERMODUL <= 24
      BLINKER();                        // hogy a villogási ütemezés ne maradjon ki
      BLINKER();                        // 1x16, 1x20-as kijelzöknél a 64 byte feltöltése kb annyi ideig tart mint négy frame
      BLINKER();
      BLINKER();
      #elif LCDCHARPERMODUL <= 48
      BLINKER();                        // hogy a villogási ütemezés ne maradjon ki
      BLINKER();                        // 2x16, 2x20-as kijelzöknél a 64 byte feltöltése kb annyi ideig tart mint két frame
      #else  // LCDCHARPERMODUL
      BLINKER();                        // nagyobb kijelzöknél a 64 byte feltöltése kb annyi ideig tart mint egy frame
      #endif // LCDCHARPERMODUL
      #endif // AUTOBLINKER
      #endif // (defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ)
    }
  }
  #endif // USERCHARSETCHANGE

  //----------------------------------------------------------------------------
  // Karakterkészlet frissítés ROM területröl
  #ifdef USERCHARSETCHANGEROM
  else if(LcdStatus == CGRROM)          // CGRAM cím beállítási fázis
  {
    GPIOX_CLRBIT(LCDRS);                // parancs megy a kijelzöre (RS = 0)
    ch.chr = SETCGRAMADDR;              // CGRAM = 0
    LcdWrite();
    LcdWrite2();
    LcdPos = 0;                         // karakter generátor karakter számláló
    LcdStatus = CHARGENROM;             // karakter generátor feltöltése következik
  }

  //----------------------------------------------------------------------------
  else if(LcdStatus == CHARGENROM)      // karakter generátor feltöltése
  {
    GPIOX_SETBIT(LCDRS);                // karakter megy a kijelzöre (RS = 1)
    ch.chr = *uchpr++;                  // karakter leíró tömb
    LcdWrite();
    LcdWrite2();
    LcdPos++;
    if(LcdPos >= 64)
    {                                   // karaktergenerátor feltöltés befejezödött
      #if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY))
      #ifdef LCDCURSOR
      LcdStatus = CURPOS;               // egyszeri frissítési mód, megszakítás nélkül kurzorral: kurzor pozíció beállítás
      #else  // LCDCURSOR
      LcdStatus = REFREND;              // egyszeri frissítési mód, megszakítás nélkül: kész az LCD karaktergenerátor frissítése -> LcdRefresh while ciklusa befejezödhet
      #endif // else LCDCURSOR
      #endif // ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY))

      #ifdef LCDMODEONCEIRQ
      LcdStatus = HOME;                 // egyszeri frissítési mód megszakítással: DDRAM = 0 lesz a következö körben
      #endif // LCDMODEONCEIRQ

      #if ((defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ))
      LcdStatus = HOME;                 // folyamatos frissítési mód: DDRAM = 0 lesz a következö körben
      #ifdef AUTOBLINKER
      #if LCDCHARPERMODUL <= 24
      BLINKER();                        // hogy a villogási ütemezés ne maradjon ki
      BLINKER();                        // 1x16, 1x20-as kijelzöknél a 64 byte feltöltése kb annyi ideig tart mint négy frame
      BLINKER();
      BLINKER();
      #elif LCDCHARPERMODUL <= 48
      BLINKER();                        // hogy a villogási ütemezés ne maradjon ki
      BLINKER();                        // 2x16, 2x20-as kijelzöknél a 64 byte feltöltése kb annyi ideig tart mint két frame
      #else  // LCDCHARPERMODUL
      BLINKER();                        // nagyobb kijelzöknél a 64 byte feltöltése kb annyi ideig tart mint egy frame
      #endif // LCDCHARPERMODUL
      #endif // AUTOBLINKER
      #endif // (defined LCDMODECONTBUSY) || (defined LCDMODECONTIRQ)
    }
  }
  #endif // USERCHARSETCHANGEROM
}

#if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY) || (defined LCDMODEONCEIRQ))
//==============================================================================
// LcdRefreshAll (csak egyszeri frissítési üzemmódban: a teljes kijelzötartalom frissítése)
// Elöfeltétel: LcdInit() inicializálni kell
// Input:           LcdText[], villogó módban LcdBlink[]
// Áttekintés:      LcdText[] teljes tartalmát átmásolja az LCD kijerlzöre 
//                  megszakítás nélküli mód:
//                    addig tart a függvém lefutása, amíg az átmásolás tart (blokkoló függvény!)
//                  megszakításos mód:
//                    csak elindítja a másolást, a karakterek kiírása megszakításból fog megtörténni,
//                    a teljes tartalom kiírása után leállítja a frissítést
// Megjegyzés:      
//==============================================================================
void LcdRefreshAll(void)
{
  #ifdef LCDMODEONCEIRQ
  #ifdef USERCHARSETCHANGE
  if(!LcdRefreshed())
  { // frissítés folyamatban van még
    if((LcdStatus == CGR) || (LcdStatus == CHARGEN))
    { // karaktergenerátor frissítés van folyamatban, semmit nem kell csinálni
    } // mert a karaktergenerátor végeztével a kijelzö tartalmat is újraírja
    else
    { // szöveg frissítés van még folyamatban -> elölröl kezdeni
      LcdStatus = HOME;
      LcdRefreshStart();                // mehet (ha pont most állt volna le)
    }
  }
  else
  { // nincs frissítés alatt
    LcdStatus = HOME;                   // megszakítás, kijelzö frissítést elölröl kezdeni
    LcdRefreshStart();                  // mehet
  }
  #else  // USERCHARSETCHANGE
  LcdStatus = HOME;                     // megszakítás, karakterkészlet módosítás nélkül
  LcdRefreshStart();
  #endif // USERCHARSETCHANGE
  #endif // LCDMODEONCEIRQ

  #ifdef LCDMODEONCEBUSY
  LcdStatus = HOME;                     // megszakítás nélkül
  while(LcdStatus != REFREND)
    LcdProcess();                       // megvárjuk amíg kiíródik
  #endif

  #ifdef LCDMODEONCEDELAY
  LcdStatus = HOME;                     // megszakítás nélkül
  while(LcdStatus != REFREND)
  {
    DelayLcd();                         // Lcd írás várakozás
    LcdProcess();                       // megvárjuk amíg kiíródik
  }
  #endif
}
#endif //  ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY) || (defined LCDMODEONCEIRQ))

#ifdef USERCHARSETCHANGE
//==============================================================================
// LcdChangeCharset (a 8db felhasználó által definiált karakterkészlet átvitele CGRAM-ba )
// Elöfeltétel: LcdInit(), USERCHARSETCHANGE
// Input:           pch*: a 64 bájtos karaktertábla címe a RAM-ban
// Áttekintés:      Egyszeri frissítési üzemmódban:
//                  megszakítás nélküli mód:
//                    addig tart a függvém lefutása, amíg az átmásolás tart (blokkoló függvény, 
//                    csak a karakterkészletet másolja át, a kijelzö tartalmat nem!)
//                  megszakításos mód:
//                    csak elindítja a másolást, a karakterkészlet és a kijelzö tartalmának kiírása 
//                    (a karakterkészlet után a szöveget is újraírja) megszakításból fog megtörténni,
//                    a teljes tartalom kiírása után leállítja a frissítést
//
//                  Folyamatos frissítési üzemmódban:
//                  megszakítás nélküli mód: 
//                    A kijelzöre történö kiírás a föprogramhurokban az LcdProcess() függvényhívással fog megtörténni (ha a kijelzö szabad).
//                    Ha a teljes karakterkészletet kiírta, akkor automatikusan a tartalom kiírására tér át.
//                  megszakításos mód:
//                    csak elindítja a másolást, a karakterkészlet és a kijelzö tartalmának kiírása megszakításból fog megtörténni,
//                    a teljes tartalom kiírása után automatikusan a tartalom kiírására tér át.
// Megjegyzés:
//==============================================================================
void LcdChangeCharset(char* pch)
{
  LcdStatus = CGR;
  uchp = pch;

  #ifdef LCDMODEONCEIRQ
  LcdRefreshStart();                    // egyszeri frissítési üzemmód, megszakításban
  #endif

  #ifdef LCDMODEONCEBUSY
  while(LcdStatus != REFREND)           // egyszeri frissítési üzemmód, megszakítás nélkül
    LcdProcess();                       // megvárjuk amíg kiíródik
  #endif

  #ifdef LCDMODEONCEDELAY
  while(LcdStatus != REFREND)           // egyszeri frissítési üzemmód várakozásos mód
  {
    DelayLcd();                         // Lcd írás várakozás
    LcdProcess();                       // megvárjuk amíg kiíródik
  }
  #endif
}
#endif // USERCHARSETCHANGE

#ifdef USERCHARSETCHANGEROM
//==============================================================================
// LcdChangeCharset (a 8db felhasználó által definiált karakterkészlet átvitele CGRAM-ba )
// Elöfeltétel: LcdInit(), USERCHARSETCHANGE
// Input:           pch*: a 64 bájtos karaktertábla címe a ROM-ban
// Áttekintés:      Egyszeri frissítési üzemmódban:
//                  megszakítás nélküli mód:
//                    addig tart a függvém lefutása, amíg az átmásolás tart (blokkoló függvény,
//                    csak a karakterkészletet másolja át, a kijelzö tartalmat nem!)
//                  megszakításos mód:
//                    csak elindítja a másolást, a karakterkészlet és a kijelzö tartalmának kiírása
//                    (a karakterkészlet után a szöveget is újraírja) megszakításból fog megtörténni,
//                    a teljes tartalom kiírása után leállítja a frissítést
//
//                  Folyamatos frissítési üzemmódban:
//                  megszakítás nélküli mód:
//                    A kijelzöre történö kiírás a föprogramhurokban az LcdProcess() függvényhívással fog megtörténni (ha a kijelzö szabad).
//                    Ha a teljes karakterkészletet kiírta, akkor automatikusan a tartalom kiírására tér át.
//                  megszakításos mód:
//                    csak elindítja a másolást, a karakterkészlet és a kijelzö tartalmának kiírása megszakításból fog megtörténni,
//                    a teljes tartalom kiírása után automatikusan a tartalom kiírására tér át.
// Megjegyzés:
//==============================================================================
void LcdChangeCharsetRom(rom char* pch)
{
  LcdStatus = CGRROM;
  uchpr = pch;

  #ifdef LCDMODEONCEIRQ
  LcdRefreshStart();                    // egyszeri frissítési üzemmód, megszakításban
  #endif

  #ifdef LCDMODEONCEBUSY
  while(LcdStatus != REFREND)           // egyszeri frissítési üzemmód, megszakítás nélkül
    LcdProcess();                       // megvárjuk amíg kiíródik
  #endif

  #ifdef LCDMODEONCEDELAY
  while(LcdStatus != REFREND)           // egyszeri frissítési üzemmód várakozásos mód
  {
    DelayLcd();                         // Lcd írás várakozás
    LcdProcess();                       // megvárjuk amíg kiíródik
  }
  #endif
}
#endif // USERCHARSETCHANGE
