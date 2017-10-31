#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    void *lp; /// Lower border
    void *hp; /// Higer border
    void *sp; /// Current position
    size_t data_size; /// Size of data element
    union
    {
        struct
        {
            uint8_t list:1; /// Is realization list or array
            uint8_t up:1; /// Is array up or down
            uint8_t allocated:1; /// Is stack memory allocated internally
        };
        uint8_t flags;
    };
} lab_stack_t;

int stack_push(lab_stack_t *s, void *v);
int stack_push_au(lab_stack_t *s, void *v);
int stack_push_ad(lab_stack_t *s, void *v);
int stack_push_l(lab_stack_t *s, void *v);

void *stack_pop(lab_stack_t *s);
void *stack_pop_au(lab_stack_t *s);
void *stack_pop_ad(lab_stack_t *s);
void *stack_pop_l(lab_stack_t *s);

int stack_init(lab_stack_t **s, bool up, bool list, void *lp, void *hp, size_t size);
void stack_delete(lab_stack_t **s);
