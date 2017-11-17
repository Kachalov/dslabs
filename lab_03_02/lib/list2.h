#pragma once

#include <inttypes.h>

typedef struct list2_st
{
    void *data;
    struct list2_st *next;
    struct list2_st *prev;
} list2_t;

uint8_t list2_init(list2_t **list_ptr);
uint8_t list2_delete(list2_t **list_ptr);
uint8_t list2_add(list2_t **list_ptr);
