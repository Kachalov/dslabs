#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queue_arr.h"
#include "errors.h"

int queue_arr_init(queue_arr_p_t *q, size_t len, size_t size)
{
	int err = EOK;

    *q = malloc(sizeof(typeof(**q)) + len * size);
    if (*q == NULL)
        err = EOOM;
    else
    {
        (*q)->data = (typeof(**q) *)*q + 1;
        (*q)->pin = (*q)->data;
        (*q)->pout = (*q)->data;
        (*q)->len = len;
        (*q)->size = size;
        (*q)->n = 0;
    }

    return err;
}

void queue_arr_delete(queue_arr_p_t *q)
{
    free(*q);
    *q = NULL;
}

int queue_arr_push(queue_arr_t *q, void *data)
{
    int err = EOK;

    if (q->n >= q->len)
        err = EOOM;
    else
    {
        memcpy(q->pin, data, q->size);
        if ((char *)q->pin - (char *)q->data + q->size == q->size * q->len)
            q->pin = q->data;
        else
            q->pin = (char *)q->pin + q->size;
        q->n++;
    }

    return err;
}

int queue_arr_pop(queue_arr_t *q, void *ret)
{
    if (q->n > 0)
    {
        memcpy(ret, q->pout, q->size);
        if ((char *)q->pout - (char *)q->data + q->size == q->size * q->len)
            q->pout = q->data;
        else
            q->pout = (char *)q->pout + q->size;
        q->n--;

        return EOK;
    }

    return ENULL;
}
