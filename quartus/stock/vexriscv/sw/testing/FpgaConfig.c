#include "FpgaConfig.h"

Pio* g_Pio = (Pio*)(MEMADDR_PIO);
Uart* g_Uart = (Uart*)(MEMADDR_UART);
InterruptController* g_InterruptController = (InterruptController*)(MEMADDR_IRQCONTROLLER);

// Hex Disp Pios

EightBitPio* hex_pio01 = (EightBitPio*)(MEMADDR_HEXPIO_01);
EightBitPio* hex_pio23 = (EightBitPio*)(MEMADDR_HEXPIO_23);
EightBitPio* hex_pio45 = (EightBitPio*)(MEMADDR_HEXPIO_45);