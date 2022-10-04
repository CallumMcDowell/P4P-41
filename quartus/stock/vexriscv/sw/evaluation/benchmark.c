#include "benchmark.h"

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

void soft_vmaxx(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t s) {
    for (int8_t i = 1; i < ELEMENTS; ++i) {
        if (a[i] > s) {
            arr[i] = a[i];
        } else {
            arr[i] = s;
        }
    }
}

void soft_vsrli(int8_t arr[ELEMENTS], int8_t a[ELEMENTS], int8_t s) {
    for (int8_t i = 0; i < ELEMENTS; ++i) {
        arr[i] = a[i] >> s;
    }
}