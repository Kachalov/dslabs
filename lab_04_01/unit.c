#include <stdio.h>

#include "unit.h"
#include "queue_arr.h"
#include "queue_lst.h"
#include "list2.h"
#include "errors.h"

void operate(FILE *fout)
{
    queue_lst_p_t q;
    queue_lst_init(&q, sizeof(unit_t));

    unit_t unit = {random_time(T3B, T3E), T2};
    queue_lst_push(q, &unit);

    float time = 0;
    float time_t1 = random_time(T1B, T1E);

    /*float total_time = 0;
    float idle_time = 0;
    float avg_time = 0;
    float avg_len = 0;
    int units_in = 0;*/
    int units_out = 0;
    int units = 0;

    for (; units_out <= UNITS; )
    {
        printf("[%zu] ", q->n);
        for (list2_t *i = q->pout; i != NULL; i = i->next)
        {
            printf("[%d %.2f](%p)",
                ((unit_t *)i->data)->type,
                ((unit_t *)i->data)->t,
                (void *)i);
            if (i->next) printf(" -> ");
        }
        printf("\n");

        queue_lst_pop(q, &unit);

        time += unit.t;

        if (unit.type == T2)
            queue_lst_insert(q, MAX_POS - 1, &unit);
        else
            units_out++;

        if (time_t1 <= time && units <= UNITS)
        {
            units++;
            time_t1 += random_time(T1B, T1E);
            unit.t = random_time(T2B, T2E);
            unit.type = T1;
            queue_lst_push(q, &unit);
        }

        if (units % 100 == 0)
            printf("units: %d time: %.2f\n", units, time);
    }

    queue_lst_delete(&q);
}

float random_time(float a, float b)
{
    return ((float)rand()/(float)(RAND_MAX)) * (a + b) - a;
}
