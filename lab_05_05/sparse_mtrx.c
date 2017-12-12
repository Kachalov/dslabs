#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <math.h>

#include "sparse_mtrx.h"
#include "lib/mtrx.h"
#include "lib/debug.h"
#include "lib/errors.h"
#include "lib/time.h"

extern tick_t ticks;

int smtrx_init(size_t m, size_t n, size_t elements, smtrx_pt *mtrx_pp)
{
    assert(mtrx_pp != NULL);

    *mtrx_pp = malloc(sizeof(smtrx_t));
    if (*mtrx_pp == NULL)
        goto oom;

    (*mtrx_pp)->m = m;
    (*mtrx_pp)->n = n;
    (*mtrx_pp)->a = calloc(elements, sizeof(*(*mtrx_pp)->a));
    if ((*mtrx_pp)->a == NULL)
        goto oom;

    (*mtrx_pp)->j = calloc(elements, sizeof(*(*mtrx_pp)->j));
    if ((*mtrx_pp)->j == NULL)
        goto oom;

    (*mtrx_pp)->r = NULL;
    (*mtrx_pp)->els = elements;
    (*mtrx_pp)->len = 0;

    return EOK;

    oom:
    if (*mtrx_pp != NULL)
        free(*mtrx_pp);

    if ((*mtrx_pp)->a == NULL)
        free((*mtrx_pp)->a);

    if ((*mtrx_pp)->j == NULL)
        free((*mtrx_pp)->j);

    return EOOM;
}

void smtrx_delete(smtrx_pt *mtrx_pp)
{
    if (*mtrx_pp)
        free(*mtrx_pp);
    *mtrx_pp = NULL;
}

int smtrx_mul(smtrx_pt a, smtrx_pt b, smtrx_pt *c)
{
    *c = NULL;
    tick_t at;

    // Only vector
    if (a->m != 1)
        return EMTRXSIZE;

    if (a->n != b->m)
        return EMTRXSIZE;

    int i = 0;
    int j = 0;

    if (a->len == 0 || b->len == 0)
    {
        at = tick();
        smtrx_init(a->m, b->n, 0, c);
        ticks = tick() - at;
        return EOK;
    }

    at = tick();
    list1_t *la = list1_get(a->r, 0);
    list1_t *ra = la->next == NULL ? NULL : la->next;
    list1_t *lb = list1_get(b->r, a->j[la->data]);
    list1_t *rb = lb->next == NULL ? NULL : lb->next;

    float *res = malloc(b->n * sizeof(*a->a));
    for (int j = 0; j < b->n; j++)
        res[j] = 0;

    {
        for (int ai = la->data; ai < ra->data; ai++)
        {
            if (lb == NULL || rb == NULL)
                break;

            i = a->j[ai];
            for (int bj = lb->data; bj < rb->data; bj++)
            {
                j = b->j[bj];
                res[j] += a->a[ai] * b->a[bj];
            }

            for (int _i = 0; _i < a->j[ai + 1] - i; _i++)
            {
                lb = rb;
                rb = lb->next == NULL ? NULL : lb->next;
            }
        }
    }
    ticks = tick() - at;

    int els = 0;
    for (int i = 0; i < b->n; i++)
        if (fabsf(res[i]) > 1e-7)
            els++;
    smtrx_init(a->m, b->n, els, c);

    for (int i = 0; i < b->n; i++)
        if (fabsf(res[i]) > 1e-7)
            _smtrx_add(*c, 0, i, res[i]);
    free(res);

    return EOK;
}

float smtrx_el(smtrx_pt m, int i, int j)
{
    list1_t *l = list1_get(m->r, i);

    if (l == NULL)
        return 0;

    list1_t *r = l->next;

    if (r == NULL)
        return 0;

    int end = r->data;
    for (int k = l->data; k < end; k++)
        if (m->j[k] == j)
            return m->a[k];
    return 0;
}

mtrx_data_i_t apply_mtrx_smtrx(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg)
{
    smtrx_pt smtrx_p = (smtrx_pt)arg;
    if (mtrx_p->d[i][j])
        _smtrx_add(smtrx_p, i, j, mtrx_p->d[i][j]);
    return mtrx_p->d[i][j];
}

mtrx_data_i_t apply_mtrx_mtrx(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg)
{
    return smtrx_el((smtrx_pt)arg, i, j);
}

mtrx_data_i_t apply_mtrx_smtrx_count(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg)
{
    int *els = (int *)arg;
    if (mtrx_p->d[i][j])
        (*els)++;
    return mtrx_p->d[i][j];
}

int mtrx_smtrx(mtrxp_t f, smtrx_pt *t_p)
{
    int els = 0;

    apply_mtrx(f, apply_mtrx_smtrx_count, &els);
    smtrx_init(f->m, f->n, els, t_p);
    apply_mtrx(f, apply_mtrx_smtrx, *t_p);

    return EOK;
}

int smtrx_mtrx(smtrx_pt f, mtrxp_t *t_p)
{
    alloc_mtrx(f->m, f->n, t_p, apply_mtrx_mtrx, f);

    return EOK;
}

float smtrx_sparse(smtrx_pt m)
{
    return 1 - (float)m->len/m->m/m->n;
}

/**
 * @attention Use only for packing
 * @param m
 * @param i
 * @param j
 * @param data
 * @return Error code
 */
int _smtrx_add(smtrx_pt m, int i, int j, float data)
{
    DPRINT("add(i=%d, j=%d, data=%.2f)\n", i, j, data);
    m->a[m->len] = data;
    m->j[m->len] = j;
    m->len++;

    list1_t *tmp = NULL;
    tmp = m->r;
    if (tmp == NULL)
    {
        list1_add_tail(&tmp);
        tmp->data = 0;
        m->r = tmp;

        list1_add_tail(&tmp);
        tmp->data = 1;
    }

    tmp = list1_get(m->r, list1_len(m->r) - 1);
    tmp->data = m->len - 1;
    for (int a = i + 2 - list1_len(m->r); a > 0; a--)
    {
        list1_add_tail(&tmp);
        tmp->data = m->len - 1;
    }

    tmp->data = m->len;

    return EOK;
}
