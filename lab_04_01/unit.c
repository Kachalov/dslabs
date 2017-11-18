#include "unit.h"
#include "queue_arr.h"
#include "queue_lst.h"

void operate(FILE *fout)
{
    queue_arr_p_t q_pre;
    queue_lst_p_t q_main;

    queue_arr_init(&q_pre, 4, sizeof(unit_t));
    queue_lst_init(&q_main, sizeof(unit_t));

    float time = 0;
    float time_t1 = 0;

    float total_time = 0;
    float idle_time = 0;
    float avg_time = 0;
    float avg_len = 0;
    int units_in = 0;
    int units_out = 0;
    int units = 0;

    float rnd = 0;

    do
    {
        // TODO
    }
    while (1);
}

float random_time(float a, float b)
{
    return ((float)rand()/(float)(RAND_MAX)) * (a + b) - a;
}
