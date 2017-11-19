#include <stdlib.h>
#include <string.h>

#include "list1.h"
#include "queue_lst.h"
#include "errors.h"

int queue_lst_init(queue_lst_p_t *q, size_t size)
{
    int err = EOK;

    *q = malloc(sizeof(typeof(**q)));
    if (*q == NULL)
        err = EOOM;
    else
    {
        (*q)->pin = NULL;
        (*q)->pout = NULL;
        (*q)->size = size;
        (*q)->n = 0;
    }

    return err;
}

void queue_lst_delete(queue_lst_p_t *q)
{
    list1_delete_all((*q)->pin);
    free(*q);
    *q = NULL;
}

int queue_lst_push(queue_lst_t *q, void *data)
{
    int err = EOK;

    list1_add(q->pin);
    memcpy(q->pin, data, q->size);
    if (q->pout == NULL)
        q->pout = q->pin;
    q->n++;

    return err;
}

void *queue_lst_pop(queue_lst_t *q)
{
    void *ret = NULL;

    if (q->n != 0)
    {
        ret = ((list1_t *)(q->pout))->data;
        q->pout = ((list1_t *)(q->pout))->next;
        q->n--;
    }

    return ret;
}
