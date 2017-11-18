#pragma once

#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    void *data;
    void *in;
    void *out;
    size_t len;
    size_t size;
    size_t n;
} queue_lst_t;

typedef queue_lst_t *queue_lst_p_t;

int queue_lst_init(queue_lst_p_t *q, size_t size);
void queue_lst_delete(queue_lst_p_t *q);
int queue_lst_push(queue_lst_t *q);
int queue_lst_pop(queue_lst_t *q);
