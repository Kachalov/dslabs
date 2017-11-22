#include <stdlib.h>
#include <assert.h>

#include "sparse_mtrx.h"
#include "lib/mtrx.h"
#include "lib/debug.h"
#include "lib/errors.h"

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

    (*mtrx_pp)->r = calloc(m + 1, sizeof(*(*mtrx_pp)->r));
    if ((*mtrx_pp)->r == NULL)
        goto oom;

    (*mtrx_pp)->els = elements;
    (*mtrx_pp)->rs = 0;
    (*mtrx_pp)->len = 0;

    return EOK;

    oom:
    if (*mtrx_pp != NULL)
        free(*mtrx_pp);

    if ((*mtrx_pp)->a == NULL)
        free((*mtrx_pp)->a);

    if ((*mtrx_pp)->j == NULL)
        free((*mtrx_pp)->j);

    if ((*mtrx_pp)->r == NULL)
        free((*mtrx_pp)->r);

    return EOOM;
}

void smtrx_delete(smtrx_pt *mtrx_pp)
{
    if (*mtrx_pp)
        free(*mtrx_pp);
    *mtrx_pp = NULL;
}

int smtrx_next(smtrx_pt m, int *i, int *j)
{
    int tmp = 0;
    int use_next = 0;

    if (*i < 0 || *j < 0)
    {
        *i = 0;
        *j = -1;
        use_next = 1;
    }

    for (int f = *i; f < *i + 2; f++)
    {
        int l = m->r[f];
        int r = m->r[f + 1];

        for (int k = l; k < r; k++)
        {
            tmp = m->j[k];
            if (use_next)
            {
                *i = f;
                *j = tmp;
                return m->a[k];
            }

            if (tmp == *j)
                use_next = 1;
        }
    }

    *i = m->m;
    *j = m->n;
    return 0;
}

int smtrx_mul(smtrx_pt a, smtrx_pt b, smtrx_pt *c)
{
    *c = NULL;

    // Only vector
    if (a->m != 1)
        return EMTRXSIZE;

    if (a->n != b->m)
        return EMTRXSIZE;

    smtrx_init(a->m, b->n, a->n, c);

    int *el = NULL;

    int ai = -1;
    int aj = -1;
    for (int av = smtrx_next(a, &ai, &aj); ai != a->m; av = smtrx_next(a, &ai, &aj))
    {
        for (int bj = 0; bj < b->n; bj++)
        {
            int bv = smtrx_el(b, aj, bj);
            if ((el = smtrx_el_list(*c, ai, bj)) == NULL)
            {
                _smtrx_add(*c, ai, bj, av * bv);
            }
            else
            {
                *el += av * bv;
            }
        }
    }

    return EOK;
}

int smtrx_el(smtrx_pt m, int i, int j)
{
    int *list = smtrx_el_list(m, i, j);
    if (list)
        return *list;
    return 0;
}

int *smtrx_el_list(smtrx_pt m, int i, int j)
{
    int l = m->r[i];
    int r = m->r[i + 1];

    for (int k = l; k < r; k++)
        if (m->j[k] == j)
            return m->a + k;
    return NULL;
}

mtrx_data_i_t apply_mtrx_smtrx(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg)
{
    smtrx_pt smtrx_p = (smtrx_pt)arg;
    if (mtrx_p->d[i][j])
        _smtrx_add(smtrx_p, i, j, mtrx_p->d[i][j]);
    return mtrx_p->d[i][j];
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

/**
 * @attention Use only for packing
 * @param m
 * @param i
 * @param j
 * @param data
 * @return Error code
 */
int _smtrx_add(smtrx_pt m, int i, int j, int data)
{
    DPRINT("add(i=%d, j=%d, data=%d)\n", i, j, data);
    m->a[m->len] = data;
    m->j[m->len] = j;

    if (!m->rs)
    {
        m->r[0] = 0;
        m->r[1] = 1;
        m->rs = 2;
    }
    m->len++;

    m->r[m->rs - 1] = m->len - 1;
    for (int a = i + 1 - m->rs; a >= 0; a--)
    {
        m->r[m->rs - 1 + a] = m->len - 1;
        m->rs++;
    }

    m->r[m->rs - 1] = m->len;

    return EOK;
}
