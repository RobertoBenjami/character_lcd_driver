#ifndef __HARDWAREPROFILE_H
#define __HARDWAREPROFILE_H

// processzorcsalád include

//==============================================================================
#if defined(__AVR__)
#define SystemClock 16000000
#define CpuClock    16000000

// E
#define LCDE         B, 3
// E2
// #define LCDE2        B, 3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRW        D, 0
// RS
#define LCDRS        D, 2

// D0
#define LCDDT0       C, 0
// D1
#define LCDDT1       C, 1
// D2
#define LCDDT2       C, 2
// D3
#define LCDDT3       C, 3

// D4
#define LCDDT4       D, 4
// D5
#define LCDDT5       D, 5
// D6
#define LCDDT6       D, 6
// D7
#define LCDDT7       D, 7

//==============================================================================
#elif (defined(_PIC14) || defined(_PIC14E))
#define SystemClock 4000000
#define CpuClock    4000000

// RA0 analóg, VREF=VDD/VSS, többi digitális
#define InitAD()    ADCON1bits.PCFG = 0b1110

// karakteres LCD kijelzõ I/O lábak (PICDEM2 PLUS)
// E
#define LCDE         A, 1
// E2
// #define LCDE2        B, 3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRW        A, 2
// RS
#define LCDRS        A, 3

// D4
#define LCDDT4       D, 0
// D5
#define LCDDT5       D, 1
// D6
#define LCDDT6       D, 2
// D7
#define LCDDT7       D, 3

//==============================================================================
#elif defined __18F4550_H
#define SystemClock 48000000
#define CpuClock    48000000

// AN0..AN7 analóg, a többi digitális I/O
#define InitAD()    ADCON1 = 0x07

// karakteres LCD kijelzõ I/O lábak (Midi Mixer)
// E
#define LCDE         A, 5
// E2
// #define LCDE2        B, 3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRW        B, 5
// RS
#define LCDRS        B, 4

// D4
#define LCDDT4       B, 0
// D5
#define LCDDT5       B, 1
// D6
#define LCDDT6       B, 2
// D7
#define LCDDT7       B, 3

//------------------------------------------------------------------------------
#elif defined __18F46K80_H
#define SystemClock 48000000
#define CpuClock    48000000
// karakteres LCD kijelzõ I/O lábak:
// E
#define LCDE         B, 2
// E2
// #define LCDE2        B, 3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRW        D, 1
// RS
#define LCDRS        B, 1

// D0
#define LCDDT0       D, 2
// D1
#define LCDDT1       D, 3
// D2
#define LCDDT2       C, 4
// D3
#define LCDDT3       C, 5
// D4
#define LCDDT4       D, 4
// D5
#define LCDDT5       D, 5
// D6
#define LCDDT6       D, 6
// D7
#define LCDDT7       D, 7


//------------------------------------------------------------------------------
#elif defined __18F452_H
#define SystemClock 4000000
#define CpuClock    4000000

// karakteres LCD kijelzõ I/O lábak (PICDEM2 PLUS)
// E
#define LCDE         A, 1
// E2
// #define LCDE2        B, 3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRW        A, 2
// RS
#define LCDRS        A, 3

// D4
#define LCDDT4       D, 0
// D5
#define LCDDT5       D, 1
// D6
#define LCDDT6       D, 2
// D7
#define LCDDT7       D, 3

//==============================================================================
#elif defined(__dsPIC30F__)
#define SystemClock 16257000
#define CpuClock    16257000

// E
#define LCDE         B, 4
// E2
// #define LCDE2        B, 3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRW        B, 9
// RS
#define LCDRS        B, 10

// D0
#define LCDDT0       D, 0
// D1
#define LCDDT1       D, 1
// D2
#define LCDDT2       D, 2
// D3
#define LCDDT3       D, 3
// D4
#define LCDDT4       D, 1
// D5
#define LCDDT5       D, 0
// D6
#define LCDDT6       B, 12
// D7
#define LCDDT7       B, 11

//==============================================================================
#elif defined(__dsPIC33F__)
#define SystemClock 32000000
#define CpuClock    32000000

// E
#define LCDE         B, 2
// E2
// #define LCDE2        B, 3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRW        B, 0
// RS
#define LCDRS        B, 1

// D0
#define LCDDT0       D, 0
// D1
#define LCDDT1       D, 1
// D2
#define LCDDT2       D, 2
// D3
#define LCDDT3       D, 3
// D4
#define LCDDT4       B, 4
// D5
#define LCDDT5       B, 5
// D6
#define LCDDT6       B, 6
// D7
#define LCDDT7       B, 7

//==============================================================================
#elif defined(__PIC24F__)
#define SystemClock 32000000
#define CpuClock    32000000

// E
#define LCDE         B, 2
// E2
#define LCDE2        A, 3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRW        B, 0
// RS
#define LCDRS        B, 1

// D0
#define LCDDT0       D, 0
// D1
#define LCDDT1       D, 1
// D2
#define LCDDT2       D, 2
// D3
#define LCDDT3       D, 3
// D4
#define LCDDT4       B, 4
// D5
#define LCDDT5       B, 5
// D6
#define LCDDT6       B, 6
// D7
#define LCDDT7       B, 7

//==============================================================================
#elif defined(__PIC24FK__)
#define SystemClock 32000000
#define CpuClock    32000000

// E
#define LCDE         B, 2
// E2
#define LCDE2        A, 3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRW        B, 0
// RS
#define LCDRS        B, 1

// D0
#define LCDDT0       D, 0
// D1
#define LCDDT1       D, 1
// D2
#define LCDDT2       D, 2
// D3
#define LCDDT3       D, 3
// D4
#define LCDDT4       B, 4
// D5
#define LCDDT5       B, 5
// D6
#define LCDDT6       B, 6
// D7
#define LCDDT7       B, 7

//==============================================================================
#elif defined(__PIC24H__)
#define SystemClock 32000000
#define CpuClock    32000000

// E
#define LCDE         B, 2
// E2
#define LCDE2        A, 3
// R/W (interrupt módban lehet nélkülözni)
#define LCDRW        B, 0
// RS
#define LCDRS        B, 1

// D0
#define LCDDT0       D, 0
// D1
#define LCDDT1       D, 1
// D2
#define LCDDT2       D, 2
// D3
#define LCDDT3       D, 3
// D4
#define LCDDT4       B, 4
// D5
#define LCDDT5       B, 5
// D6
#define LCDDT6       B, 6
// D7
#define LCDDT7       B, 7

#else
#error "ismeretlen processzortípus"
#endif // defined(xxx proci)

#endif // ifndef __HARDWAREPROFILE_H
