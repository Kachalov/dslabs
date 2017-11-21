#pragma once

#include <inttypes.h>

typedef struct list1_st
{
    int data;
    struct list1_st *next;
} list1_t;

uint8_t list1_init(list1_t **list_ptr);
uint8_t list1_delete(list1_t **list_ptr);
uint8_t list1_add(list1_t **list_ptr);
list1_t *list1_get(list1_t *list_ptr, uint64_t ndx);
