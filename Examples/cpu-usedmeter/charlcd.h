// készítö: Roberto Benjami
// verzió:  v0.65 2017.11

#ifndef __CHARLCD_H
#define __CHARLCD_H

//------------------------------------------------------------------------------
// Karakteres LCD kiszolgáló eljárások
// - Inicializálás LcdInit() függvénnyel (ez párszáz msec idöt igényel) 
// - A megjelenítendö szöveget LcdText[] tömbbe tegyük be. 
//   Ez tartalmazza amit a kijelzönek meg kell jelenítenie. 
//   A #0 karakterek helyett szóközt írhat a kijelzöre (beállítástól függöen).
//
// Üzemmódok (az 5 üzemmódból csak egyet válassz ki!):
//-------------------------------------
// #define LCDMODEONCEBUSY
// - Egyszeri frissítési üzemmód megszakítás nélkül, BUSY flag figyeléssel:
//   használata:
//     1. Az LcdText[]-be beírjuk a megjelenítendö szöveget.
//     2. LcdRefreshAll() függvény meghívásával átmásoljuk a kijelzöbe.
//   korlátozások:
//     R/W lábat a foglaltsági állapot visszaolvasás miatt mindenképp használni kell (LCDRWUSED).
//     Automata villogtatás nem müködik, csak úgy lehet a villogást megvalósítani,
//     ha a villogási ütemnek megfelelö idöben BlinkPhase változóba betesszük 
//     az aktuális villogási fázist, majd LcdRefreshAll() függvényt ismét meghívjuk.
//   megjegyzés: 
//     Az LcdRefreshAll() BLOKKOLÓ típusú függvény, addig nem tér vissza, 
//     amíg a kijelzö frissítése folyamatban van.
//     Figyelem! Kijelzö nélkül, hibás vagy rosszul bekötött kijelzövel végtelen ciklusba kerülhet a program.
//
//-------------------------------------
// #define LCDMODEONCEDELAY
// - Egyszeri frissítési üzemmód megszakítás nélkül, LCD írási müveletek között várakozási ciklussal:
//   használata:
//     1. Az LcdText[]-be beírjuk a megjelenítendö szöveget.
//     2. LcdRefreshAll() függvény meghívásával átmásoljuk a kijelzöbe.
//   korlátozások:
//     Automata villogtatás nem müködik, csak úgy lehet a villogást megvalósítani,
//     ha a villogási ütemnek megfelelö idöben BlinkPhase változóba betesszük 
//     az aktuális villogási fázist, majd LcdRefreshAll() függvényt ismét meghívjuk.
//   megjegyzés: 
//     Lehetöség van a kijelzö R/W lábát GND-re kötve egy I/O lábat megspórolni.
//     Az LcdRefreshAll() BLOKKOLÓ típusú függvény, addig nem tér vissza,
//     amíg a kijelzö frissítése folyamatban van.
//
//-------------------------------------
// #define LCDMODEONCEIRQ
// - Egyszeri frissítési üzemmód megszakítással:
//   használata:
//     1. Az LcdText[]-be beírjuk a megjelenítendö szöveget.
//     2. LcdRefreshAll() függvény meghívásával átmásoljuk a kijelzöbe.
//   korlátozások:
//     Automata villogtatás nem müködik, csak úgy lehet a villogást megvalósítani,
//     ha a villogási ütemnek megfelelö idöben BlinkPhase változóba betesszük 
//     az aktuális villogási fázist, majd LcdRefreshAll() függvényt ismét meghívjuk.
//   megjegyzés:
//     Lehetöség van a kijelzö R/W lábát GND-re kötve egy I/O lábat megspórolni.
//     Az LcdRefreshAll() NEM BLOKKOLÓ típusú függvény, csak elindítja a kijelzö frissítését,
//     a frissítés maga megszakításból fog megtörténni.
//     A frissítés befejeztét az LcdRefreshed() függvénnyel (valójában ez egy változó) kérdezhetjük le:
//     pl. a while(!LcdRefreshed()); addig várakozik, amíg a frissítés folyamatban van.
//     Önmagában nem okoz gondot, ha a frissítés befejezése elött újból meghívjuk az LcdRefreshAll() függvényt.
//     Ilyenkor félbehagyja az elözö frissítési ciklust, és elölröl kezdi.
//     Ha viszont folyamatosan félbeszakítjuk, akkor lehet hogy a szöveg vége sosem fog megjelenni mert nem jut el a végéig.
//     8 bites PIC esetén a közös megszakításkiszolgáló függvénybe be kell illeszteni egy LcdIntProcess() függvényhívást.
//
//-------------------------------------
// #define  LCDMODECONTBUSY
// - Folyamatos frissítési üzemmód megszakítás nélkül:
//   használata:
//     1. Az LcdText[]-be beírjuk a megjelenítendö szöveget
//     2. LcdUpdateChar() függvényt folyamatosan hívogatva idövel át fog kerülni a kijelzöre
//        (ezt a függvényt a föprogramhurokba érdemes berakni, hogy rendszeresen meghívódjon)
//   korlátozások:
//     R/W lábat a foglaltsági állapot visszaolvasás miatt mindenképp használni kell (LCDRWUSED).
//     Automata villogtatás nem müködik, csak úgy lehet a villogást megvalósítani,
//     ha a villogási ütemnek megfelelö idöben BlinkPhase változóba betesszük az aktuális villogási fázist.
//     Kurzor nem használható.
//   megjegyzés: 
//     Az LcdUpdateChar() NEM BLOKKOLÓ típusú függvény, ha a kijelzö foglalt -> azonnal visszatér,
//     ha szabad -> egy (dupla kijelzös módban kettö) karaktert ír ki a kijelzö(k)re.
//     
//-------------------------------------
#define  LCDMODECONTIRQ
// - Folyamatos frissítési üzemmód megszakítással:
//   használata:
//     1. Az LcdText[]-be beírjuk a megjelenítendö szöveget
//     2. PIC16, PIC18 esetén a közös megszakításkiszolgáló programba beillesztjük
//        az LcdIntProcess() függvényhívást.
//   korlátozások:
//     Kurzor nem használható.
//   megjegyzés: 
//     Lehetöség van a kijelzö R/W lábát GND-re kötve egy I/O lábat megspórolni.
//     Automata villogtatás csak ebben az üzemmódban használható.

//==============================================================================
#include "hardwareprofile.h"
// Ha külsö hardverprofilt használunk akkor azt itt adhatjuk meg
// - belsö hardverprofil esetén kommenteljük ki
// - külsö hardverprofil esetén az ott megadott paramétereket ebben a file-ban kommenteljük ki
//------------------------------------------------------------------------------
// LCD kijelzö I/O lábak:
// - LCDE: E láb hozzárendelése
// - LCDE2: E2 láb hozzárendelése (csak két kijelzö, vagy dupla kijelzö esetén asjuk meg)
// - LCDRW: RW láb hozzárendelése (csak akkor adjuk meg, ha használjuk is, nem hasznlva a GND-re kell kötni)
// - LCDDT0..LCDDT3: D0..D3 láb hozzárendelése (csak 8 bites üzemmodban adjuk meg)
// - LCDDT4..LCDDT7: D4..D7 láb hozzárendelése

#ifndef LCDE         // ha hardwareprofile.h -ban E láb hozzárendelését megadjuk, akkor a többit is ott adjuk meg
#if defined(__18CXX)
// E, E2, R/W (interrupt módban lehet nélkülözni), RS, D0..D7 (4bites módban csak D4..D7 lesz használva)
#define LCDE(f)      f(A, 4)
#define LCDE2(f)     f(B, 3)
#define LCDRW(f)     f(B, 5)
#define LCDRS(f)     f(B, 4)
#define LCDDT0(f)    f(C, 0)
#define LCDDT1(f)    f(C, 1)
#define LCDDT2(f)    f(D, 2)
#define LCDDT3(f)    f(D, 3)
#define LCDDT4(f)    f(B, 0)
#define LCDDT5(f)    f(B, 1)
#define LCDDT6(f)    f(B, 2)
#define LCDDT7(f)    f(B, 3)
#else   // defined(__18CXX)
// E, E2, R/W (interrupt módban lehet nélkülözni), RS, D0..D7 (4bites módban csak D4..D7 lesz használva)
#define LCDE         A, 4
#define LCDE2        B, 3
#define LCDRW        B, 5
#define LCDRS        B, 4
#define LCDDT0       C, 0
#define LCDDT1       C, 1
#define LCDDT2       D, 2
#define LCDDT3       D, 3
#define LCDDT4       B, 0
#define LCDDT5       B, 1
#define LCDDT6       B, 2
#define LCDDT7       B, 3

#endif  // else defined(__18CXX)
#endif  // LCDE

//------------------------------------------------------------------------------
// #define SystemClock 4000000
// #define CpuClock    4000000
// Rendszer órajel frekvencia (szükséges az idözítésekhez és a timerek beállításához)
// (ha a CpuClock megegyezik SystemClock-al, akkor CpuClock-ot nem kötelezö megadni)

//==============================================================================
#define LCDLINES  2
// Sorok száma (1, 2, 4 lehet)
//------------------------------------------------------------------------------
#define LCDWIDTH  16
// Soronkénti karakterek száma (16, 20, 24, 40 lehet)
//------------------------------------------------------------------------------
#define LCDTIMERNUM  2
// Timer interrrupt mód: a felhasznált timer kiválasztása
// (lehetöleg olyat, amelyiket nincs másra használva :-)
// - AVR:   0, 1, 2
// - PIC16: 0, 1, 2
// - PIC18: 0, 1, 2, 3
// - PIC24: 1, 2, 3, 4, 5
//------------------------------------------------------------------------------
#define LCDTIMERPR18  -1
// PIC18 csak megszakítás módban: melyik prioritású megszakítást használja az idözítöhöz
// lehetséges értékei:
// - -1 : nincs a többszintö prioritás használva (ilyenkor minden csak a HIGH interruptban megy)
// -  0 : alacsony prioritás
// -  1 : magas prioritás
//------------------------------------------------------------------------------
// #define LCDUSERTIMER
// Csak megszakítás módban, lehetöség van saját idözítés szerint hivogatni az LcdIntProcess() frissítö rutint.
// Ilyenkor nem történik meg a TIMER inicializálása, azt nekünk kell megoldanunk.
// Akkor érdemes ezt használni, ha más célra is használunk idözítöt, ami a kijelzö frissítési
// ütemezése szerinti tempóban jár.
//------------------------------------------------------------------------------
// #define LCDGLOBALINTAUTOSTART
// Csak megszakítás módban, ha ez engedélyezve van, az inicializáláskor bekapcsolja
// a globális megszakítást is. Ha az inicaializáláskor még ezt nem szeretnénk megtenni
// kommenteljük ezt ki!
//------------------------------------------------------------------------------
#define LCDFRAMEPERSEC 20
// Csak megszakítás módban: a kijelzö frissítési sebessége (FPS)
// ajánlott érték: 20
//------------------------------------------------------------------------------
#define LCDEXECUTIONTIME 150
// Csak LCDMODEONCEDELAY módban felhasználva: a kijelzö parancsvégrehajtási ideje (usec)
// (A kijelzö adatlapján EXECUTION TIME találjuk. 40usec a tipikus ideje, legalább ekkora idöt válasszunk!)
// ajánlott érték 50
//------------------------------------------------------------------------------
#define LCDBLINKCHAR
// Villogtathatóak a karakterek
// - villogás bekapcsolása: LcdBlinkChar(n); (n = karakter száma)
// - villogás kikapcsolása: LcdUnBlinkChar(n); (n = karakter száma)
//------------------------------------------------------------------------------
#define LCDBLINKSPEED 5
// A villogó karakterek ennyi ideig frame-ig lesznek sötétek és világosak
// - csak LCDMODECONTIRQ üzemmódban!
// - a többi üzemmmódban saját magunknak kell elöállítani
//   a villogási ütemet és BlinkPhase változóba rakni !
// - ha LCDMODECONTIRQ üzemmódban automatikus helyett
//   saját villogási ütemezést akarsz -> LCDBLINKSPEED 0 értéket adj!
//------------------------------------------------------------------------------
// #define LCDCURSOR
// Kurzorhasználat engedélyezése (csak egyszeri frissítési üzemmódban)
// - kurzor pozíció beállítása: LcdCursorPos = n (0 < n < kijelzö(k) karaktereinek száma - 1)
// - kurzor bekapcsolása: LcdCursorOn()
// - kurzor kikapcsolása: LcdCursorOff()
// - villogó kurzor bekapcsolása: LcdCursorBlink()
// - villogó kurzor kikapcsolása: LcdCursorUnBlink()
//------------------------------------------------------------------------------
#define LCDZEROCHANGE
// Ha definiálva van, az LcdText-ben levö #0 kódú karakterek helyett #32 (SPACE) karaktert
// ír ki a kijelzöre (a számátalakító függvények #0 karaktert tesznek a szám végére).
// Kommenteld ki, ha a #0 kódú karaktert a felhasználó által definiált karakterhez akarod használni.
// (a #0..#7 kódú karakter a #8..#15 kódokon is elérhetö, ezért a kikommentelést nem javaslom)
//==============================================================================
#define USERCHARSET
// Saját karakterkészlet feltöltése inicializáláskor.
// Engedélyezve az USERxCHARy fogja definiálni a karakterek formáját.
// Ezt át lehet szerkeszteni saját igényeink szerint.
//------------------------------------------------------------------------------
#define USERCHARSETCHANGE
// Karakterkészlet futás alatti változtatásának lehetösége RAM területröl.
// - LcdChangeCharset(char* pch) függvénnyel lehet feltölteni az aktuális karakterkészletet.
//   a karakterkészletet USERCHARSETARRAY típusú változóval lehet létrehozni a RAM-ban.
//------------------------------------------------------------------------------
// #define USERCHARSETCHANGEROM
// Karakterkészlet futás alatti változtatásának lehetösége ROM területröl.
// - LcdChangeCharsetRom(rom char* pch) függvénnyel lehet feltölteni az aktuális karakterkészletet.
//   a karakterkészletet USERCHARSETARRAY típusú változóval lehet létrehozni a ROM-ban.
//------------------------------------------------------------------------------
// Saját karakterkészlet használata esetén CGRAM-ba fel lehet tölteni 8db 5x8-as karaktert.
// Az alábbi minta '0'..'7' számot definiál inverz kijelzéssel.
// Ha az 5x8-es karaktermátrix bitje = 0 -> világos, 1 -> fekete (világos hátterü kijelzö esetén).
// Ezeket elérni a 0..7, ill 8..15 karakterkódokon keresztül lehet.
// Javaslom a 8..15  kódokat használni, mert a program a 0 kódot -> 32 (SPACE) kóddá alakíthatja.
// Ezt azért teszi hogy a könyvtári átalakítófüggvények által berakott #0 karaktererek
// ne látszódjanak.
// Figyelem: ha a legalsó sor fekete, akkor nem látszik az alsó vonalas kurzor!

#define USER0CHAR0   0b11111
#define USER0CHAR1   0b11011
#define USER0CHAR2   0b10101
#define USER0CHAR3   0b10101
#define USER0CHAR4   0b10101
#define USER0CHAR5   0b10101
#define USER0CHAR6   0b11011
#define USER0CHAR7   0b11111

#define USER1CHAR0   0b11111
#define USER1CHAR1   0b11101
#define USER1CHAR2   0b11001
#define USER1CHAR3   0b10101
#define USER1CHAR4   0b11101
#define USER1CHAR5   0b11101
#define USER1CHAR6   0b11101
#define USER1CHAR7   0b11111

#define USER2CHAR0   0b11111
#define USER2CHAR1   0b10011
#define USER2CHAR2   0b10101
#define USER2CHAR3   0b11101
#define USER2CHAR4   0b11011
#define USER2CHAR5   0b10111
#define USER2CHAR6   0b10001
#define USER2CHAR7   0b11111

#define USER3CHAR0   0b11111
#define USER3CHAR1   0b10011
#define USER3CHAR2   0b11101
#define USER3CHAR3   0b10011
#define USER3CHAR4   0b11101
#define USER3CHAR5   0b11101
#define USER3CHAR6   0b10011
#define USER3CHAR7   0b11111

#define USER4CHAR0   0b11111
#define USER4CHAR1   0b11101
#define USER4CHAR2   0b11001
#define USER4CHAR3   0b10101
#define USER4CHAR4   0b10001
#define USER4CHAR5   0b11101
#define USER4CHAR6   0b11101
#define USER4CHAR7   0b11111

#define USER5CHAR0   0b11111
#define USER5CHAR1   0b10001
#define USER5CHAR2   0b10111
#define USER5CHAR3   0b10011
#define USER5CHAR4   0b11101
#define USER5CHAR5   0b11101
#define USER5CHAR6   0b10011
#define USER5CHAR7   0b11111

#define USER6CHAR0   0b11111
#define USER6CHAR1   0b11001
#define USER6CHAR2   0b10111
#define USER6CHAR3   0b10011
#define USER6CHAR4   0b10101
#define USER6CHAR5   0b10101
#define USER6CHAR6   0b11011
#define USER6CHAR7   0b11111

#define USER7CHAR0   0b11111
#define USER7CHAR1   0b10001
#define USER7CHAR2   0b11101
#define USER7CHAR3   0b11101
#define USER7CHAR4   0b11011
#define USER7CHAR5   0b10111
#define USER7CHAR6   0b10111
#define USER7CHAR7   0b11111

//==============================================================================
// CharLcd "driver" optimalizálási lehetöségek
// (ha nem akarunk élni vele kommenteljük ki mindet)

//------------------------------------------------------------------------------
// LCD adatlábak optimalizálási lehetösége (hogy gyorsítani lehessen az eredeti bitenkénti írást)
// - LCDDTIN csak LCDMODEONCEBUSY és LCDMODECONTBUSY üzemmódban van használva
// - LCDDT0TO3, LCDDT4TO7 csak 4 bites módban van használva
// - LCDDT0TO7 csak 8 bites módban van használva
// - a kiirandó adat ch.chr-ben illetve ch.bit0...ch.bit7-ben áll rendelkezésre
// - (ha egymást követö lábakra kerülnek az adat lábak, v0.64-tol kezdve a fordíto is tudja optimalizálni)

// LCD adatlábakra csatlakozó I/O bemenetre állítása (LCD -> CPU)
// #define LCDDTIN   TRISB |=  (0b00001111)
// LCD adatlábakra csatlakozó I/O kimenetre állítása (CPU -> LCD)
// #define LCDDTOUT  TRISB &= ~(0b00001111)
// 4bites módban az also 4bit kiírása (az adat a ch.chr alsó felében van jelen)
// #define LCDDT0TO3 LATB = (LATB & ~(0b00001111)) | (ch.chr & 0b00001111)
// 4bites módban a felsö 4bit kiírása (az adat a ch.chr felsö felében van jelen)
// #define LCDDT4TO7 LATB = (LATB & ~(0b00001111)) | ((ch.chr >> 4) & 0b00001111)
// 8bites módban mind a 8bit kiírása (az adat ch.chr-ben van jelen)
// #define LCDDT0TO7 LATB = ch.chr

//------------------------------------------------------------------------------
// Mivel a fordítóprogramok nem optimalizálnak ki egy függvényt, ha ASM betét is van benne (a NOP definíciója ASM betét)
// érdemes olyan utasításra cserélni, ami nem fog semmit csinálni, de egy utasításciklusnyi ideig tart.
// (pl. <64kByte memóriáju PIC18-nál a PCLATU egyébként is mindig nulla, semmi nem történik ha lenullázzuk)
// #define LCDNOP  PCLATU = 0

//==============================================================================
// Innentöl kezdve nem kell belenyúlni !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//==============================================================================
// Használható adatok:
//
// - char LcdText[] : ebbe a karaktereket tartalmazó tömbbe kell elhelyezni a megjelenítendö szöveget
//
// - unsigned char BlinkPhase : villogási fázis (ha 0 -> villogó karakterek látszanak, 1 -> nem látszanak)
//     Megszakításos folyamatos frissítési üzemmódban, ha LCDBLINKSPEED > 0) nem kell használni,
//     ilyenkor automata módban megy a villogtatás
//     A többi üzemmódban nekünk kell a villogási ütemezést biztosítani)
//
// - unsigned char LcdCursorPos : kurzor pozíció (egyszeri frissítési üzemmódban, ha engedélyezve van)
//
// - USERCHARSETARRAY : a futás közbeni felhasználói karakterkészletet ilyen típusú változóba lehet eltárolni
//                      ez egy 64karaktert tároló tömb 
//==============================================================================
// Használható/használandó függvények:
//
// - LcdInit() : Az LCD inicializását, és a felhasználói karakterek feltöltését végzi (futási ideje kb. 130msec).
//
// - LcdProcess() : LCDMODECONTBUSY üzemmódban ezt kell rendszeresen meghívni, hogy frissüljön a kijelzö tartalma.
//
// - LcdIntProcess() : PIC16xxx, PIC18xxx megszakításos üzemmódban a közös megszakításkiszolgálóból kell ezt a függvényt meghívni.
//                     Lehetöség van saját idözítésböl is hivogatni a frissítö rutint, akkor is ezt kell használni (AVR és PIC24-en is).
//
// - LcdBlinkChar(n) : 'n'-edik karakter (0..karakterek száma-1) villogni fog
//
// - LcdUnBlinkChar(n) : 'n'-edik karakter (0..karakterek száma-1) nem fog villogni
//
// - LcdRefreshStart() : Megszakításos üzemmódban ezzel lehet az ideiglenesen leállított frissítést újra elindítani.
//
// - LcdRefreshStop() : Megszakításos üzemmódban ezzel lehet a frissítést ideiglenesen leállítani.
//                      Pl. ha pontos idözítés miatt nem akarjuk, hogy megszakítás keletkezzen az LCD miatt.
//
// - LcdRefreshAll() : Egyszeri frissítési üzemmódban ha újra szeretnénk írni az LCD-t ezzel tehetjük meg.
//                     Akkor célszerü ezt megtenni, amikor az LcdText tömbben teljesen összeállt a megjelenítendö szöveg
//                     Megszakítás nélkül addig a függvényben marad, amíg az LCD írása tart,
//                     megszakításos módban rögtön visszatér. Ekkor az írás befejztét az LcdRefreshed() függvénnyel kérdezhetjük le.
//
// - LcdRefreshed() : Megszakításos egyszeri frissítési üzemmódban:
//                    ha a visszatérö érték == 0 -> LcdRefreshAll() függvényhívás után még tart a frissítés
//                    pl. a while(!LcdRefreshed()); addig várakozik, amíg a frissítés folyamatban van
//
// - LcdCursorOn()      : Kurzor bekapcsolása.
// - LcdCursorOff()     : Kurzor kikapcsolása.
// - LcdCursorBlink()   : Kurzor villogás bekapcsolása.
// - LcdCursorUnBlink() : Kurzor villogás kikapcsolása (egy alsó vonal marad a kurzor).
//
// - LcdChangeCharset(char* pch) : Futás alatt a felhasználói karakterkészlet cseréje.
//                                 Paraméterként a karaktereket tároló 64bájtos tömb címét (USERCHARSETARRAY) kell megadni.
//
//------------------------------------------------------------------------------

// Szöveg tömb
extern volatile char LcdText[];

// Villogás tömb, villogás ki/be
#ifdef LCDBLINKCHAR
extern volatile char LcdBlink[];
extern volatile unsigned char BlinkPhase;// 0 = villogó karakterek látszanak, egyébként nem látszik
#define LcdBlinkChar(s) LcdBlink[ s >> 3 ] |= (1 << ( s & 7))
#define LcdUnBlinkChar(s) LcdBlink[ s >> 3 ] &= ~(1 << ( s & 7))
#endif // LCDBLINKCHAR

// Felhasználói karakterkészlet
#ifdef USERCHARSET
typedef unsigned char USERCHARSETARRAY[64];
#if defined(__18CXX)
extern rom USERCHARSETARRAY userromcharset;
#else  // defined(__18CXX)
extern const USERCHARSETARRAY userromcharset; 
#endif // else defined(__18CXX)
#ifdef USERCHARSETCHANGE
void LcdChangeCharset(char* pch);
#endif // USERCHARSETCHANGE
#endif // USERCHARSET

// Kurzor
#ifdef LCDCURSOR
#define LcdCursorOn()         LcdCursorType |= 2
#define LcdCursorOff()        LcdCursorType &= ~2
#define LcdCursorBlink()      LcdCursorType |= 1
#define LcdCursorUnBlink()    LcdCursorType &= ~1
extern volatile unsigned char LcdCursorType;
extern volatile unsigned char LcdCursorPos;
#endif  // LCDCURSOR

// LCD inicializálása
void LcdInit(void);

// Egy karakter frissítése (csak folyamatos frissítési üzemmódban, megszakítás nélkül)
#ifdef LCDMODECONTBUSY
void LcdProcess(void);
#endif

// Teljes kijelzötartalom frissítése (csak egyszeri frissítési üzemmódban)
#if ((defined LCDMODEONCEBUSY) || (defined LCDMODEONCEDELAY) || (defined LCDMODEONCEIRQ))
void LcdRefreshAll(void);
#endif

//------------------------------------------------------------------------------
#if ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))

#ifdef LCDUSERTIMER

// saját megszakításkezelövel és idözítéskezelövel használva
extern unsigned char LcdIrqStatus;
void LcdProcess(void);
#define LcdIntProcess()    if(!LcdIrqStatus)LcdProcess()
#define LcdRefreshStart()  LcdIrqStatus = 0
#define LcdRefreshStop()   LcdIrqStatus = 1
#ifdef  LCDMODEONCEIRQ
#define LcdRefreshed()     LcdIrqStatus
#endif // LCDMODEONCEIRQ

#else  // LCDUSERTIMER

// AVR: LcdRefreshStart(), LcdRefreshStop(), LcdRefreshed()
#if (defined(__AVR__))

#if LCDTIMERNUM == 0
#if defined OCR0 || defined OCR0A
#ifdef TIMSK0
#define LcdRefreshStart()  {TCNT0 = 0; TIMSK0 = (1<<OCIE0A);}
#define LcdRefreshStop()   TIMSK0 = 0
#define LcdRefreshed()     (!TIMSK0)
#else  // TIMSK0
#define LcdRefreshStart()  {TCNT0 = 0; TIMSK |= (1<<OCIE0);}
#define LcdRefreshStop()   TIMSK &= ~(1<<OCIE0)
#define LcdRefreshed()     (!(TIMSK & (1<<OCIE0)))
#endif // TIMSK0
#else  // defined OCR0 || defined OCR0A
#define LcdRefreshStart()  {TCNT0 = 0; TIMSK = (1<<TOIE0);}
#define LcdRefreshStop()   TIMSK = 0
#define LcdRefreshed()     (!TIMSK)
#endif // defined OCR0 || defined OCR0A

#elif LCDTIMERNUM == 1
// timer bekapcsolás
#ifdef TIMSK1
#define LcdRefreshStart()  {TCNT1 = 0; TIMSK1 = (1<<OCIE1A);}
#define LcdRefreshStop()   TIMSK1 = 0
#define LcdRefreshed()     (!TIMSK1)
#else  // TIMSK1
#define LcdRefreshStart()  {TCNT1 = 0; TIMSK |= (1<<OCIE1A);}
#define LcdRefreshStop()   TIMSK &= ~(1<<OCIE1A)
#define LcdRefreshed()     (!(TIMSK & (1<<OCIE1A)))
#endif // TIMSK1

#elif LCDTIMERNUM == 2
#ifdef TIMSK2
#define LcdRefreshStart()  {TCNT2 = 0; TIMSK2 = (1<<OCIE2A);}
#define LcdRefreshStop()   TIMSK2 = 0
#define LcdRefreshed()     (!TIMSK2)
#else  // TIMSK2
// #define LcdRefreshStart()  {IFR0 &= ~(1<<OCIF2); TIMSK |= (1<<OCIE2);}
#define LcdRefreshStart()  {TCNT2 = 0; TIMSK |= (1<<OCIE2);}
#define LcdRefreshStop()   TIMSK &= ~(1<<OCIE2)
#define LcdRefreshed()     (!(TIMSK & (1<<OCIE2)))
#endif // TIMSK2
#endif // LCDTIMERNUM
#endif // defined(__AVR__)

//------------------------------------------------------------------------------
// PIC16: LcdIntProcess(), LcdRefreshStart(), LcdRefreshStop(), LcdRefreshed()
// interruptba LcdIntProcess() függvényt be kell tenni
#if (defined(_PIC14) || defined(_PIC14E))
void LcdProcess(void);

#if LCDTIMERNUM == 0
#define LcdIntProcess()    if(INTCONbits.T0IE && INTCONbits.T0IF)LcdProcess()
#define LcdRefreshStart()  {TMR0 = 0; INTCONbits.T0IE = 1;}
#define LcdRefreshStop()   INTCONbits.T0IE = 0
#define LcdRefreshed()     (!INTCONbits.T0IE)
#elif LCDTIMERNUM == 1
#define LcdIntProcess()    if(PIR1bits.TMR1IF)LcdProcess()
#define LcdRefreshStart()  T1CONbits.TMR1ON = 1
#define LcdRefreshStop()   T1CONbits.TMR1ON = 0
#define LcdRefreshed()     (!T1CONbits.TMR1ON)
#elif LCDTIMERNUM == 2
#define LcdIntProcess()    if(PIR1bits.TMR2IF)LcdProcess()
#define LcdRefreshStart()  T2CONbits.TMR2ON = 1
#define LcdRefreshStop()   T2CONbits.TMR2ON = 0
#define LcdRefreshed()     (!T2CONbits.TMR2ON)
#endif // LCDTIMERNUM
#endif // defined(_PIC14)

//------------------------------------------------------------------------------
// PIC18: LcdIntProcess(), LcdRefreshStart(), LcdRefreshStop(), LcdRefreshed()
// LOW interruptba LcdIntProcess() függvényt be kell tenni
#if ((defined(__18CXX)) || (defined(__PICC18__)))
void LcdProcess(void);

#if LCDTIMERNUM == 0
#define LcdIntProcess()    if(INTCONbits.TMR0IF)LcdProcess()
#define LcdRefreshStart()  T0CONbits.TMR0ON = 1
#define LcdRefreshStop()   T0CONbits.TMR0ON = 0
#define LcdRefreshed()     (!T0CONbits.TMR0ON)
#elif LCDTIMERNUM == 1
#define LcdIntProcess()    if(PIR1bits.TMR1IF)LcdProcess()
#define LcdRefreshStart()  T1CONbits.TMR1ON = 1
#define LcdRefreshStop()   T1CONbits.TMR1ON = 0
#define LcdRefreshed()     (!T1CONbits.TMR1ON)
#elif LCDTIMERNUM == 2
#define LcdIntProcess()    if(PIR1bits.TMR2IF)LcdProcess()
#define LcdRefreshStart()  T2CONbits.TMR2ON = 1
#define LcdRefreshStop()   T2CONbits.TMR2ON = 0
#define LcdRefreshed()     (!T2CONbits.TMR2ON)
#elif LCDTIMERNUM == 3
#define LcdIntProcess()    if(PIR2bits.TMR3IF)LcdProcess()
#define LcdRefreshStart()  T3CONbits.TMR3ON = 1
#define LcdRefreshStop()   T3CONbits.TMR3ON = 0
#define LcdRefreshed()     (!T3CONbits.TMR3ON)
#endif // LCDTIMERNUM
#endif // defined(__18CXX)

//------------------------------------------------------------------------------
// PIC30: LcdRefreshStart(), LcdRefreshStop(), LcdRefreshed()
#if (defined(__C30__))

#if LCDTIMERNUM == 1
#define LcdRefreshStart()  T1CONbits.TON = 1
#define LcdRefreshStop()   T1CONbits.TON = 0
#define LcdRefreshed()     (!T1CONbits.TON)
#elif LCDTIMERNUM == 2
#define LcdRefreshStart()  T2CONbits.TON = 1
#define LcdRefreshStop()   T2CONbits.TON = 0
#define LcdRefreshed()     (!T2CONbits.TON)
#elif LCDTIMERNUM == 3
#define LcdRefreshStart()  T3CONbits.TON = 1
#define LcdRefreshStop()   T3CONbits.TON = 0
#define LcdRefreshed()     (!T3CONbits.TON)
#elif LCDTIMERNUM == 4
#define LcdRefreshStart()  T4CONbits.TON = 1
#define LcdRefreshStop()   T4CONbits.TON = 0
#define LcdRefreshed()     (!T4CONbits.TON)
#elif LCDTIMERNUM == 5
#define LcdRefreshStart()  T5CONbits.TON = 1
#define LcdRefreshStop()   T5CONbits.TON = 0
#define LcdRefreshed()     (!T5CONbits.TON)
#endif // LCDTIMERNUM
#endif // defined(__C30__)

#endif // else LCDUSERTIMER

#endif // ((defined LCDMODEONCEIRQ) || (defined LCDMODECONTIRQ))

#endif // __CHARLCD_H
