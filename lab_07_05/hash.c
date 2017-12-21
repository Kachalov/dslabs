#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "hash.h"

uint64_t *hash_pow = NULL;
int hash_pow_len = 0;

void init_hash_pow(int n)
{
    if (hash_pow)
        free(hash_pow);

    hash_pow = malloc(sizeof(uint64_t) * n);
    hash_pow_len = n;

    hash_pow[0] = 1;
    for (int i = 1; i <= n; i++)
        hash_pow[i] = hash_pow[i - 1] * HASH_POW_BASE;
}

uint64_t hash(uint64_t *h, int l, int r)
{
    uint64_t ret = h[r];

    if (l > 0)
        ret -= h[l - 1];

    return ret;
}

uint64_t hash_str(char *h)
{
    static uint64_t hs[1000];
    int len = strlen(h);

    hs[0] = h[0];
    for (int i = 1; i < len; i++)
        hs[i] = hs[i - 1] + hash_pow[i] * h[i];

    return hash(hs, 0, len);
}
