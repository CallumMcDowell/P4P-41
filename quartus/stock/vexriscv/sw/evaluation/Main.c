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

#include "benchmark.h"

/* Part of above either or */
#define CUSTOM_INSTRUCT

// Optional test benches
#define CUSTOM_INSTRUCT_VMUL
#define CUSTOM_INSTRUCT_VACC
#define CUSTOM_INSTRUCT_VMAXE_VMINE_VMAX_X
#define CUSTOM_INSTRUCT_VSLRI

#ifdef CUSTOM_INSTRUCT
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

// Helper Funcs
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

int8_t* fill_array(int8_t array[], int8_t a, int8_t b, int8_t c, int8_t d) {

	int8_t vals[ELEMENTS] = {a, b, c, d};

	for(int8_t i=0;i<ELEMENTS;i++) {
		array[i] = vals[i];
	}

	return array; // For easy inline with soft_ functions.
}

int main() {

	// Set GPIO to output.
	g_Pio->direction = 0xffffffff;

	// !!!
	// Note: order of execution seems to influence cycle count measured.
	// !!!
	
	// volatile uint32_t Mcycle0, Mcycle1;

	// volatile uint32_t rdcycle0, rdcycle1;
	// volatile uint32_t rdinstret0, rdinstret1;

	// volatile uint64_t Mcycle2, Mcycle4;

	volatile int32_t z = 0;
	volatile int32_t x;
	volatile int8_t a_array[ELEMENTS] = {0};
	volatile int8_t b_array[ELEMENTS] = {0};

	while(1) {
		
		
		// VACC
		// VMAXE
		// VMINE
		// VMAX.X
		// VMUL
		// VSRLI

		#ifdef CUSTOM_INSTRUCT_VACC

		x = soft_vacc(fill_array(a_array,-1,-1,-1,-1));
		x = soft_vacc(fill_array(a_array,15,15,15,15));
		x = soft_vacc(fill_array(a_array,-128,-128,-128,-128));

		z = _vacc(z, build_vec32(-1,-1,-1,-1));				// -4(signed) 1020 (unsigned)
		z = _vacc(z, build_vec32(15,15,15,15));				// 60 (signed) 60 (unsigned)
		z = _vacc(z, build_vec32(-128,-128,-128,-128));		// -512 (signed) 512 (unsigned)
		#endif // CUSTOM_INSTRUCT_VACC

		#ifdef CUSTOM_INSTRUCT_VMAXE_VMINE_VMAX_X

		x = soft_vmaxe(fill_array(a_array,-1, -2, -3, -4));
		x = soft_vmaxe(fill_array(a_array,1, 2, 3, 4));
		x = soft_vmaxe(fill_array(a_array,-8, 8, 9, -8));
		x = soft_vmaxe(fill_array(a_array,0, 0, 0, 0));
		
		x = soft_vmine(fill_array(a_array,-1, -2, -3, -4));
		x = soft_vmine(fill_array(a_array,1, 2, 3, 4));
		x = soft_vmine(fill_array(a_array,-8, 8, 9, -8));
		x = soft_vmine(fill_array(a_array,0, 0, 0, 0));

		int8_t soft_array_result[ELEMENTS];
		soft_vmax_x(soft_array_result, fill_array(a_array, 1, 2, 3, 4), fill_array(b_array, 0, 0, 0, 0));
		soft_vmax_x(soft_array_result, fill_array(a_array, 0, 0, 0, 0), fill_array(b_array, -1, -2, -3, -4));
		soft_vmax_x(soft_array_result, fill_array(a_array, -1, -2, -3, -4), fill_array(b_array, 1, 2, 3, 4));
		soft_vmax_x(soft_array_result, fill_array(a_array, -1, -9, -8, 4), fill_array(b_array, 4, -2, -3, 4));

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

		soft_vmul(soft_array_result, fill_array(a_array, 1, 2, 3, 4), fill_array(b_array, 0, 0, 0, 0));
		soft_vmul(soft_array_result, fill_array(a_array, 1, 2, 3, 4), fill_array(b_array, -1, -2, -3, -4));
		soft_vmul(soft_array_result, fill_array(a_array, -1, -2, -3, -4), fill_array(b_array, -1, -2, -3, -4));

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
		soft_vsrli(soft_array_result, fill_array(a_array, 1, 2, 3, 4), 1);
		soft_vsrli(soft_array_result, fill_array(a_array, 1, 2, 3, 4), 2);
		soft_vsrli(soft_array_result, fill_array(a_array, 1, 2, 3, 4), 3);
		soft_vsrli(soft_array_result, fill_array(a_array, 1, 2, 3, 4), 4);

		a = _vslri(a, build_vec32(0x01, 0x01, 0x01, 0x01)); //0x02020202
		a = _vslri(a, build_vec32(0x80 ,0x80 ,0x80 ,0x80)); //0x00000000
		a = _vslri(a, build_vec32(0xFF, 0xFF, 0x0F, 0xF0)); //0xfefe1ee0
		#endif // CUSTOM_INSTRUCT_VSLRI


		// // ----------------------------------------------
		// // rdCycle tests
		// rdcycle0 = rdcycle();
		// rdcycle1 = rdcycle();
		// uint32_t rdCycleOverhead = rdcycle1 - rdcycle0;

		// // Is this cached? Whopping 15 cycles, but the following two are the expected
		// // 2 cycles instead??
		// rdcycle0 = rdcycle();
		// x = 1;
		// x = 2;
		// x = 3;
		// rdcycle1 = rdcycle();
		// uint32_t cycleElapsed3 = rdcycle1 - rdcycle0 - rdCycleOverhead;	

		// rdcycle0 = rdcycle();
		// x = 1;
		// rdcycle1 = rdcycle();
		// uint32_t cycleElapsed1 = rdcycle1 - rdcycle0 - rdCycleOverhead;

		// rdcycle0 = rdcycle();
		// x = 1;
		// x = 2;
		// rdcycle1 = rdcycle();
		// uint32_t cycleElapsed2 = rdcycle1 - rdcycle0 - rdCycleOverhead;
		// // ----------------------------------------------
		// // instret tests
		// rdinstret0 = rdinstret();
		// rdinstret1 = rdinstret();
		// uint32_t instretOverhead = rdinstret1 - rdinstret0;	

		// rdinstret0 = rdinstret();
		// x = 1;
		// rdinstret1 = rdinstret();
		// uint32_t instretElapsed1 = rdinstret1 - rdinstret0 - instretOverhead;	

		// rdinstret0 = rdinstret();
		// x = 1;
		// x = 2;
		// rdinstret1 = rdinstret();
		// uint32_t instretElapsed2 = rdinstret1 - rdinstret0 - instretOverhead;	

		// rdinstret0 = rdinstret();
		// x = 1;
		// x = 2;
		// x = 3;
		// rdinstret1 = rdinstret();
		// uint32_t instretElapsed3 = rdinstret1 - rdinstret0 - instretOverhead;		
		// // ----------------------------------------------
		// // Inline assembly tests
		// rdinstret0 = rdinstret();
		// asm("nop");
		// rdinstret1 = rdinstret();
		// uint32_t instretElapsedNop = rdinstret1 - rdinstret0 - instretOverhead;

		// rdcycle0 = rdcycle();
		// asm("addi t0, a0, 1");
		// rdcycle1 = rdcycle();
		// uint32_t cycleElapsedSW = rdcycle1 - rdcycle0 - rdCycleOverhead;

		// rdinstret0 = rdinstret();
		// asm("addi t0, a0, 1");
		// rdinstret1 = rdinstret();
		// uint32_t instretElapsedSW = rdinstret1 - rdinstret0 - instretOverhead;

		// z = 0;
	}	
}
