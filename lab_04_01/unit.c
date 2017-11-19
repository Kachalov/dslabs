#include <stdio.h>

#include "unit.h"
#include "queue_arr.h"
#include "queue_lst.h"
#include "list2.h"
#include "errors.h"

void operate(FILE *fout)
{
    queue_arr_p_t q_pre;
    queue_arr_init(&q_pre, MAX_POS, sizeof(unit_t));

    queue_lst_p_t q_main;
    queue_lst_init(&q_main, sizeof(unit_t));

    unit_t unit = {random_time(T3B, T3E), T2};
    queue_arr_push(q_pre, &unit);

    float time = 0;
    float time_t1 = random_time(T1B, T1E);

    /*float total_time = 0;
    float idle_time = 0;
    float avg_time = 0;
    float avg_len = 0;
    int units_in = 0;
    int units_out = 0;*/
    int units = 0;

    for (; units < UNITS; )
    {
        queue_arr_pop(q_pre, &unit);

        time += unit.t;

        if (unit.type == T2)
            queue_arr_push(q_pre, &unit);

        if (time_t1 <= time)
        {
            units++;
            time_t1 += random_time(T1B, T1E);
            unit.t = random_time(T2B, T2E);
            unit.type = T1;
            queue_lst_push(q_main, &unit);
        }

        for (int i = 0; i < MAX_POS && q_pre->n < q_pre->len && q_main->n > 0; i++)
        {
            if (queue_lst_pop(q_main, &unit) != EOK)
            {
                printf("FAILED POP!\n");
                break;
            }

            if (queue_arr_push(q_pre, &unit) != EOK)
            {
                printf("FAILED PUSH!\n");
                break;
            }
        }

        printf("[%zu] ", q_main->n);
        for (list2_t *i = q_main->pout; i != NULL; i = i->next)
        {
            printf("%p", (void *)i);
            if (i->next) printf(" -> ");
        }
        printf("\n");

        if (units % 100 == 0)
        {
            printf("time: %.2f\n", time);
        }
    }

    queue_arr_delete(&q_pre);
    queue_lst_delete(&q_main);
}

float random_time(float a, float b)
{
    return ((float)rand()/(float)(RAND_MAX)) * (a + b) - a;
}
