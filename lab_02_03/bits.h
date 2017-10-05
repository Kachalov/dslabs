#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define BITMASK_CHUNK 64
#define BITMASK_SLOTS 64

void set_bit(uint64_t *data, size_t ndx, bool val);
bool get_bit(uint64_t *data, size_t ndx);
void print_bits(uint64_t *data);
