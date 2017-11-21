#include <stdlib.h>
#include <assert.h>

#include "sparse_mtrx.h"
#include "lib/mtrx.h"
#include "lib/errors.h"

void smtrx_init(size_t m, size_t n, smtrx_pt mtrx_p)
{
    assert(mtrx_p != NULL);

    mtrx_p->m = m;
    mtrx_p->n = n;
    mtrx_p->a = NULL;
    mtrx_p->j = NULL;
    mtrx_p->r = NULL;
}

int smtrx_mul(smtrx_pt a, smtrx_pt b, smtrx_pt c_p)
{
    return EOK;
}

int smtrx_el(smtrx_pt m, int i, int j)
{
    int l = list1_get(m->r, i)->data;
    int r = list1_get(m->r, i + 1)->data;

    for (int k = l; k < r; k++)
        if ((list1_get(m->j, k)->data) == j)
            return list1_get(m->a, k)->data;
    return 0;
}

mtrx_data_i_t apply_mtrx_smtrx(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg)
{
    smtrx_pt smtrx_p = (smtrx_pt)arg;
    _smtrx_add(smtrx_p, i, j, mtrx_p->d[i][j]);
    return mtrx_p->d[i][j];
}

int mtrx_smtrx(mtrxp_t f, smtrx_pt t)
{
    smtrx_init(f->m, f->n, t);
    apply_mtrx(f, apply_mtrx_smtrx, t);

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
    list1_t *tmp;
    int x = 0;

    tmp = m->a;
    list1_add_tail(&tmp);
    tmp->data = data;

    tmp = m->j;
    list1_add_tail(&tmp);
    tmp->data = j;

    tmp = m->r;
    if (tmp == NULL)
    {
        list1_add_tail(&tmp);
        tmp->data = list1_len(m->j) - 1;

        list1_add_tail(&tmp);
        tmp->data = list1_len(m->j);
    }

    x = (list1_get(m->j, list1_len(m->j) - 1))->data;
    for (int a = i + 2 - list1_len(m->r); a > 0; a--)
    {
        list1_add_tail(&tmp);
        tmp->data = x;
    }

    tmp->data = i;

    return EOK;
}
