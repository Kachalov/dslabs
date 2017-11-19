#include <stdlib.h>
#include <stdint.h>

#include "list1.h"
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

uint8_t list1_delete_all(list1_t **list_ptr)
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

uint8_t list1_delete_head(list1_t **list_ptr)
{
    uint8_t err = EOK;
    list1_t *next = (*list_ptr)->next;

    free(*list_ptr);
    *list_ptr = next;

    return err;
}

void list1_delete_by_data(void *data)
{
    free((char *)data - (size_t)((list1_t *)0)->data);
}
