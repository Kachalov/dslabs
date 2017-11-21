#include <stdio.h>

#include "unit.h"
#include "queue_arr.h"
#include "queue_lst.h"
#include "list2.h"
#include "errors.h"

void operate(FILE *fout, int verbose)
{
    int err = EOK;

    QUEUE_P_TYPE q;
    QUEUE_INIT(q);

    unit_t unit = {
        .time = random_time(T3B, T3E),
        .time_added = 0,
        .type = T2
    };
    QUEUE_PUSH(q, unit);

    float time = 0;
    float rnd = 0;
    float time_t1 = random_time(T1B, T1E);

    float units_sum = 0;
    float time_sum = 0;

    float time_idle = 0;

    int units_in = 0;
    int units_out = 0;
    int units_t1_out = 0;
    int units_t1_in = 0;
    int print_last = 0;


    #ifdef QUEUE_LIST
        printf("Queue on list\n");
    #else
        printf("Queue on array\n");
    #endif
    printf("units:\t\tavg queue len:\t\tavg time:\n");
    while (units_t1_out < UNITS)
    {
        #ifdef DEBUG
            #ifdef QUEUE_LIST
                printf("[%zu] ", q->n);
                for (list2_t *i = q->pout; i != NULL; i = i->next)
                {
                    printf("[%d %.2f:%.2f](%p)",
                        ((unit_t *)i->data)->type,
                        ((unit_t *)i->data)->time_added,
                        ((unit_t *)i->data)->time,
                        (void *)i);
                    if (i->next) printf(" -> ");
                }
                printf("\n");
            #endif
        #endif

        units_sum += q->n;
        rnd = random_time(T1B, T1E);

        err = QUEUE_POP(q, unit);
        if (err == EOK)
        {
            time_sum += time - unit.time_added;
            time += unit.time;

            if (unit.type == T2)
            {
                units_in++;
                unit.time_added = time - unit.time;
                QUEUE_INSERT(q, unit);
            }
            else
                units_t1_out++;
            units_out++;
        }
        else
        {
            time_idle += rnd;
            time += rnd;
        }

        if (time_t1 <= time && units_t1_in < UNITS)
        {
            units_t1_in++;
            units_in++;
            time_t1 += rnd;
            unit.time_added = time_t1;
            unit.time = random_time(T2B, T2E);
            unit.type = T1;
            QUEUE_PUSH(q, unit);
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
    printf("Time idle: %.2f\n", time_idle);

    QUEUE_DELETE(q);
}

float random_time(float a, float b)
{
    return ((float)rand()/(float)(RAND_MAX)) * (a + b) - a;
}
