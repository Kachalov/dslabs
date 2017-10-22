#include <stdio.h>

#include "bits.h"

void set_bit(uint64_t *data, size_t ndx, bool val)
{
    if (val)
    {
        data[ndx / BITMASK_CHUNK] |=
            (uint64_t)1 << (ndx % BITMASK_CHUNK);
    }
    else
    {
        data[ndx / BITMASK_CHUNK] &=
            ~((uint64_t)1 << (ndx % BITMASK_CHUNK));
    }
}

bool get_bit(uint64_t *data, size_t ndx)
{
    return (data[ndx / BITMASK_CHUNK]) & ((uint64_t)1 << (ndx % BITMASK_CHUNK));
}

void print_bits(uint64_t *data)
{
    for (size_t i = 0; i <  BITMASK_SLOTS; i++)
    {
        for (size_t j = 0; j <  BITMASK_CHUNK; j++)
            printf("%d", get_bit(data, i * BITMASK_CHUNK + j));
        printf("\n");
    }
}
