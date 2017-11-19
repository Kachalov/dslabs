#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list2.h"
#include "queue_lst.h"
#include "errors.h"

int queue_lst_init(queue_lst_p_t *q, size_t size)
{
    int err = EOK;

    *q = malloc(sizeof(__typeof__(**q)));
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
    while(queue_lst_pop(*q, NULL) != ENULL);
    //list2_delete((list2_t **)&(*q)->pin);
    free(*q);
    *q = NULL;
}

int queue_lst_push(queue_lst_t *q, void *data)
{
    int err = EOK;
    list2_t *lst = NULL;

    err = list2_add((list2_t **)&q->pin);
    if (err == EOK)
    {
        ((list2_t *)q->pin)->data = malloc(q->size);
        if (((list2_t *)q->pin)->data == NULL)
        {
            err = EOOM;
            lst = ((list2_t *)(q->pin))->next;
            list2_delete_el((list2_t **)&q->pin);
            q->pin = lst;
        }
        else
        {
            memcpy(((list2_t *)q->pin)->data, data, q->size);
            if (q->pout == NULL)
                q->pout = q->pin;
            q->n++;
        }
    }

    return err;
}

int queue_lst_insert(queue_lst_t *q, size_t pos, void *data)
{
    // TODO
    return queue_lst_push(q, data);
}

int queue_lst_pop(queue_lst_t *q, void *ret)
{
    list2_t *lst = NULL;

    if (q->n != 0)
    {
        if (ret != NULL)
        {
            memcpy(ret, ((list2_t *)(q->pout))->data, q->size);
        }

        if (((list2_t *)(q->pout)) == q->pin)
            q->pin = NULL;

        free(((list2_t *)(q->pout))->data);
        lst = ((list2_t *)(q->pout))->next;
        list2_delete_el((list2_t **)&q->pout);
        q->pout = lst;
        q->n--;

        return EOK;
    }

    return ENULL;
}
