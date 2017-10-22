#pragma once

#include <inttypes.h>

typedef struct list_st
{
    void *data;
    struct list_st *next;
} list_t;

uint8_t create_list(list_t **list_ptr);
uint8_t delete_list(list_t **list_ptr);
uint8_t add_element(list_t **list_ptr);
