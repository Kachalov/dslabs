#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef struct
{
    uint8_t up:1;
    uint8_t list:1;
    void *lp;
    void *hp;
    void *sp;
} stack_t;

int stack_push(stack_t *s);
int stack_pop(stack_t *s);

int stack_init(stack_t *s, void *lp, bool up, bool list, size_t offset);
