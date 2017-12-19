#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hasho.h"
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

int ho_hash(ho_pt h, char *k)
{
    int hash = k[0] % h->n;

    return hash;
}

int ho_add(ho_pt h, char *k, char *v)
{
    int hash = ho_hash(h, k);

    if (h->els + 1 > h->n)
        return EOOM;

    h->els++;
    if (!h->data[hash])
        h->cells++;

    hoe_pt he = hoe_init(k, v);

    for (; h->data[hash] != NULL &&
           strcmp(h->data[hash]->k, k) != 0 &&
           h->data[hash]->deleted != 0;
         hash = (hash + 1) % h->n);

    h->data[hash] = he;

    return EOK;
}

char *ho_get(ho_pt h, char *k)
{
    int hash = ho_hash(h, k);

    for (; h->data[hash] != NULL &&
           strcmp(h->data[hash]->k, k) != 0;
         hash = (hash + 1) % h->n);

    return h->data[hash] ? h->data[hash]->v : NULL;
}

void ho_del(ho_pt h, char *k)
{
    int hash = ho_hash(h, k);
    char *v = ho_get(h, k);

    if (v)
    {
        h->els--;
        if (!h->data[hash])
            h->cells--;
    }
}

void ho_print(ho_pt h)
{

}
