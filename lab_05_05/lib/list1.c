#include <stdlib.h>
#include <stdint.h>

#include "list1.h"
#include "debug.h"
#include "errors.h"

uint8_t list1_init(list1_t **list_ptr)
{
    list1_t *list = malloc(sizeof(list1_t));

    if (list == NULL)
    {
        return EOOM;
    }

    list->data = 0;
    list->next = NULL;
    *list_ptr = list;

    return EOK;
}

uint8_t list1_delete(list1_t **list_ptr)
{
    list1_t *next = NULL;

    while (*list_ptr != NULL)
    {
        next = (*list_ptr)->next;
        free(*list_ptr);
        *list_ptr = next;
    }

    return EOK;
}

uint8_t list1_add(list1_t **list_ptr)
{
    list1_t *element = NULL;
    uint8_t err = EOK;

    if (*list_ptr == NULL)
    {
        err = list1_init(list_ptr);
        return err;
    }

    err = list1_init(&element);
    if (err)
        return err;

    element->next = *list_ptr;
    *list_ptr = element;
    return err;
}

uint8_t list1_add_tail(list1_t **list_ptr)
{
    list1_t *element = NULL;
    list1_t *last = *list_ptr;
    uint8_t err = EOK;

    if (*list_ptr == NULL)
    {
        err = list1_init(list_ptr);
        return err;
    }

    err = list1_init(&element);
    if (err)
        return err;

    while (last->next != NULL)
        last = last->next;

    (*list_ptr)->next = element;
    *list_ptr = element;
    return err;
}

list1_t *list1_get(list1_t *list_ptr, uint64_t ndx)
{
    list1_t *ret = list_ptr;
    for (int i = 0; i < ndx && ret != NULL; i++)
        ret = ret->next;
    return ret;
}

uint64_t list1_len(list1_t *list_ptr)
{
    int i = 0;
    for (list1_t *l = list_ptr; l != NULL; i++, l = l->next);
    return i;
}
