#include "benchmark.h"
#include "customInstruct.h"

void soft_vmul(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t b[ELEMENTS]) {
    for (int8_t i = 0; i < ELEMENTS; ++i) {
        arr[i] = a[i] * b[i];
    }
}

int32_t soft_vacc(int8_t a[ELEMENTS]) {
    int32_t acc = 0;
    for (int8_t i = 0; i < ELEMENTS; ++i) {
        acc += a[i];
    }
    return acc;
}

int8_t soft_vmaxe(int8_t a[ELEMENTS]) {
    int8_t max = a[0];
    for (int8_t i = 1; i < ELEMENTS; ++i) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

int8_t soft_vmine(int8_t a[ELEMENTS]) {
    int8_t min = a[0];
    for (int8_t i = 1; i < ELEMENTS; ++i) {
        if (a[i] < min) {
            min = a[i];
        }
    }
    return min;
}

void soft_vmax_x(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t b[]) {
    for (int8_t i = 0; i < ELEMENTS; ++i) {
        if (a[i] > b[i]) {
            arr[i] = a[i];
        } else {
            arr[i] = b[i];
        }
    }
}

void soft_vsrli(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t s) {
    for (int8_t i = 0; i < ELEMENTS; ++i) {
        arr[i] = a[i] >> s;
    }
}


// Hardware instruction
void measure_hard_vacc() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t a, vec0;
    vec0 = build_vec32(5, 10, 15, 20);

    rdinstret0 = rdinstret();
    a = _vacc(a, vec0);	// 50
    rdinstret1 = rdinstret();

    vec0 = build_vec32(1, 2, 3, 4);

    rdcycle0 = rdcycle();
    a = _vacc(a, vec0);	// 10
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}

// Correctness [o]
void measure_hard_vmul() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t a, vec0, vec1;
    vec0 = build_vec32(-1, -2, -3, -4);
    vec1 = build_vec32(1, 2, 3, 4);

    rdinstret0 = rdinstret();
    a = _vmul(a, vec0, vec1);	// 0xfffcf7f0
    rdinstret1 = rdinstret();

    rdcycle0 = rdcycle();
    a = _vmul(a, vec0, vec1);	// 0xfffcf7f0
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}

void measure_hard_vmaxe() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t a, vec0;
    vec0 = build_vec32(-1, -2, -3, -4);

    rdinstret0 = rdinstret();
    a = _vmaxe(a, vec0);	// -1
    rdinstret1 = rdinstret();

    vec0 = build_vec32( 1, 2, 3, 4);

    rdcycle0 = rdcycle();
    a = _vmaxe(a, vec0);	// 4
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}

void measure_hard_vmine() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t a, vec0;
    vec0 = build_vec32(-1, -2, -3, -4);

    rdinstret0 = rdinstret();
    a = _vmine(a, vec0);	// -4
    rdinstret1 = rdinstret();

    vec0 = build_vec32( 1, 2, 3, 4);

    rdcycle0 = rdcycle();
    a = _vmine(a, vec0);	// 1
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}

void measure_hard_vmax_x() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t a, vec0, vec1;
    vec0 = build_vec32(-1, -2, -3, -4);
    vec1 = build_vec32(1, 2, 3, 4);

    rdinstret0 = rdinstret();
    a = _vmax_x(a, vec0, vec1);	// 0x01020304
    rdinstret1 = rdinstret();

    rdcycle0 = rdcycle();
    a = _vmax_x(a, vec0, vec1);	// 0x01020304
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}

void measure_hard_vsrli() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t a, vec0;
    vec0 = build_vec32(0x01, 0x01, 0x01, 0x01);

    rdinstret0 = rdinstret();
    a = _vsrli(a, vec0);	// -4
    rdinstret1 = rdinstret();

    vec0 = build_vec32(0x01, 0x01, 0x01, 0x01);

    rdcycle0 = rdcycle();
    a = _vsrli(a, vec0);	// 1
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}

// // Software instruction 
void measure_soft_vacc() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t a;
    volatile int8_t a_array[ELEMENTS] = {0};

    fill_array(a_array, 5, 10, 15, 20);

    rdinstret0 = rdinstret();
    a = soft_vacc(a_array);
    rdinstret1 = rdinstret();

    fill_array(a_array, -20, -15, -10, -5); // Clears cache

    rdcycle0 = rdcycle();
    a = soft_vacc(a_array);
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);   
}

void measure_soft_vmul() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile int8_t a_array[ELEMENTS] = {0};
	volatile int8_t b_array[ELEMENTS] = {0};
	int8_t soft_array_result[ELEMENTS];

    fill_array(a_array, 1, 2, 3, 4);
    fill_array(b_array, -1, -2, -3, -4);

    rdinstret0 = rdinstret();
    soft_vmul(soft_array_result, a_array, b_array);	
    rdinstret1 = rdinstret();

    rdcycle0 = rdcycle();
    soft_vmul(soft_array_result, a_array, b_array);	
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}

void measure_soft_vmaxe() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t a;
    volatile int8_t a_array[ELEMENTS] = {0};

    fill_array(a_array,-1, -2, -3, -4);

    rdinstret0 = rdinstret();
    a = soft_vmaxe(a_array);
    rdinstret1 = rdinstret();

    fill_array(a_array, -4, -3, -2, -1); // Clears cache

    rdcycle0 = rdcycle();
    a = soft_vmaxe(a_array);
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}

void measure_soft_vmine() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t a;
    volatile int8_t a_array[ELEMENTS] = {0};

    fill_array(a_array,-1, -2, -3, -4);

    rdinstret0 = rdinstret();
    a = soft_vmine(a_array);
    rdinstret1 = rdinstret();

    fill_array(a_array, -1, -2, -3, -4); // Clears cache

    rdcycle0 = rdcycle();
    a = soft_vmine(a_array);
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}

void measure_soft_vmax_x() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile int8_t a_array[ELEMENTS] = {0};
	volatile int8_t b_array[ELEMENTS] = {0};
	int8_t soft_array_result[ELEMENTS];

    fill_array(a_array, 1, 2, 3, 4);
    fill_array(b_array, -1, -2, -3, -4);

    rdinstret0 = rdinstret();
    soft_vmax_x(soft_array_result, a_array, b_array);	
    rdinstret1 = rdinstret();

    rdcycle0 = rdcycle();
    soft_vmax_x(soft_array_result, a_array, b_array);	
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}
void measure_soft_vsrli() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t a;
    volatile int8_t a_array[ELEMENTS] = {0};

    fill_array(a_array,1, 2, 3, 4);

    rdinstret0 = rdinstret();
    a = soft_vmine(a_array);
    rdinstret1 = rdinstret();

    fill_array(a_array, -1, -2, -3, -4); // Clears cache

    rdcycle0 = rdcycle();
    a = soft_vmine(a_array);
    rdcycle1 = rdcycle();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();

    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}


// Cycle Count Overhead Calc
uint32_t rdCycleOverhead() {
    uint32_t rdcycle0, rdcycle1;
    rdcycle0 = rdcycle();
    rdcycle1 = rdcycle();
    uint32_t rdCycleOverhead = rdcycle1 - rdcycle0;
    return rdCycleOverhead;
}

// Instruction Count Overhead Calc
uint32_t rdinstretOverhead() {
    uint32_t rdinstret0, rdinstret1;
    rdinstret0 = rdinstret();
    rdinstret1 = rdinstret();
    uint32_t instretOverhead = rdinstret1 - rdinstret0;	
    return instretOverhead;
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