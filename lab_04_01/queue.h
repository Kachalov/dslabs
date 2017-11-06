#pragma once

#include <stdlib.h>

typedef struct
{
    void *data;
    void *in_p;
    void *out_p;
    size_t len;
    size_t size;
    size_t n;
} queue_t;

int queue_init(queue_t **q, size_t len, size_t size);
int queue_delete(queue_t **q);
