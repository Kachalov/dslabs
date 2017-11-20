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

    unit_t unit = {
        .time = random_time(T3B, T3E),
        .time_added = 0,
        .type = T2
    };
    queue_lst_push(q, &unit);

    float time = 0;
    float time_t1 = random_time(T1B, T1E);

    float units_sum = 0;
    float time_sum = 0;

    int units_in = 0;
    int units_out = 0;
    int units_t1_out = 0;
    int units_t1_in = 0;
    int print_last = 0;


    printf("units:\t\tavg queue len:\t\tavg time:\n");
    while (units_t1_out < UNITS)
    {
        #ifdef DEBUG
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
        #endif

        units_sum += q->n;

        queue_lst_pop(q, &unit);

        time_sum += time - unit.time_added;
        time += unit.time;

        if (unit.type == T2)
        {
            units_in++;
            queue_lst_insert(q, MAX_POS - 1, &unit);
        }
        else
            units_t1_out++;
        units_out++;

        while (time_t1 <= time && units_t1_in < UNITS)
        {
            units_t1_in++;
            units_in++;
            time_t1 += random_time(T1B, T1E);
            unit.time_added = time - unit.time;
            unit.time = random_time(T2B, T2E);
            unit.type = T1;
            queue_lst_push(q, &unit);
        }

        if (units_t1_in % 100 == 0 && units_t1_in != print_last)
        {
            printf("%zu\t\t%.2f\t\t\t%.2f\n",
                   q->n, units_sum / units_out, time_sum / units_out);
            print_last = units_t1_in;
        }
    }

    printf("T1: in(%d) out(%d)\n", units_t1_in, units_t1_out);
    printf("T2: %d\n", units_out - units_t1_out);
    printf("Total time: %.2f Expected: %.2f-%0.2f\n",
           time_t1, UNITS * ((T1E - T1B)/2) * (97./100), UNITS * ((T1E - T1B)/2) * (103./100));

    queue_lst_delete(&q);
}

float random_time(float a, float b)
{
    return ((float)rand()/(float)(RAND_MAX)) * (a + b) - a;
}
