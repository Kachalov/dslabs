#include <stdlib.h>
#include "queue_arr.h"

#include "errors.h"

int queue_arr_init(queue_arr_p_t *q, size_t len, size_t size)
{
	int err = EOK;

    *q = malloc(sizeof(typeof(q)) + len * size);
    if (*q == NULL)
        err = EOOM;
    else
    {
        (*q)->data = (char *)*q + sizeof(typeof(q));
        (*q)->in = (*q)->data;
        (*q)->out = (*q)->data;
        (*q)->len = 0;
        (*q)->size = 0;
        (*q)->n = 0;
    }

    return err;
}

void queue_arr_delete(queue_arr_p_t *q)
{
    free(*q);
    *q = NULL;
}

int queue_arr_push(queue_arr_t *q)
{
    int err = EOK;

    return err;
}

int queue_arr_pop(queue_arr_t *q)
{
    int err = EOK;

    return err;
}
