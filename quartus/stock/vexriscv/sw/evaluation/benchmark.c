#include "benchmark.h"

void soft_vmul(int arr[ELEMENTS], int a[ELEMENTS], int b[ELEMENTS]) {
    for (int i = 0; i < ELEMENTS; ++i) {
        arr[i] = a[i] * b[i];
    }
}

int soft_vacc(int a[ELEMENTS]) {
    int acc = 0;
    for (int i = 0; i < ELEMENTS; ++i) {
        acc += a[i];
    }
    return acc;
}

int soft_vmaxe(int a[ELEMENTS]) {
    int max = a[0];
    for (int i = 1; i < ELEMENTS; ++i) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

int soft_vmine(int a[ELEMENTS]) {
    int min = a[0];
    for (int i = 1; i < ELEMENTS; ++i) {
        if (a[i] < min) {
            min = a[i];
        }
    }
    return min;
}

void soft_vmaxx(int arr[ELEMENTS], int a[ELEMENTS], int s) {
    for (int i = 1; i < ELEMENTS; ++i) {
        if (a[i] > s) {
            arr[i] = a[i];
        } else {
            arr[i] = s;
        }
    }
}

void soft_vsrli(int arr[ELEMENTS], int a[ELEMENTS], int s) {
    for (int i = 0; i < ELEMENTS; ++i) {
        arr[i] = a[i] >> s;
    }
}