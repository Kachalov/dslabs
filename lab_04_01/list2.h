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
uint8_t list2_delete_el(list2_t **list_ptr);
uint8_t list2_add(list2_t **list_ptr);
uint8_t list2_insert(size_t ndx, list2_t **list_ptr);
void list2_delete_by_data(void *data);
