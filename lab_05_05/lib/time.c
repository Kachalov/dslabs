#include "time.h"

tick_t ticks = 0;

tick_t tick(void)
{
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((tick_t)hi << 32) | lo;
}
