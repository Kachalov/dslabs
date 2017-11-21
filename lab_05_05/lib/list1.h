#pragma once

#include <inttypes.h>

typedef struct list1_st
{
    void *data;
    struct list1_st *next;
} list1_t;

uint8_t list1_init(list1_t **list_ptr);
uint8_t list1_delete(list1_t **list_ptr);
uint8_t list1_add(list1_t **list_ptr);
