#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stack.h"
#include "lib/errors.h"
#include "lib/list2.h"

int stack_push(stack_t *s, void *v)
{
    if (s->list)
    {
        return stack_push_l(s, v);
    }
    else
    {
        if (s->up)
            return stack_push_au(s, v);
        else
            return stack_push_ad(s, v);
    }
}

int stack_push_au(stack_t *s, void *v)
{
    s->sp = (char *)s->sp + s->data_size;
    if (s->sp > s->hp)
    {
        s->sp = (char *)s->sp - s->data_size;
        return EOOM;
    }

    memcpy(s->sp, v, s->data_size);
    return EOK;
}

int stack_push_ad(stack_t *s, void *v)
{
    s->sp = (char *)s->sp - s->data_size;
    if (s->sp < s->lp)
    {
        s->sp = (char *)s->sp + s->data_size;
        return EOOM;
    }

    memcpy(s->sp, v, s->data_size);
    return EOK;
}

int stack_push_l(stack_t *s, void *v)
{
    if (((list2_t *)(s->sp))->next == NULL)
    {
        list2_add((list2_t **)(&s->sp));
    }

    s->sp = ((list2_t *)(s->sp));
    memcpy(s->sp, v, s->data_size);

    return EOK;
}


void *stack_pop(stack_t *s)
{
    if (s->list)
    {
        return stack_pop_l(s);
    }
    else
    {
        if (s->up)
            return stack_pop_au(s);
        else
            return stack_pop_ad(s);
    }
}

void *stack_pop_au(stack_t *s)
{
    if (s->sp < s->lp)
        return NULL;

    s->sp = (char *)s->sp - s->data_size;
    return (char *)s->sp + s->data_size;
}

void *stack_pop_ad(stack_t *s)
{
    if (s->sp >= s->hp)
        return NULL;

    s->sp = (char *)s->sp + s->data_size;
    return (char *)s->sp - s->data_size;
}

void *stack_pop_l(stack_t *s)
{
    if (s->sp == s->lp)
        return NULL;

    s->sp = ((list2_t *)(s->sp))->prev;
    return ((list2_t *)(s->sp))->next;
}


int stack_init(stack_t **s, bool up, bool list, void *lp, void *hp, size_t size)
{
    *s = (stack_t *)malloc(sizeof(stack_t));
    (*s)->data_size = size;
    (*s)->lp = lp;
    (*s)->hp = hp;
    if (list)
    {
        // TODO: error checking
        if (lp == NULL)
        {
            list2_init((list2_t **)(&(*s)->lp));
        }
        (*s)->sp = (*s)->lp;
        (*s)->hp = NULL;
    }
    else
    {
        (*s)->sp = lp == NULL ? NULL : up ? (char *)lp - size : (char *)hp + size;
    }
    (*s)->up = up;
    (*s)->list = list;

    return EOK;
}

void stack_delete(stack_t **s)
{
    if (*s == NULL)
        return;

    if (((*s)->list))
        list2_delete((list2_t **)(&(*s)->lp));

    free(*s);
    *s = NULL;
}
