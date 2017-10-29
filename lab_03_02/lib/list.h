#pragma once

#include <inttypes.h>

typedef struct list_st
{
    void *data;
    struct list_st *next;
} list_t;

uint8_t list_init(list_t **list_ptr);
uint8_t list_delete(list_t **list_ptr);
uint8_t list_add(list_t **list_ptr);
