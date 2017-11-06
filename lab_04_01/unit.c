#include "unit.h"
#include "queue.h"

void operate(FILE *fout)
{
    queue_t q1;
    queue_t q2;

    queue_init(q1, 4, sizeof(unit_t));
    queue_init(q1, QUEUE_SIZE - 4, sizeof(unit_t));

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
