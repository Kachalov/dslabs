#pragma once

#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    void *data;
    void *pin;
    void *pout;
    size_t len;
    size_t size;
    size_t n;
} queue_arr_t;

typedef queue_arr_t *queue_arr_p_t;

int queue_arr_init(queue_arr_p_t *q, size_t len, size_t size);
void queue_arr_delete(queue_arr_p_t *q);
int queue_arr_push(queue_arr_t *q, void *data);
void queue_arr_pop(queue_arr_t *q, void **ret);
