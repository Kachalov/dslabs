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

#ifdef QUEUE_LIST
    #define QUEUE_TYPE queue_lst_t
    #define QUEUE_P_TYPE queue_lst_p_t
    #define QUEUE_INIT(q_ptr) queue_lst_init(&(q_ptr), sizeof(unit_t))
    #define QUEUE_PUSH(q, unit) queue_lst_push(q, &(unit))
    #define QUEUE_POP(q, unit) queue_lst_pop(q, &(unit))
    #define QUEUE_INSERT(q, unit) queue_lst_insert(q, MAX_POS - 1, &(unit))
    #define QUEUE_DELETE(q) queue_lst_delete(&(q))
#else
    #define QUEUE_TYPE queue_arr_t
    #define QUEUE_P_TYPE queue_arr_p_t
    #define QUEUE_INIT(q_ptr) queue_arr_init(&(q_ptr), QUEUE_SIZE, sizeof(unit_t))
    #define QUEUE_PUSH(q, unit) queue_arr_push(q, &(unit))
    #define QUEUE_POP(q, unit) queue_arr_pop(q, &(unit))
    #define QUEUE_INSERT(q, unit) queue_arr_insert(q, MAX_POS - 1, &(unit))
    #define QUEUE_DELETE(q) queue_arr_delete(&(q))
#endif

typedef struct
{
    float time;
    float time_added;
    uint8_t type;
} unit_t;

void operate(FILE *fout);
float random_time(float a, float b);
