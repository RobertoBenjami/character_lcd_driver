#include <p30Fxxxx.h>

 int FOSC __attribute__((space(prog), address(0xF80000))) = 0xC302 ;
//_FOSC(
//    HS &               // Oscillator (HS)
//    CSW_FSCM_OFF       // Clock Switching and Monitor (Sw Disabled, Mon Disabled)
//);
 int FWDT __attribute__((space(prog), address(0xF80002))) = 0x3F ;
//_FWDT(
//    WDTPSB_16 &        // WDT Prescaler B (1:16)
//    WDTPSA_512 &       // WDT Prescaler A (1:512)
//    WDT_OFF            // Watchdog Timer (Disabled)
//);
 int FBORPOR __attribute__((space(prog), address(0xF80004))) = 0x87B3 ;
//_FBORPOR(
//    PWRT_64 &          // POR Timer Value (64ms)
//    BORV20 &           // Brown Out Voltage (Reserved)
//    PBOR_ON &          // PBOR Enable (Enabled)
//    MCLR_EN            // Master Clear Enable (Enabled)
//);
 int FGS __attribute__((space(prog), address(0xF8000A))) = 0x7 ;
//_FGS(
//    GWRP_OFF &         // General Code Segment Write Protect (Disabled)
//    CODE_PROT_OFF      // General Segment Code Protection (Disabled)
//);
 int FICD __attribute__((space(prog), address(0xF8000C))) = 0xC003 ;
//_FICD(
//    ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)
//);


