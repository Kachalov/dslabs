#include <stdlib.h>
#include <stdint.h>

#include "list.h"
#include "debug.h"
#include "errors.h"

int list_init(list1_t **head)
{
    int err = EOK;
    *head = malloc(sizeof(list1_t));

    if (!*head)
        err = EOOM;
    else
    {
        (*head)->data = NULL;
        (*head)->next = NULL;
    }

    return err;
}

void list_free(list1_t **head)
{
    while (pop_front(head));
}

void delete_node_before(list1_t **head, list1_t *before)
{
    if (before)
    {
        list1_t *del = before->next;
        before->next = del ? del->next : NULL;
        pop_front(&del);
    }
    else
    {
        pop_front(head);
    }
}

void *pop_front(list1_t **head)
{
    void *ret = NULL;
    list1_t *tmp = NULL;

    if (*head)
    {
        ret = (*head)->data;
        tmp = (*head)->next;
        free(*head);
        *head = tmp;
    }

    return ret;
}

void list_insert(list1_t **head, list1_t *elem, list1_t *before)
{
    list1_t *cur = *head;
    list1_t *prev = NULL;

    elem->next = NULL;

    for (; cur && cur != before; prev = cur, cur = cur->next);

    if (cur)
    {
        elem->next = cur->next;
        cur->next = elem;
    }
    else if (!before && prev) // End of non-empty list
    {
        prev->next = elem;
    }
    else if (!before) // Empty list
    {
        *head = elem;
    }
}

int insert_data(list1_t **head, void *data)
{
    int err = EOK;
    list1_t *el = NULL;

    err = list_init(&el);
    el->data = data;
    list_insert(head, el, NULL);

    return err;
}

list1_t *reverse(list1_t *head)
{
    list1_t *ret = NULL;
    list1_t *cur = head;
    list1_t *before = NULL;

    while (cur)
    {
        before = cur->next;
        cur->next = ret;
        ret = cur;
        cur = before;
    }

    return ret;
}

list1_t *sort(list1_t *head, int (*cmp)(const void *, const void *))
{
    list1_t *ret = NULL;
    list1_t *itn = NULL;

    for (list1_t *it = head; it;)
    {
        itn = it->next;
        sorted_insert(&ret, it, cmp);
        #ifdef DEBUG
            DPRINT("list: ");
            list_print_int(ret);
        #endif
        it = itn;
    }

    return ret;
}

void sorted_insert(list1_t **head, list1_t *el,
                   int (*cmp)(const void *, const void *))
{
    list1_t *prev = NULL;

    for (list1_t *it = *head;
         it && cmp(it->data, el->data) <= 0;
         prev = it, it = it->next);

    DPRINT("sorted_insert:prev = %p", (void *)prev);

    if (*head && !prev)
    {
        el->next = *head;
        *head = el;
    }
    else
        list_insert(head, el, prev);
}
