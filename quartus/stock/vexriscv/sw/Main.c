/* Copyright (C) 2021 ARIES Embedded GmbH

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE. */

#include <stdint.h>

#include "FpgaConfig.h"
#include "Hal.h"

/* Define either or*/
// #define ARIES_EMBEDDED_CORE
// #define TEST_GPIO_LED

/* Part of above either or */
#define CUSTOM_INSTRUCT
// Optional test benches
#define CUSTOM_INSTRUCT_VACC
#define CUSTOM_INSTRUCT_VMAXE_VMINE

#ifdef CUSTOM_INSTRUCT
uint32_t _simd_add(uint32_t r2, uint32_t r1, uint32_t rd);

// P4P Custom Instructions
// ---------------------------------------
// Must Have
// ---------------------------------------
#ifdef CUSTOM_INSTRUCT_VACC
uint32_t _vacc(uint32_t rd, uint32_t r1);
#endif
#ifdef CUSTOM_INSTRUCT_VMAXE_VMINE
uint32_t _vmaxe(uint32_t rd, uint32_t r1);
#endif

#endif

#ifdef ARIES_EMBEDDED_CORE
static int counterMod = 1;

void IRQHandlerTimer(void) {
	// Invert direction of counter
	counterMod = -counterMod;
	// Rearm timer
	Hal_TimerStart(2 * CLK_FREQ); // 2 seconds
}

void IRQHandlerUart() {
	char c;
	if (g_Uart->status & UART_RRDY) {
		c = UartGet(g_Uart);
		UartPut(g_Uart, c);
	}
	g_Uart->status = 0;
}
#endif

// Test Functions
uint32_t add(uint32_t a, uint32_t b) {
	return a+b;
}

void write_to_port(uint32_t x) {
	g_Pio->port = x;
}

uint32_t build_vec32(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
	uint32_t ret = a;
	ret = (ret << 8) | b;
	ret = (ret << 8) | c;
	ret = (ret << 8) | d;
	return ret;
}

int main() {

#ifdef ARIES_EMBEDDED_CORE
	// Greetings
	UartWrite(g_Uart, "\n\n* * * VexRiscv Demo  -  ");
	UartWrite(g_Uart, DBUILD_VERSION);
	UartWrite(g_Uart, "  - ");
	UartWrite(g_Uart, DBUILD_DATE);
	UartWrite(g_Uart, "  * * *\n");

	// Enable interrupt on timer and uart receive.
	Hal_SetExtIrqHandler(IRQ_UART, IRQHandlerUart);
	Hal_EnableInterrupt(IRQ_UART);
	g_Uart->control |= UART_RRDY;
	Hal_EnableMachineInterrupt(IRQ_M_EXT);
	Hal_SetTimerIrqHandler(IRQHandlerTimer);
	Hal_TimerStart(3 * CLK_FREQ); // 3 seconds
	Hal_GlobalEnableInterrupts();

	// Binary counter that ticks 32 times per second
	uint32_t timeLast = Hal_ReadTime32();
#endif

	// Set GPIO to output.
	g_Pio->direction = 0xffffffff;

#ifdef CUSTOM_INSTRUCT

	int32_t x, y, z;
	x = 0x01010000;
	y = 0x00000101;
	z = 0x10000000;

	uint32_t vacc_r1;

	while(1) {
#ifdef CUSTOM_INSTRUCT_VACC
		z = _vacc(z, build_vec32(-1,-1,-1,-1));		// -4(signed) 1020 (unsigned)
		z = _vacc(z, build_vec32(15,15,15,15));		// 60 (signed) 60 (unsigned)
		z = _vacc(z, build_vec32(-128,-128,-128,-128));		// -512 (signed) 512 (unsigned)
#endif // CUSTOM_INSTRUCT_VACC

#ifdef CUSTOM_INSTRUCT_VMAXE_VMINE

		z = _vmaxe(z, build_vec32(-1, -2, -3, -4)); // -1
		z = _vmaxe(z, build_vec32(1, 2, 3, 4));		// 4
		z = _vmaxe(z, build_vec32(-8, 8, 9, -8));	// 9
		z = _vmaxe(z, build_vec32(0, 0, 0, 0));		// 0
		z = _vmaxe(z, build_vec32(-8, 8, 9, -8));	// 9
		z = _vmaxe(z, build_vec32(9, -8, -8, 8));	// 9
		z = _vmaxe(z, build_vec32(-8, -8, -8, -8));	// -8
		
#endif // CUSTOM_INSTRUCT_VMINE

		// Included to test compatibility with other custom instructions.
		z = _simd_add(z, x, y);
	}
#endif

#ifdef ARIES_EMBEDDED_CORE
	uint32_t x, y, z;
	x = 1;
	while (1) {
		uint32_t timeNow = Hal_ReadTime32();
		if ((timeNow - timeLast) > (CLK_FREQ / 32)) {
			timeLast = timeNow;
			write_to_port(add(x, 1));
		}
	}
#endif

#ifdef TEST_GPIO_LED
	uint32_t x, y, z;
	x = 2;
	y = 3;
	z = 4;

	while(1) {
		write_to_port(add(x, 1));
		write_to_port(x);
		write_to_port(y);
		write_to_port(z);
	}
#endif
}
