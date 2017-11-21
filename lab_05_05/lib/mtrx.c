#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "mtrx.h"
#include "mtrx_apply.h"
#include "errors.h"

int alloc_mtrx(mtrx_size_t m, mtrx_size_t n, mtrxp_t *mtrx_pp,
               apply_mtrx_f_t f, void *arg)
{
    assert(*mtrx_pp == NULL);
    assert(m != 0 || n != 0);

    *mtrx_pp = (mtrxp_t)malloc(sizeof(mtrx_t) +
                               m * sizeof(mtrx_data_i_t *) + m * n * sizeof(mtrx_data_i_t));

    if (*mtrx_pp == NULL)
        return EOOM;

    (*mtrx_pp)->d = (mtrx_data_t)((char *)(*mtrx_pp) + sizeof(mtrx_t));
    (*mtrx_pp)->n = n;
    (*mtrx_pp)->m = m;

    for (int i = 0; i < m; i++)
    {
        (*mtrx_pp)->d[i] = (mtrx_data_i_t *)&(*mtrx_pp)->d[m] + i * n;
        for (int j = 0; j < n; j++)
            (*mtrx_pp)->d[i][j] = f != NULL ? f(*mtrx_pp, i, j, arg) : 0;
    }

    return EOK;
}

int free_mtrx(mtrxp_t *mtrx_pp)
{
    if (*mtrx_pp != NULL)
    {
        free(*mtrx_pp);
        *mtrx_pp = NULL;
    }
    return EOK;
}

void apply_mtrx(mtrxp_t mtrx_p, apply_mtrx_f_t f, void *arg)
{
    for (int i = 0; i < mtrx_p->m; i++)
        for (int j = 0; j < mtrx_p->n; j++)
            mtrx_p->d[i][j] = f(mtrx_p, i, j, arg);
}

int mul_mtrx(mtrxp_t a, mtrxp_t b, mtrxp_t *c_p)
{
    assert(a != NULL);
    assert(b != NULL);

    int err = EOK;

    if (a->n != b->m)
    {
        err = EMTRXSIZE;
    }
    else
    {
        err = alloc_mtrx(a->m, b->n, c_p, NULL, NULL);
        if (err == EOK)
        {
            for (int i = 0; i < a->m; i++)
                for (int j = 0; j < b->n; j++)
                    (*c_p)->d[i][j] = mul_i_mtrx(a, b, i, j);
        }
    }

    return err;
}

mtrx_data_i_t mul_i_mtrx(mtrxp_t a, mtrxp_t b, mtrx_size_t i, mtrx_size_t j)
{
    mtrx_data_i_t r = 0;
    for (int k = 0; k < a->n; k++)
        r += a->d[i][k] * b->d[k][j];
    return r;
}
