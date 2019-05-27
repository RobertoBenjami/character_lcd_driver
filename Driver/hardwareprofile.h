#ifndef __HARDWAREPROFILE_H
#define __HARDWAREPROFILE_H

// #define  HARDWARE_MIDICNTR40PINPIC
// #define  HARDWARE_PICDEM2PLUS
// #define  HARDWARE_PICDEM2PLUS_EXT8BITLCD
// #define  HARDWARE_ARDUINOUNO

//==============================================================================
#ifdef  HARDWARE_MIDICNTR40PINPIC

#define SystemClock 48000000
#define CpuClock    48000000

#if (defined(__18F4550_H) || defined(_18F4550))
// AN0..AN7 analóg, a többi digitális I/O
#define InitAD()    ADCON1 = 0x07
#endif

// C18 fordito
#if defined(__18CXX)
// karakteres LCD kijelz? I/O lábak (Midi Mixer)
#define LCDE(f)     f(A, 4)
#define LCDRW(f)    f(B, 5)
#define LCDRS(f)    f(B, 4)
#define LCDDT4(f)   f(B, 0)
#define LCDDT5(f)   f(B, 1)
#define LCDDT6(f)   f(B, 2)
#define LCDDT7(f)   f(B, 3)
#endif  // #if defined(__18CXX)

#if (defined(__XC__) || defined(__PICC18__))
// karakteres LCD kijelz? I/O lábak (Midi Mixer)
#define LCDE        A, 4
#define LCDRW       B, 5
#define LCDRS       B, 4
#define LCDDT4      B, 0
#define LCDDT5      B, 1
#define LCDDT6      B, 2
#define LCDDT7      B, 3
#endif

#endif  // #ifdef  HARDWARE_MIDICNTR40PINPIC

//==============================================================================
#ifdef  HARDWARE_PICDEM2PLUS

#if (defined(_PIC14) || defined(_PIC14E))
#define SystemClock 4000000
#define CpuClock    4000000

// RA0 analóg, VREF=VDD/VSS, többi digitális
#define InitAD()    ADCON1bits.PCFG = 0b1110
#endif

#if (defined(__18CXX) || defined(__PICC18__))
#define SystemClock 10000000
#define CpuClock    10000000

// RA0 analóg, VREF=VDD/VSS, többi digitális
#define InitAD()    ADCON1bits.PCFG = 0b1110
#endif

// C18 fordito
#if defined(__18CXX)
// karakteres LCD kijelz? I/O lábak (Picdem plus)
#define LCDE(f)     f(A, 1)
#define LCDRW(f)    f(A, 2)
#define LCDRS(f)    f(A, 3)
#define LCDDT4(f)   f(D, 0)
#define LCDDT5(f)   f(D, 1)
#define LCDDT6(f)   f(D, 2)
#define LCDDT7(f)   f(D, 3)
#endif  // #if defined(__18CXX)

#if (defined(__XC__) || defined(__PICC18__) || defined(_PIC14) || defined(_PIC14E))
// karakteres LCD kijelzö I/O lábak (Picdem plus)
#define LCDE         A, 1
#define LCDRW        A, 2
#define LCDRS        A, 3
#define LCDDT4       D, 0
#define LCDDT5       D, 1
#define LCDDT6       D, 2
#define LCDDT7       D, 3
#endif

#endif  // #ifdef  HARDWARE_PICDEM2PLUS

//==============================================================================
#ifdef  HARDWARE_PICDEM2PLUS_EXT8BITLCD

#if (defined(_PIC14) || defined(_PIC14E))
#define SystemClock 4000000
#define CpuClock    4000000

// RA0 analóg, VREF=VDD/VSS, többi digitális
#define InitAD()    ADCON1bits.PCFG = 0b1110
#endif

#if (defined(__18CXX) || defined(__PICC18__))
#define SystemClock 10000000
#define CpuClock    10000000

// RA0 analóg, VREF=VDD/VSS, többi digitális
#define InitAD()    ADCON1bits.PCFG = 0b1110
#endif

// C18 fordito
#if defined(__18CXX)
// karakteres LCD kijelz? I/O lábak (Picdem plus)
#define LCDE(f)     f(A, 5)
#define LCDRW(f)    f(A, 2)
#define LCDRS(f)    f(A, 3)
#define LCDDT0(f)   f(D, 0)
#define LCDDT1(f)   f(D, 1)
#define LCDDT2(f)   f(D, 2)
#define LCDDT3(f)   f(D, 3)
#define LCDDT4(f)   f(D, 4)
#define LCDDT5(f)   f(D, 5)
#define LCDDT6(f)   f(D, 6)
#define LCDDT7(f)   f(D, 7)
#endif  // #if defined(__18CXX)

#if (defined(__XC__) || defined(__PICC18__) || defined(_PIC14) || defined(_PIC14E))
// karakteres LCD kijelzö I/O lábak (Picdem plus)
#define LCDE         A, 5
#define LCDRW        A, 2
#define LCDRS        A, 3
#define LCDDT0       D, 0
#define LCDDT1       D, 1
#define LCDDT2       D, 2
#define LCDDT3       D, 3
#define LCDDT4       D, 4
#define LCDDT5       D, 5
#define LCDDT6       D, 6
#define LCDDT7       D, 7
#endif

#endif  // #ifdef  HARDWARE_PICDEM2PLUS_EXT8BITLCD




//==============================================================================
#if (defined(_PIC14) || defined(_PIC14E))
#define SystemClock 4000000
#define CpuClock    4000000

// RA0 analóg, VREF=VDD/VSS, többi digitális
#define InitAD()    ADCON1bits.PCFG = 0b1110

// karakteres LCD kijelz? I/O lábak (PICDEM2 PLUS)
// E
#define LCDE         A, 1
#define LCDE2        B, 3
#define LCDRW        A, 2
#define LCDRS        A, 3
#define LCDDT4       D, 0
#define LCDDT5       D, 1
#define LCDDT6       D, 2
#define LCDDT7       D, 3
#endif

//------------------------------------------------------------------------------
#if defined __18F46K80_H
#define SystemClock 48000000
#define CpuClock    48000000

// karakteres LCD kijelz? I/O lábak:
#define LCDE         B, 2
#define LCDE2        B, 3
#define LCDRW        D, 1
#define LCDRS        B, 1
#define LCDDT0       D, 2
#define LCDDT1       D, 3
#define LCDDT2       C, 4
#define LCDDT3       C, 5
#define LCDDT4       D, 4
#define LCDDT5       D, 5
#define LCDDT6       D, 6
#define LCDDT7       D, 7
#endif

//==============================================================================
#if defined(__dsPIC30F__)
#define SystemClock 16257000
#define CpuClock    16257000

#define LCDE         B, 4
#define LCDE2        B, 3
#define LCDRW        B, 9
#define LCDRS        B, 10
#define LCDDT0       D, 0
#define LCDDT1       D, 1
#define LCDDT2       D, 2
#define LCDDT3       D, 3
#define LCDDT4       D, 1
#define LCDDT5       D, 0
#define LCDDT6       B, 12
#define LCDDT7       B, 11
#endif

//==============================================================================
#if defined(__dsPIC33F__)
#define SystemClock 32000000
#define CpuClock    32000000

#define LCDE         B, 2
#define LCDE2        B, 3
#define LCDRW        B, 0
#define LCDRS        B, 1
#define LCDDT0       D, 0
#define LCDDT1       D, 1
#define LCDDT2       D, 2
#define LCDDT3       D, 3
#define LCDDT4       B, 4
#define LCDDT5       B, 5
#define LCDDT6       B, 6
#define LCDDT7       B, 7
#endif

//==============================================================================
#if defined(__PIC24F__)
#define SystemClock 32000000
#define CpuClock    32000000

#define LCDE         B, 2
#define LCDE2        A, 3
#define LCDRW        B, 0
#define LCDRS        B, 1
#define LCDDT0       D, 0
#define LCDDT1       D, 1
#define LCDDT2       D, 2
#define LCDDT3       D, 3
#define LCDDT4       B, 4
#define LCDDT5       B, 5
#define LCDDT6       B, 6
#define LCDDT7       B, 7
#endif

//==============================================================================
#if defined(__PIC24FK__)
#define SystemClock 32000000
#define CpuClock    32000000

#define LCDE         B, 2
#define LCDE2        A, 3
#define LCDRW        B, 0
#define LCDRS        B, 1
#define LCDDT0       D, 0
#define LCDDT1       D, 1
#define LCDDT2       D, 2
#define LCDDT3       D, 3
#define LCDDT4       B, 4
#define LCDDT5       B, 5
#define LCDDT6       B, 6
#define LCDDT7       B, 7
#endif

//==============================================================================
#if defined(__PIC24H__)
#define SystemClock 32000000
#define CpuClock    32000000

#define LCDE         B, 2
#define LCDE2        A, 3
#define LCDRW        B, 0
#define LCDRS        B, 1
#define LCDDT0       D, 0
#define LCDDT1       D, 1
#define LCDDT2       D, 2
#define LCDDT3       D, 3
#define LCDDT4       B, 4
#define LCDDT5       B, 5
#define LCDDT6       B, 6
#define LCDDT7       B, 7
#endif

//==============================================================================
#if defined(__AVR__)
#define SystemClock 16000000
#define CpuClock    16000000

// E
#define LCDE         B, 2
#define LCDRW        B, 1
#define LCDRS        B, 0
#define LCDDT4       D, 4
#define LCDDT5       D, 5
#define LCDDT6       D, 6
#define LCDDT7       D, 7
#endif

#endif // ifndef __HARDWAREPROFILE_H
