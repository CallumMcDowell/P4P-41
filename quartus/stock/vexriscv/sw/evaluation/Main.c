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
#include "hex.h"

#include "customInstruct.h"

void p4p_disp() {
	hex_pio45->port = (0b00011000011001);
	hex_pio23->port = (0b00011001111111);
	hex_write_uint(41, hex_pio01);
}

int main() {

	// Set GPIO to output.
	g_Pio->direction = 0xffffffff;

	uint32_t a;
	init_hex_disp();
	p4p_disp();

	while(1) {
		// uncomment one test function to execute it and print to hex display.


		// test_hex_disp();
	// VACC
		// measure_hard_vacc();
		// measure_soft_vacc();
	// VMUL
		// measure_hard_vmul();
		// measure_soft_vmul();
	// VMAXE
		// measure_hard_vmaxe();
		// measure_soft_vmaxe();
	// VMINE
		// measure_hard_vmine();
		// measure_soft_vmine();
	// VMAX.X
		// measure_hard_vmax_x();
		// measure_soft_vmax_x();
	// VSRLI
		// measure_hard_vsrli();
		// measure_soft_vsrli();
	// Synthetic Benchmarks
		// Typical 2D Convolution Solution in C
		// synthetic_matrix_product_common();
		// synthetic_matrix_product_vector();
	}
}
