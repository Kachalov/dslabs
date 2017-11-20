#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "list2.h"
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

uint8_t list2_delete_el(list2_t **list_ptr)
{
    if ((*list_ptr)->prev != NULL)
        (*list_ptr)->prev->next = (*list_ptr)->next;

    if ((*list_ptr)->next != NULL)
        (*list_ptr)->next->prev = (*list_ptr)->prev;

    free(*list_ptr);
    *list_ptr = NULL;

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

uint8_t list2_insert(size_t ndx, list2_t **list_ptr)
{
    uint8_t err = EOK;
    list2_t *list = *list_ptr;
    list2_t *next = NULL;

    for (size_t i = 0; i <= ndx && list->next != NULL; i++)
    {
        list = list->next;
    }
    next = list;
    list = next->prev;

    if ((err = list2_add(&list)) != EOK)
        return err;

    if (next != NULL)
    {
        next->prev = list;
        list->next = next;
    }

    *list_ptr = list;

    return err;
}

void list2_delete_by_data(void *data)
{
    list2_t *list = (list2_t *)((char *)data - (size_t)((list2_t *)0));
    list2_delete_el(&list);
}
