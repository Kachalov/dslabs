#pragma once

#include <stdint.h>

#define HASH_POW_BASE 63
#define HASH_MAX_COLLISION 5

extern uint64_t *hash_pow;

void init_hash_pow(int n);
uint64_t hash(uint64_t *h, int l, int r);
uint64_t hash_str(char *h);
