// PIC18xxx USB bootloader és megszakításvektor definíciók
// (a bootloader, csak USB PIC-ek esetén kerül beállításra)

#ifndef _BOOTLOADERS_H
#define _BOOTLOADERS_H

#if (defined(__18F14K50_H) || defined(__18F2550_H) || defined(__18F4550_H))

// ha bootloadert használsz akkor válaszd ki hogy melyiket (csak az egyiket)
// ha nem használsz akkor mindkettõ legyen kikommentezve
// a megfelelõ linker scriptet se felejtsd el a projectben beállítani !
// #define PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER
#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER

#endif

// innentõl kezdve nem kell beleírni semmit sem

void YourHighPriorityISRCode();
void YourLowPriorityISRCode();

#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
  #define REMAPPED_RESET_VECTOR_ADDRESS           0x1000
  #define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS  0x1008
  #define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS   0x1018
#elif defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
  #define REMAPPED_RESET_VECTOR_ADDRESS            0x800
  #define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS   0x808
  #define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS    0x818
#else
  #define REMAPPED_RESET_VECTOR_ADDRESS             0x00
  #define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS    0x08
  #define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS     0x18
#endif


#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
  extern void _startup (void);        // See c018i.c in your C18 compiler dir
  #pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
  void _reset (void)
  {
    _asm goto _startup _endasm
  }
#endif // defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)


#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
void Remapped_High_ISR (void)
{
  _asm goto YourHighPriorityISRCode _endasm
}

#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
void Remapped_Low_ISR (void)
{
  _asm goto YourLowPriorityISRCode _endasm
}


#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
  #pragma code HIGH_INTERRUPT_VECTOR = 0x08
  void High_ISR (void)
  {
    _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
  }

  #pragma code LOW_INTERRUPT_VECTOR = 0x18
  void Low_ISR (void)
  {
    _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
  }
#endif //end of "#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)"

#pragma code

#endif // _BOOTLOADERS_H
