// processzorcsalád include

//==============================================================================
#if defined(__AVR__)
#define SystemClock 16000000
#define CpuClock    16000000

// E
#define LCDEPORT    'B'
#define LCDEPINNUM   2
// E2
#define LCDE2PORT   'B'
#define LCDE2PINNUM  3
// R/W
#define LCDRWPORT   'B'
#define LCDRWPINNUM  0
// RS
#define LCDRSPORT   'B'
#define LCDRSPINNUM  1

// D0
#define LCDDT0PORT  'C'
#define LCDDT0PINNUM 0
// D1
#define LCDDT1PORT  'C'
#define LCDDT1PINNUM 1
// D2
#define LCDDT2PORT  'D'
#define LCDDT2PINNUM 2
// D3
#define LCDDT3PORT  'D'
#define LCDDT3PINNUM 3

// D4
#define LCDDT4PORT  'D'
#define LCDDT4PINNUM 4
// D5
#define LCDDT5PORT  'D'
#define LCDDT5PINNUM 5
// D6
#define LCDDT6PORT  'D'
#define LCDDT6PINNUM 6
// D7
#define LCDDT7PORT  'D'
#define LCDDT7PINNUM 7

#endif  // (__AVR__)

//==============================================================================
#if (defined(_PIC14) || defined(_PIC14E))
#define SystemClock 20000000
#define CpuClock    20000000

// RA0 analóg, VREF=VDD/VSS, többi digitális
#define InitAD()    ADCON1bits.PCFG = 0b1110

// karakteres LCD kijelzõ I/O lábak (PICDEM2 PLUS)
// E
#define LCDEPORT    'A'
#define LCDEPINNUM   1
// R/W (interrupt módban lehet nélkülözni)
#define LCDRWPORT   'A'
#define LCDRWPINNUM  2
// RS
#define LCDRSPORT   'A'
#define LCDRSPINNUM  3
// D4
#define LCDDT4PORT  'D'
#define LCDDT4PINNUM 0
// D5
#define LCDDT5PORT  'D'
#define LCDDT5PINNUM 1
// D6
#define LCDDT6PORT  'D'
#define LCDDT6PINNUM 2
// D7
#define LCDDT7PORT  'D'
#define LCDDT7PINNUM 3

#endif

//==============================================================================
#if defined __18F4550_H
#define SystemClock 48000000
#define CpuClock    48000000

// AN0..AN7 analóg, a többi digitális I/O
#define InitAD()    ADCON1 = 0x07

// karakteres LCD kijelzõ I/O lábak (Midi Mixer)
// E
#define LCDEPORT    'A'
#define LCDEPINNUM   4
// R/W (interrupt módban lehet nélkülözni)
#define LCDRWPORT   'B'
#define LCDRWPINNUM  5
// RS
#define LCDRSPORT   'B'
#define LCDRSPINNUM  4

// D4
#define LCDDT4PORT  'B'
#define LCDDT4PINNUM 0
// D5
#define LCDDT5PORT  'B'
#define LCDDT5PINNUM 1
// D6
#define LCDDT6PORT  'B'
#define LCDDT6PINNUM 2
// D7
#define LCDDT7PORT  'B'
#define LCDDT7PINNUM 3

#endif

//------------------------------------------------------------------------------
#if defined __18F46K80_H
#define SystemClock 48000000
#define CpuClock    48000000
// karakteres LCD kijelzõ I/O lábak:
// E
#define LCDEPORT    'B'
#define LCDEPINNUM   2
// E2 (csak a 80 karakteresnél nagyobbnak van pl. 4x40 karakteres)
#define LCDE2PORT   'A'
#define LCDE2PINNUM  3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRWPORT   'D'
#define LCDRWPINNUM  1
// RS
#define LCDRSPORT   'B'
#define LCDRSPINNUM  1

// D0..D3 (data) csak 8 bites módban szükséges, 4 bites módban nem lesz használva
// D0
#define LCDDT0PORT  'D'
#define LCDDT0PINNUM 2
// D1
#define LCDDT1PORT  'D'
#define LCDDT1PINNUM 3
// D2
#define LCDDT2PORT  'C'
#define LCDDT2PINNUM 4
// D3
#define LCDDT3PORT  'C'
#define LCDDT3PINNUM 5

// D4
#define LCDDT4PORT  'D'
#define LCDDT4PINNUM 4
// D5
#define LCDDT5PORT  'D'
#define LCDDT5PINNUM 5
// D6
#define LCDDT6PORT  'D'
#define LCDDT6PINNUM 6
// D7
#define LCDDT7PORT  'D'
#define LCDDT7PINNUM 7

#endif

//------------------------------------------------------------------------------
#if (defined __18F452_H) || (defined _PIC18F452_H_)
#define SystemClock 40000000
// #define CpuClock    20000000

// karakteres LCD kijelzõ I/O lábak (PICDEM2 PLUS)
// E
#define LCDEPORT    'A'
#define LCDEPINNUM   1
// R/W (interrupt módban lehet nélkülözni)
#define LCDRWPORT   'A'
#define LCDRWPINNUM  2
// RS
#define LCDRSPORT   'A'
#define LCDRSPINNUM  3
// D4
#define LCDDT4PORT  'D'
#define LCDDT4PINNUM 0
// D5
#define LCDDT5PORT  'D'
#define LCDDT5PINNUM 1
// D6
#define LCDDT6PORT  'D'
#define LCDDT6PINNUM 2
// D7
#define LCDDT7PORT  'D'
#define LCDDT7PINNUM 3
#endif

//==============================================================================
#if defined(__dsPIC30F__)
#define SystemClock 16257000
#define CpuClock    16257000

// E
#define LCDEPORT    'B'
#define LCDEPINNUM   4
// E2 (csak a 80 karakteresnél nagyobbnak van pl. 4x40 karakteres)
// #define LCDE2PORT   'A'
// #define LCDE2PINNUM  3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRWPORT   'B'
#define LCDRWPINNUM  9
// RS
#define LCDRSPORT   'B'
#define LCDRSPINNUM  10

// D0..D3 (data) csak 8 bites módban szükséges, 4 bites módban nem lesz használva
// D0
// #define LCDDT0PORT  'D'
// #define LCDDT0PINNUM 0
// D1
// #define LCDDT1PORT  'D'
// #define LCDDT1PINNUM 1
// D2
// #define LCDDT2PORT  'D'
// #define LCDDT2PINNUM 2
// D3
// #define LCDDT3PORT  'D'
// #define LCDDT3PINNUM 3

// D4
#define LCDDT4PORT  'D'
#define LCDDT4PINNUM 1
// D5
#define LCDDT5PORT  'D'
#define LCDDT5PINNUM 0
// D6
#define LCDDT6PORT  'B'
#define LCDDT6PINNUM 12
// D7
#define LCDDT7PORT  'B'
#define LCDDT7PINNUM 11
#endif

//==============================================================================
#if defined(__dsPIC33F__)
#define SystemClock 32000000
#define CpuClock    32000000

// E
#define LCDEPORT    'B'
#define LCDEPINNUM   2
// E2 (csak a 80 karakteresnél nagyobbnak van pl. 4x40 karakteres)
#define LCDE2PORT   'A'
#define LCDE2PINNUM  3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRWPORT   'B'
#define LCDRWPINNUM  0
// RS
#define LCDRSPORT   'B'
#define LCDRSPINNUM  1

// D0..D3 (data) csak 8 bites módban szükséges, 4 bites módban nem lesz használva
// D0
#define LCDDT0PORT  'D'
#define LCDDT0PINNUM 0
// D1
#define LCDDT1PORT  'D'
#define LCDDT1PINNUM 1
// D2
#define LCDDT2PORT  'D'
#define LCDDT2PINNUM 2
// D3
#define LCDDT3PORT  'D'
#define LCDDT3PINNUM 3

// D4
#define LCDDT4PORT  'B'
#define LCDDT4PINNUM 4
// D5
#define LCDDT5PORT  'B'
#define LCDDT5PINNUM 5
// D6
#define LCDDT6PORT  'B'
#define LCDDT6PINNUM 6
// D7
#define LCDDT7PORT  'B'
#define LCDDT7PINNUM 7

#endif

//==============================================================================
#if defined(__PIC24F__)
#define SystemClock 32000000
#define CpuClock    32000000

// E
#define LCDEPORT    'B'
#define LCDEPINNUM   2
// E2 (csak a 80 karakteresnél nagyobbnak van pl. 4x40 karakteres)
#define LCDE2PORT   'A'
#define LCDE2PINNUM  3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRWPORT   'B'
#define LCDRWPINNUM  0
// RS
#define LCDRSPORT   'B'
#define LCDRSPINNUM  1

// D0..D3 (data) csak 8 bites módban szükséges, 4 bites módban nem lesz használva
// D0
#define LCDDT0PORT  'D'
#define LCDDT0PINNUM 0
// D1
#define LCDDT1PORT  'D'
#define LCDDT1PINNUM 1
// D2
#define LCDDT2PORT  'D'
#define LCDDT2PINNUM 2
// D3
#define LCDDT3PORT  'D'
#define LCDDT3PINNUM 3

// D4
#define LCDDT4PORT  'B'
#define LCDDT4PINNUM 4
// D5
#define LCDDT5PORT  'B'
#define LCDDT5PINNUM 5
// D6
#define LCDDT6PORT  'B'
#define LCDDT6PINNUM 6
// D7
#define LCDDT7PORT  'B'
#define LCDDT7PINNUM 7

#endif

//==============================================================================
#if defined(__PIC24FK__)
#define SystemClock 32000000
#define CpuClock    32000000

// E
#define LCDEPORT    'B'
#define LCDEPINNUM   2
// E2 (csak a 80 karakteresnél nagyobbnak van pl. 4x40 karakteres)
#define LCDE2PORT   'A'
#define LCDE2PINNUM  3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRWPORT   'B'
#define LCDRWPINNUM  0
// RS
#define LCDRSPORT   'B'
#define LCDRSPINNUM  1

// D0..D3 (data) csak 8 bites módban szükséges, 4 bites módban nem lesz használva
// D0
#define LCDDT0PORT  'D'
#define LCDDT0PINNUM 0
// D1
#define LCDDT1PORT  'D'
#define LCDDT1PINNUM 1
// D2
#define LCDDT2PORT  'D'
#define LCDDT2PINNUM 2
// D3
#define LCDDT3PORT  'D'
#define LCDDT3PINNUM 3

// D4
#define LCDDT4PORT  'B'
#define LCDDT4PINNUM 4
// D5
#define LCDDT5PORT  'B'
#define LCDDT5PINNUM 5
// D6
#define LCDDT6PORT  'B'
#define LCDDT6PINNUM 6
// D7
#define LCDDT7PORT  'B'
#define LCDDT7PINNUM 7

#endif

//==============================================================================
#if defined(__PIC24H__)
#define SystemClock 32000000
#define CpuClock    32000000

// E
#define LCDEPORT    'B'
#define LCDEPINNUM   2
// E2 (csak a 80 karakteresnél nagyobbnak van pl. 4x40 karakteres)
#define LCDE2PORT   'A'
#define LCDE2PINNUM  3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRWPORT   'B'
#define LCDRWPINNUM  0
// RS
#define LCDRSPORT   'B'
#define LCDRSPINNUM  1

// D0..D3 (data) csak 8 bites módban szükséges, 4 bites módban nem lesz használva
// D0
#define LCDDT0PORT  'D'
#define LCDDT0PINNUM 0
// D1
#define LCDDT1PORT  'D'
#define LCDDT1PINNUM 1
// D2
#define LCDDT2PORT  'D'
#define LCDDT2PINNUM 2
// D3
#define LCDDT3PORT  'D'
#define LCDDT3PINNUM 3

// D4
#define LCDDT4PORT  'B'
#define LCDDT4PINNUM 4
// D5
#define LCDDT5PORT  'B'
#define LCDDT5PINNUM 5
// D6
#define LCDDT6PORT  'B'
#define LCDDT6PINNUM 6
// D7
#define LCDDT7PORT  'B'
#define LCDDT7PINNUM 7

#endif

