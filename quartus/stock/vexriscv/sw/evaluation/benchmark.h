#include <stdint.h>

#define ELEMENTS 4

void soft_vmul(uint32_t arr[ELEMENTS], uint32_t a[ELEMENTS], uint32_t b[ELEMENTS]);

uint32_t soft_vacc(uint32_t a[ELEMENTS]);

uint32_t soft_vmaxe(uint32_t a[ELEMENTS]);

uint32_t soft_vmine(uint32_t a[ELEMENTS]);

void soft_vmaxx(uint32_t arr[ELEMENTS], uint32_t a[ELEMENTS], uint32_t s);

void soft_vsrli(uint32_t arr[ELEMENTS], uint32_t a[ELEMENTS], uint32_t s);