#include "benchmark.h"
#include "customInstruct.h"

// Software Instructions
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

//------------------------------------------------------------------
/* Helper Functions */
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

//------------------------------------------------------------------

/* Helper Functions */
// By Georage
// Retrived from: https://stackoverflow.com/questions/32373868/how-can-i-pad-and-unpad-a-2d-matrix-with-zeros-in-c
void pad(uint32_t *s, uint32_t *d, uint32_t dim)
{
    int i,j;
    for(i=0;i<dim;i++)
        for(j=0;j<dim;j++)
            *(d+(i*(dim+2)+(dim+3+j)))=*(s+i*dim+j);
}

void fill(uint32_t *s, uint32_t dim)
{
    uint32_t i,j;
    for(i=0;i<dim;i++)
    {
        for(j=0;j<dim;j++) *(s+dim*i+j)=getRand();
    }   
}

// void prnt(uint32_t *s, uint32_t dim)
// {
    // uint32_t i,j;
    // for(i=0;i<dim;i++)
    // {
        // for(j=0;j<dim;j++) printf("%d ",*(s+dim*i+j));
        // printf("\n");
    // }   
// }

// By Pleasant94 and samgak
// Retrived from: https://stackoverflow.com/questions/41452226/c-2d-convolution
// Modified to accomedate padding
void convolution_2D_soft(uint32_t N[][WIDTH2], uint32_t M[][MASK_WIDTH2], uint32_t P[][WIDTH2]) {

    // find center position of kernel (half of kernel size)
    uint32_t kCenterX = MASK_WIDTH2 / 2;
    uint32_t kCenterY = MASK_WIDTH1 / 2;
    
    for (uint32_t i = 1; i < WIDTH1-1; ++i)              // rows
    {
        for (uint32_t j = 1; j < WIDTH2-1; ++j)          // columns
        {
            for (uint32_t m = 0; m < MASK_WIDTH1; ++m)     // kernel rows
            {
                for (uint32_t n = 0; n < MASK_WIDTH2; ++n) // kernel columns
                {
                    // index of input signal, used for checking boundary
                    uint32_t ii = i + (m - kCenterY);
                    uint32_t jj = j + (n - kCenterX);
    
                    // ignore input samples which are out of bound
                    if (ii >= 0 && ii < WIDTH1 && jj >= 0 && jj < WIDTH2)
                        P[i][j] += N[ii][jj] * M[m][n];
                }
            }
        }
    }
}

// Alternative convolution from convolution_2D_soft.
// Has overhead for setting up data for HW instructions.
void convolution_2D_vector_instruct(uint32_t N[][WIDTH2], uint32_t M[][MASK_WIDTH2], uint32_t P[][WIDTH2]) {

    // Load kernal sets:
    uint32_t kernelSet[MASK_WIDTH2], imageSet[MASK_WIDTH2];
    uint32_t resultSet;
    // Temp var:
    uint32_t a,b,c;

    for (uint32_t m = 0; m < MASK_WIDTH1; ++m)     // kernel rows
    {
        kernelSet[m] = build_vec32(0, M[m][0], M[m][1], M[m][2]);
    }

    
    for (uint32_t i = 1; i < WIDTH1-1; ++i)              // rows
    {
        for (uint32_t j = 1; j < WIDTH2-1; ++j)          // columns
        {
            // For Debugging
            // a = N[i-1][j-1];
            // a = N[i-1][j];
            // a = N[i-1][j+1];
            // a = N[i][j-1];
            // a = N[i][j];
            // a = N[i][j+1];
            // a = N[i+1][j-1];
            // a = N[i+1][j];
            // a = N[i+1][j+1];

            // Setup Data
            imageSet[0] = (N[i-1][j-1]);
            imageSet[0] = (imageSet[0]<<8) | (N[i-1][j]);
            imageSet[0] = (imageSet[0]<<8) | (N[i-1][j+1]);
            imageSet[1] = (N[i][j-1]);
            imageSet[1] = (imageSet[1]<<8) | (N[i][j]);
            imageSet[1] = (imageSet[1]<<8) | (N[i][j+1]);
            imageSet[2] = (N[i+1][j-1]);
            imageSet[2] = (imageSet[2]<<8) | (N[i+1][j]);
            imageSet[2] = (imageSet[2]<<8) | (N[i+1][j+1]);

            // Multiply
            a = (uint32_t) _vmul(resultSet, kernelSet[0], imageSet[0]); 
            b = (uint32_t) _vmul(resultSet, kernelSet[1], imageSet[1]);
            c = (uint32_t) _vmul(resultSet, kernelSet[2], imageSet[2]);

            // Accumulate
            P[i][j] =  _vacc(resultSet, a)+_vacc(resultSet, b)+_vacc(resultSet, c);
        }
    }
}

uint32_t getRand() {
    return rand() % 10;
}

void synthetic_matrix_product_common() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    volatile uint32_t input_features[DIMS][DIMS] = {0};
    volatile uint32_t padded_features[DIMS+2][DIMS+2] = {0};
    volatile uint32_t output_features[DIMS+2][DIMS+2] = {0};
    volatile uint32_t kernel[3][3]={ {getRand(),getRand(),getRand()},
                            {getRand(),getRand(),getRand()},
                            {getRand(),getRand(),getRand()}};

    // uint32_t input_features[DIMS][DIMS] = {0};
    // uint32_t padded_features[DIMS+2][DIMS+2] = {0};
    // uint32_t output_features[DIMS+2][DIMS+2] = {0};
    // uint32_t kernel[3][3]={ {getRand(),getRand(),getRand()},
    //                         {getRand(),getRand(),getRand()},
    //                         {getRand(),getRand(),getRand()}};

    fill(input_features, DIMS);
    pad(input_features, padded_features, DIMS);

    rdcycle0 = rdcycle();
    convolution_2D_soft(padded_features, kernel, output_features);
    rdcycle1 = rdcycle();    

    // Clears cache (unnecessary, but included)
    fill(input_features, DIMS);
    pad(input_features, padded_features, DIMS);

    rdinstret0 = rdinstret();
    convolution_2D_soft(padded_features, kernel, output_features);
    rdinstret1 = rdinstret();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();
    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}

void synthetic_matrix_product_vector() {
    uint32_t rdcycle0, rdcycle1;
	uint32_t rdinstret0, rdinstret1;

    rdinstret0 = 0;
    rdinstret1 = 0;
    rdcycle0 = 0;
    rdcycle1 = 0; 

    volatile uint32_t input_features[DIMS][DIMS] = {0};
    volatile uint32_t padded_features[DIMS+2][DIMS+2] = {0};
    volatile uint32_t output_features[DIMS+2][DIMS+2] = {0};
    volatile uint32_t kernel[3][3]={ {getRand(),getRand(),getRand()},
                            {getRand(),getRand(),getRand()},
                            {getRand(),getRand(),getRand()}};

    // uint32_t input_features[DIMS][DIMS] = {0};
    // uint32_t padded_features[DIMS+2][DIMS+2] = {0};
    // uint32_t output_features[DIMS+2][DIMS+2] = {0};
    // uint32_t kernel[3][3]={ {getRand(),getRand(),getRand()},
    //                         {getRand(),getRand(),getRand()},
    //                         {getRand(),getRand(),getRand()}};

    fill(input_features, DIMS);
    pad(input_features, padded_features, DIMS);

    rdcycle0 = rdcycle();
    convolution_2D_vector_instruct(padded_features, kernel, output_features);
    rdcycle1 = rdcycle();

    // Clears cache (unnecessary, but included)
    fill(input_features, DIMS);
    pad(input_features, padded_features, DIMS);

    rdinstret0 = rdinstret();
    convolution_2D_vector_instruct(padded_features, kernel, output_features);
    rdinstret1 = rdinstret();

    uint32_t instretElapsed = rdinstret1 - rdinstret0 - rdinstretOverhead();
    uint32_t cycleElapsed = rdcycle1 - rdcycle0 - rdCycleOverhead();
    hex_write_uint(instretElapsed, INSTR_DISPLAY);
    hex_write_uint(cycleElapsed, CYCLE_DISPLAY);
}