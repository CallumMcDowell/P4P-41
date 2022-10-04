#include <stdint.h>

#define ELEMENTS 4

void soft_vmul(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t b[ELEMENTS]);

int32_t soft_vacc(int8_t a[ELEMENTS]);

int8_t soft_vmaxe(int8_t a[ELEMENTS]);

int8_t soft_vmine(int8_t a[ELEMENTS]);

void soft_vmax_x(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t b[]);

void soft_vsrli(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t s);