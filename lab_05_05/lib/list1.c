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

    list->data = NULL;
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
