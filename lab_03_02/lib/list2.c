#include <stdlib.h>
#include <stdint.h>

#include "list2.h"
#include "debug.h"
#include "errors.h"

uint8_t list2_init(list2_t **list_ptr)
{
    list2_t *list = malloc(sizeof(list2_t));

    if (list == NULL)
    {
        return EOOM;
    }

    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;
    *list_ptr = list;

    return EOK;
}

uint8_t list2_delete(list2_t **list_ptr)
{
    list2_t *next = NULL;

    while (*list_ptr != NULL)
    {
        next = (*list_ptr)->next;
        free(*list_ptr);
        *list_ptr = next;
    }

    return EOK;
}

uint8_t list2_add(list2_t **list_ptr)
{
    list2_t *element = NULL;
    uint8_t err = EOK;

    if (*list_ptr == NULL)
    {
        err = list2_init(list_ptr);
        return err;
    }

    err = list2_init(&element);
    if (err)
        return err;

    element->prev = *list_ptr;
    (*list_ptr)->next = element;
    *list_ptr = element;
    return err;
}
