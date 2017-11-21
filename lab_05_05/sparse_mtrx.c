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
        int l = list1_get(m->r, f)->data;
        int r = list1_get(m->r, f + 1)->data;

        for (int k = l; k < r; k++)
        {
            tmp = list1_get(m->j, k)->data;
            if (use_next)
            {
                *i = f;
                *j = tmp;
                return list1_get(m->a, k)->data;
            }

            if (tmp == *j)
                use_next = 1;
        }
    }

    *i = m->m;
    *j = m->n;
    return 0;
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
    if (mtrx_p->d[i][j])
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
    printf("add(i=%d, j=%d, data=%d)\n", i, j, data);
    list1_t *tmp;

    tmp = m->a;
    list1_add_tail(&tmp);
    tmp->data = data;
    if (m->a == NULL)
        m->a = tmp;

    tmp = m->j;
    list1_add_tail(&tmp);
    tmp->data = j;
    if (m->j == NULL)
        m->j = tmp;

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
    tmp->data = list1_len(m->j) - 1;
    for (int a = i + 2 - list1_len(m->r); a > 0; a--)
    {
        list1_add_tail(&tmp);
        tmp->data = list1_len(m->j) - 1;
    }

    tmp->data = list1_len(m->j);

    return EOK;
}
