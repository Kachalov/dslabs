#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define T1 1
#define T2 2

#define T1B 0f
#define T1E 5f

#define T2B 0f
#define T2E 4f

#define T3B 0f
#define T3E 4f

#ifndef QUEUE_SIZE
    #define QUEUE_SIZE 20
#endif

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
