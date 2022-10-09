#include <stdint.h>

#include "hex.h"
#include "RiscvDef.h"

#define ELEMENTS 4
#define CYCLE_DISPLAY hex_pio23
#define INSTR_DISPLAY hex_pio45

// Software versions of instructions
void soft_vmul(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t b[ELEMENTS]);
int32_t soft_vacc(int8_t a[ELEMENTS]);
int8_t soft_vmaxe(int8_t a[ELEMENTS]);
int8_t soft_vmine(int8_t a[ELEMENTS]);
void soft_vmax_x(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t b[]);
void soft_vsrli(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t s);


/* 
Measurements to be made when free running without debug configuration. 
Result displaied on HEX DISPLAY 0-5:
    - HEX4-5: Instructions
    - HEX2-3: Cycles
    - HEX0-1: None
*/

// Hardware instruction 
void measure_hard_vacc();
void measure_hard_vmul();
void measure_hard_vmaxe();
void measure_hard_vmine();
void measure_hard_vmax_x();
void measure_hard_vsrli();

// Software instruction 
void measure_soft_vacc();
void measure_soft_vmul();
void measure_soft_vmaxe();
void measure_soft_vmine();
void measure_soft_vmax_x();
void measure_soft_vsrli();

/* Helper Functions */
uint32_t build_vec32(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
int8_t* fill_array(int8_t array[], int8_t a, int8_t b, int8_t c, int8_t d);

// Cycle Count Overhead Calc
uint32_t rdCycleOverhead();
// Instruction Count Overhead Calc
uint32_t rdinstretOverhead();

/* Synthetic Benchmark */
// Synthetic benchmark: 2D Convolution in C

#define DIMS 4
#define WIDTH1 DIMS+2
#define WIDTH2 DIMS+2
#define MASK_WIDTH1 3
#define MASK_WIDTH2 3

/* Helper Functions */
void pad(uint32_t *s, uint32_t *d, uint32_t dim);
void fill(uint32_t *s, uint32_t dim);
uint32_t getRand();

void convolution_2D_soft(uint32_t N[][WIDTH2], uint32_t M[][MASK_WIDTH2], uint32_t P[][WIDTH2]);
void convolution_2D_vector_instruct(uint32_t N[][WIDTH2], uint32_t M[][MASK_WIDTH2], uint32_t P[][WIDTH2]);
void synthetic_matrix_product();
void synthetic_matrix_product_vector();