#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash.h"
#include "hasho.h"
#include "lib/debug.h"
#include "lib/errors.h"

hoe_pt hoe_init(char *k , char *v)
{
    hoe_pt he = malloc(sizeof(hoe_t) + strlen(k) + strlen(v) + 2);
    if (he)
    {
        he->k = (char *)(he + 1);
        he->v = he->k + strlen(k) + 1;
        he->deleted = 0;

        strcpy(he->k, k);
        strcpy(he->v, v);
    }

    return he;
}

ho_pt ho_init(int n)
{
    ho_pt h = malloc(sizeof(ho_t) + sizeof(hoe_pt) * n);
    if (h)
    {
        h->data = (void *)(h + 1);
        h->n = n;
        h->els = 0;
        h->cells = 0;

        for (int i = 0; i < n; i++)
            h->data[i] = NULL;
    }

    return h;
}

uint64_t ho_hash(ho_pt h, char *k)
{
    uint64_t pow = 1;
    uint64_t hash = 0;

    for (char *i = k; *k; k++, pow *= HASH_POW_BASE)
        hash = (hash + *i * pow) % h->n;

    return hash;
}

int ho_add(ho_pt h, char *k, char *v)
{
    uint64_t hash = ho_hash(h, k);
    uint64_t hash_origin = hash;

    if (h->els + 1 > h->n)
        return EOOM;

    hoe_pt he = hoe_init(k, v);

    for (; h->data[hash] != NULL &&
           strcmp(h->data[hash]->k, k) != 0 &&
           h->data[hash]->deleted == 0;
         hash = (hash + 1) % h->n);

    if (!h->data[hash])
        h->els++;
    if (hash == hash_origin)
        h->cells++;

    h->data[hash] = he;
    DPRINT("HO ADD: hash(%s, %s) = %lu", k, v, hash);

    return EOK;
}

char *ho_get(ho_pt h, char *k)
{
    uint64_t hash = ho_hash(h, k);
    int i = 0;

    for (; h->data[hash] != NULL &&
           strcmp(h->data[hash]->k, k) != 0 &&
           i < h->n;
         hash = (hash + 1) % h->n);

    if (i == h->n)
        return NULL;

    return h->data[hash] && !h->data[hash]->deleted ? h->data[hash]->v : NULL;
}

void ho_del(ho_pt h, char *k)
{
    uint64_t hash = ho_hash(h, k);
    int i = 0;

    for (; h->data[hash] != NULL &&
           strcmp(h->data[hash]->k, k) != 0 &&
           i < h->n;
         hash = (hash + 1) % h->n, i++);

    if (i == h->n)
        return;

    if (h->data[hash] && !h->data[hash]->deleted)
    {
        h->data[hash]->deleted = 1;
    }
}

void ho_print(ho_pt h)
{
    for (int i = 0; i < h->n; i++)
        if (h->data[i] && !h->data[i]->deleted)
            fprintf(stderr, "%d: %s:%s\n", i, h->data[i]->k, h->data[i]->v);
        else if (h->data[i])
            fprintf(stderr, "%d: \033[7;31;31m DEL \033[0m\n", i);
}

void ho_restruct(ho_pt *h)
{
    int ns = primary(2 * (*h)->n + 1);
    ho_pt nh = ho_init(ns);

    for (int i = 0; i < (*h)->n; i++)
        if ((*h)->data[i] && !(*h)->data[i]->deleted)
            ho_add(nh, (*h)->data[i]->k, (*h)->data[i]->v);

    *h = nh;
}

float ho_efficiency(ho_pt h)
{
    return (float)h->els / h->cells;
}
