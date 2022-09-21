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
// #define CUSTOM_INSTRUCT

// Optional test benches
// #define CUSTOM_INSTRUCT_VMUL
// #define CUSTOM_INSTRUCT_VACC
// #define CUSTOM_INSTRUCT_VMAXE_VMINE_VMAX_X
// #define CUSTOM_INSTRUCT_VSLRI

#define TEST_CYCLE_COUNT

uint32_t Hal_ReadUCycle32();
uint32_t Hal_ReadMCycle32();
uint32_t Hal_ReadUTime32();
uint64_t Hal_ReadMCycle64();

#ifdef CUSTOM_INSTRUCT

	uint32_t _simd_add(uint32_t r2, uint32_t r1, uint32_t rd);

	// P4P Custom Instructions
	// ---------------------------------------
	// Must Have
	// ---------------------------------------
	#ifdef CUSTOM_INSTRUCT_VMUL
	uint32_t _vmul(uint32_t rd, uint32_t r1, uint32_t r2);
	#endif

	#ifdef CUSTOM_INSTRUCT_VACC
	uint32_t _vacc(uint32_t rd, uint32_t r1);
	#endif

	#ifdef CUSTOM_INSTRUCT_VMAXE_VMINE_VMAX_X
	uint32_t _vmaxe(uint32_t rd, uint32_t r1);
	uint32_t _vmine(uint32_t rd, uint32_t r1);
	uint32_t _vmax_x(uint32_t rd, uint32_t r1, uint32_t r2);
	#endif

	#ifdef CUSTOM_INSTRUCT_VSLRI
	uint32_t _vslri(uint32_t rd, uint32_t r1);
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
	// // Greetings
	// UartWrite(g_Uart, "\n\n* * * VexRiscv Demo  -  ");
	// UartWrite(g_Uart, DBUILD_VERSION);
	// UartWrite(g_Uart, "  - ");
	// UartWrite(g_Uart, DBUILD_DATE);
	// UartWrite(g_Uart, "  * * *\n");

	// // Enable interrupt on timer and uart receive.
	// Hal_SetExtIrqHandler(IRQ_UART, IRQHandlerUart);
	// Hal_EnableInterrupt(IRQ_UART);
	// g_Uart->control |= UART_RRDY;
	// Hal_EnableMachineInterrupt(IRQ_M_EXT);
	// Hal_SetTimerIrqHandler(IRQHandlerTimer);
	// Hal_TimerStart(3 * CLK_FREQ); // 3 seconds
	// Hal_GlobalEnableInterrupts();

	// Binary counter that ticks 32 times per second
	uint32_t timeLast = Hal_ReadTime32();
#endif

	// Set GPIO to output.
	g_Pio->direction = 0xffffffff;

#ifdef TEST_CYCLE_COUNT

	volatile uint32_t Mcycle0, Mcycle1;

	volatile uint32_t rdcycle0, rdcycle1;
	volatile uint32_t rdinstret0, rdinstret1;

	volatile uint64_t Mcycle2, Mcycle4;

	uint32_t x, y, z;

	while(1) {
		// uint32_t timeNow = Hal_ReadTime32();
		// Mcycle0 = Hal_ReadMCycle32();
		// Mcycle1 = Hal_ReadMCycle32();
		// uint32_t myCycleOverhead = Mcycle1 - Mcycle0;

		Mcycle2 = Hal_ReadMCycle64();
		x = x+1;
		Mcycle4 = Hal_ReadMCycle64();
		uint64_t myCycleOverhead64_0 = Mcycle4 - Mcycle2;

		// Mcycle0 = Hal_ReadMCycle32();
		// rdcycle0 = rdcycle();
		// uint32_t myAndrdCycleOverhead = rdcycle0 - Mcycle0;

		rdcycle0 = rdcycle();
		rdcycle1 = rdcycle();
		uint32_t rdCycleOverhead = rdcycle1 - rdcycle0;

		Mcycle2 = Hal_ReadMCycle64();
		x = 1;
		Mcycle4 = Hal_ReadMCycle64();
		uint64_t myCycleOverhead64_1 = Mcycle4 - Mcycle2;

		rdcycle0 = rdcycle();
		x = 1;
		rdcycle1 = rdcycle();
		uint32_t cycleElapsed1 = rdcycle1 - rdcycle0;

		rdcycle0 = rdcycle();
		x = 1;
		x = 2;
		rdcycle1 = rdcycle();
		uint32_t cycleElapsed2 = rdcycle1 - rdcycle0;

		rdcycle0 = rdcycle();
		x = 1;
		x = 2;
		x = 3;
		rdcycle1 = rdcycle();
		uint32_t cycleElapsed3 = rdcycle1 - rdcycle0;	

		rdinstret0 = rdinstret();
		rdinstret1 = rdinstret();
		uint32_t instretOverhead = rdinstret1 - rdinstret0;	

		rdinstret0 = rdinstret();
		asm("nop");
		rdinstret1 = rdinstret();
		uint32_t instretElapsedNop = rdinstret1 - rdinstret0;

		rdcycle0 = rdcycle();
		asm("addi t0, a0, 1");
		rdcycle1 = rdcycle();
		uint32_t cycleElapsedSW = rdcycle1 - rdcycle0;

		rdinstret0 = rdinstret();
		asm("addi t0, a0, 1");
		rdinstret1 = rdinstret();
		uint32_t instretElapsedSW = rdinstret1 - rdinstret0;

		rdinstret0 = rdinstret();
		x = 1;
		rdinstret1 = rdinstret();
		uint32_t instretElapsed1 = rdinstret1 - rdinstret0;	

		rdinstret0 = rdinstret();
		x = 1;
		x = 2;
		rdinstret1 = rdinstret();
		uint32_t instretElapsed2 = rdinstret1 - rdinstret0;	

		rdinstret0 = rdinstret();
		x = 1;
		x = 2;
		x = 3;
		rdinstret1 = rdinstret();
		uint32_t instretElapsed3 = rdinstret1 - rdinstret0;		

		z = 0;
	}	

#endif TEST_CYCLE_COUNT


	// uint32_t timeLast = Hal_ReadMCycle32();
	// while (1) {
	// 	uint32_t timeNow = Hal_ReadMCycle32();
	// 	uint32_t tdiff = (timeNow - timeLast);
	// 	uint32_t truet = (CLK_FREQ / 32);

	// 	if (tdiff > truet) {
	// 		timeLast = timeNow;
	// 		x += 1;
	// 		write_to_port(x);
	// 	}
	// }

#ifdef CUSTOM_INSTRUCT

	// int32_t x, y, z;
	// x = 0x01010000;
	// y = 0x00000101;
	// z = 0x10000000;

	while(1) {
#ifdef CUSTOM_INSTRUCT_VACC
		z = _vacc(z, build_vec32(-1,-1,-1,-1));				// -4(signed) 1020 (unsigned)
		z = _vacc(z, build_vec32(15,15,15,15));				// 60 (signed) 60 (unsigned)
		z = _vacc(z, build_vec32(-128,-128,-128,-128));		// -512 (signed) 512 (unsigned)
#endif // CUSTOM_INSTRUCT_VACC

#ifdef CUSTOM_INSTRUCT_VMAXE_VMINE_VMAX_X

		z = _vmaxe(z, build_vec32(-1, -2, -3, -4)); // -1
		z = _vmaxe(z, build_vec32(1, 2, 3, 4));		// 4
		z = _vmaxe(z, build_vec32(-8, 8, 9, -8));	// 9
		z = _vmaxe(z, build_vec32(0, 0, 0, 0));		// 0
		z = _vmaxe(z, build_vec32(-8, 8, 9, -8));	// 9
		z = _vmaxe(z, build_vec32(9, -8, -8, 8));	// 9
		z = _vmaxe(z, build_vec32(-8, -8, -8, -8));	// -8

		z = _vmine(z, build_vec32(-1, -2, -3, -4)); // -4
		z = _vmine(z, build_vec32(1, 2, 3, 4));		// 1
		z = _vmine(z, build_vec32(-8, 8, 9, -8));	// -8
		z = _vmine(z, build_vec32(0, 0, 0, 0));		// 0
		z = _vmine(z, build_vec32(-8, 8, 9, -8));	// -8
		z = _vmine(z, build_vec32(9, -8, -8, 8));	// -8
		z = _vmine(z, build_vec32(-8, -8, -8, -8));	// -8

		z = _vmax_x(z, build_vec32(1, 2, 3, 4), build_vec32(0, 0, 0, 0));		// 0x01020304
		z = _vmax_x(z, build_vec32(0, 0, 0, 0), build_vec32(-1, -2, -3, -4));	// 0x00000000
		z = _vmax_x(z, build_vec32(-1, -2, -3, -4), build_vec32(1, 2, 3, 4));	// 0x01020304
		z = _vmax_x(z, build_vec32(1, 2, 3, 4), build_vec32(-1, -2, -3, -4));	// 0x01020304
		z = _vmax_x(z, build_vec32(1, 2, 3, 4), build_vec32(4, -2, -3, 4));		// 0x04020304
		z = _vmax_x(z, build_vec32(-1, -9, -8, 4), build_vec32(4, -2, -3, 4));	// 0x04fefd04


#endif // CUSTOM_INSTRUCT_VMINE

#ifdef CUSTOM_INSTRUCT_VMUL

		uint32_t a;
		// Range -8 to 7
		a = _vmul(a, build_vec32(1, 2, 3, 4), build_vec32(0, 0, 0, 0));		// 0x00000000
		a = _vmul(a, build_vec32(0, 0, 0, 0), build_vec32(-1, -2, -3, -4));	// 0x00000000
		a = _vmul(a, build_vec32(-1, -2, -3, -4), build_vec32(1, 2, 3, 4));	// 0xfffcf7f0
		a = _vmul(a, build_vec32(1, 2, 2, 2), build_vec32(1, 2, 2, 3));		// 0x01040406
		a = _vmul(a, build_vec32(1, 2, 3, 4), build_vec32(-1, -2, -3, -4));	// 0xfffcf7f0
		a = _vmul(a, build_vec32(1, 2, 3, 4), build_vec32(4, -2, -3, 4));	// 0x04fcf710
		a = _vmul(a, build_vec32(-1, -8, -8, 4), build_vec32(4, -2, -3, 4));// 0xfc101810

		a = _vmul(a, build_vec32(7, 8, -8, -7), build_vec32(8, -7, 7, -8));	// 0xc838c838
		a = _vmul(a, build_vec32(0, 0, 0, 0), build_vec32(0, 0, 0, 0));

#endif // CUSTOM_INSTRUCT_VMUL

#ifdef CUSTOM_INSTRUCT_VSLRI
		a = _vslri(a, build_vec32(0x01, 0x01, 0x01, 0x01)); //0x02020202
		a = _vslri(a, build_vec32(0x80 ,0x80 ,0x80 ,0x80)); //0x00000000
		a = _vslri(a, build_vec32(0xFF, 0xFF, 0x0F, 0xF0)); //0xfefe1ee0
#endif // CUSTOM_INSTRUCT_VSLRI

		// Included to test compatibility with other custom instructions.
		z = _simd_add(z, x, y);
		z = Hal_ReadTime32();
	}
#endif // CUSTOM_INSTRUCT

#ifdef ARIES_EMBEDDED_CORE
	uint32_t x, y, z;
	x = 1;
	while (1) {
		uint32_t timeNow = Hal_ReadTime32();
		uint32_t cycleNow = Hal_ReadMCycle32();
		uint32_t rdtime = rdtime();
		uint32_t rdcycle = rdcycle();

		uint32_t timediff = (timeNow - timeLast);
		if (timediff > (CLK_FREQ / 32)) {
			timeLast = timeNow;
			x += 1;
			write_to_port(x);
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
