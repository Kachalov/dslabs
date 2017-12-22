#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash.h"
#include "hashc.h"
#include "lib/errors.h"
#include "lib/list.h"

int hc_cmps = 0;

hce_pt hce_init(char *k , char *v)
{
    hce_pt he = malloc(sizeof(hce_t) + strlen(k) + strlen(v) + 2);
    if (he)
    {
        he->k = (char *)(he + 1);
        he->v = he->k + strlen(k) + 1;

        strcpy(he->k, k);
        strcpy(he->v, v);
    }

    return he;
}

hc_pt hc_init(int n)
{
    hc_pt h = malloc(sizeof(hc_t) + sizeof(hce_pt) * n);
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

uint64_t hc_hash(hc_pt h, char *k)
{
    uint64_t pow = 1;
    uint64_t hash = 0;

    for (char *i = k; *k; k++, pow *= HASH_POW_BASE)
        hash = (hash + *i * pow) % h->n;

    return hash;
}

int hc_add(hc_pt h, char *k, char *v)
{
    uint64_t hash = hc_hash(h, k);

    h->els++;
    if (!h->data[hash])
        h->cells++;

    hce_pt he = hce_init(k, v);

    for_each(it, h->data[hash])
    {
        hc_cmps++;
        if (strcmp(((hce_pt)it->data)->k, k) == 0)
        {
            it->data = he;
            return EOK;
        }
    }

    return insert_data(&h->data[hash], he);
}

char *hc_get(hc_pt h, char *k)
{
    uint64_t hash = hc_hash(h, k);

    for_each(it, h->data[hash])
    {
        hc_cmps++;
        if (strcmp(((hce_pt)it->data)->k, k) == 0)
            return ((hce_pt)it->data)->v;
    }

    return NULL;
}

void hc_del(hc_pt h, char *k)
{
    uint64_t hash = hc_hash(h, k);
    list1_t *prev = NULL;

    for_each(it, h->data[hash])
    {
        hc_cmps++;
        if (strcmp(((hce_pt)it->data)->k, k) == 0)
        {
            delete_node_before(&h->data[hash], prev);
            h->els--;
            if (!h->data[hash])
                h->cells--;
            return;
        }
        prev = it;
    }
}

void hc_print(hc_pt h)
{
    for (int i = 0; i < h->n; i++)
    {
        if (!h->data[i])
            continue;

        fprintf(stderr, "%d: ", i);
        for_each(it, h->data[i])
            fprintf(stderr, "%s:%s, ", ((hce_pt)it->data)->k, ((hce_pt)it->data)->v);
        fprintf(stderr, "\n");
    }
}

void hc_restruct(hc_pt *h)
{
    int ns = primary(2 * (*h)->n + 1);
    hc_pt nh = hc_init(ns);

    for (int i = 0; i < (*h)->n; i++)
    {
        if (!(*h)->data[i])
            continue;

        for_each(it, (*h)->data[i])
            hc_add(nh, ((hce_pt) it->data)->k, ((hce_pt) it->data)->v);
    }

    *h = nh;
}

float hc_efficiency(hc_pt h)
{
    return (float)h->els / h->cells;
}
