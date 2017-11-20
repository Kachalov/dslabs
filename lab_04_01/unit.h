#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define T1 1
#define T2 2

#define T1B 0.f
#define T1E 5.f

#define T2B 0.f
#define T2E 4.f

#define T3B 0.f
#define T3E 4.f

#ifndef QUEUE_SIZE
    #define QUEUE_SIZE 20
#endif

#define MAX_POS 4

#ifndef UNITS
    #define UNITS 1000
#endif

typedef struct
{
    float t;
    uint8_t type;
} unit_t;

void operate(FILE *fout);
float random_time(float a, float b);
