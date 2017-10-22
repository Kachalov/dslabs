#include <stdlib.h>
#include <stdint.h>

#include "list.h"
#include "debug.h"
#include "errors.h"

uint8_t create_list(list_t **list_ptr)
{
    list_t *list = malloc(sizeof(list_t));

    if (list == NULL)
    {
        return EOOM;
    }

    list->data = NULL;
    list->next = NULL;
    *list_ptr = list;

    return EOK;
}

uint8_t delete_list(list_t **list_ptr)
{
    list_t *next = NULL;

    while (*list_ptr != NULL)
    {
        next = (*list_ptr)->next;
        free(*list_ptr);
        *list_ptr = next;
    }

    return EOK;
}

uint8_t add_element(list_t **list_ptr)
{
    list_t *element = NULL;
    uint8_t err = EOK;

    if (*list_ptr == NULL)
    {
        err = create_list(list_ptr);
        return err;
    }

    err = create_list(&element);
    if (err)
        return err;

    element->next = *list_ptr;
    *list_ptr = element;
    return err;
}
