#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>

#include "mtrx.h"
#include "mtrx_apply.h"
#include "errors.h"
#include "time.h"

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

void sprint_mtrx(mtrxp_t mtrx_p, char *str)
{
    assert(str != NULL);

    strcpy(str, "");
    if (mtrx_p == NULL)
        return;

    for (int i = 0; i < mtrx_p->m; i++)
    {
        for (int j = 0; j < mtrx_p->n; j++)
        {
            sprintf(str + strlen(str), "%.3f", mtrx_p->d[i][j]);
            if (j != mtrx_p->n - 1)
                sprintf(str + strlen(str), " ");
        }
        sprintf(str + strlen(str), "\n");
    }
}

void print_mtrx(mtrxp_t mtrx_p)
{
    char buf[4096] = "";
    sprint_mtrx(mtrx_p, buf);
    printf("%s", buf);
}

int fprint_mtrx(mtrxp_t mtrx_p, char *fn)
{
    assert(fn != NULL);

    char buf[4096] = "";
    int err = EOK;
    FILE *f = NULL;

    f = fopen(fn, "w");
    if (f == NULL)
        err = ENOFILE;

    if (err == EOK)
    {
        sprint_mtrx(mtrx_p, buf);
        fprintf(f, "%s", buf);
        fclose(f);
    }

    return err;
}

int sum_mtrx(mtrxp_t a, mtrxp_t b, mtrxp_t *c_p)
{
    assert(a != NULL);
    assert(b != NULL);

    int err = EOK;

    if (a->m != b->m || a->n != b->n)
    {
        err = EMTRXSIZE;
    }
    else
    {
        err = alloc_mtrx(a->m, a->n, c_p, NULL, NULL);
        if (err == EOK)
        {
            for (int i = 0; i < a->m; i++)
                for (int j = 0; j < a->n; j++)
                    (*c_p)->d[i][j] = a->d[i][j] + b->d[i][j];
        }
    }

    return err;
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
            tick_t at = tick();
            for (int i = 0; i < a->m; i++)
                for (int j = 0; j < b->n; j++)
                    (*c_p)->d[i][j] = mul_i_mtrx(a, b, i, j);
            printf("Ticks MTRX: %"PRIu64"\n", tick() - at);
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

void swap_col_mtrx(mtrxp_t a, mtrx_size_t i, mtrx_size_t j)
{
    mtrx_data_i_t tmp;
    for (int k = 0; k < a->m; k++)
    {
        tmp = a->d[k][i];
        a->d[k][i] = a->d[k][j];
        a->d[k][j] = tmp;
    }
}

mtrx_sizes_t size_file_mtrx(FILE *f)
{
    mtrx_sizes_t ss;
    ss.m = 0;
    ss.n = 0;
    mtrx_data_i_t tmp = 0;
    char c;
    int i = 0;
    fpos_t pos;

    fgetpos(f, &pos);
    while (!feof(f))
    {
        if ((c = fgetc(f)) == '\n')
        {
            ss.m++;
            i = 0;
        }
        else
        {
            ungetc(c, f);
            if (fscanf(f, "%f", &tmp) == 1)
            {
                ss.n = ++i > ss.n ? i : ss.n;
            }
            else
            {
                if (feof(f))
                    break;

                // EFORMAT case
                ss.m = 1;
                ss.n = 0;
                break;
            }
        }
    }

    fsetpos(f, &pos);
    return ss;
}

int read_mtrx(char *fn, mtrxp_t *m)
{
    int err = EOK;
    FILE *f = fopen(fn, "r");
    if (f == NULL)
        err = ENOFILE;

    if (err == EOK)
    {
        mtrx_sizes_t ss = size_file_mtrx(f);
        if (ss.m == 0 && ss.n == 0)
        {
            err = ENODATA;
        }
        else if (ss.m + ss.n == 1)
        {
            err = EFORMAT;
        }
        else
        {
            err = alloc_mtrx(ss.m, ss.n, m, NULL, NULL);

            ss.m = 0;
            ss.n = 0;
            char c;
            mtrx_data_i_t tmp = 0;

            if (err == EOK)
            {
                while (!feof(f))
                {
                    if ((c = fgetc(f)) == '\n')
                    {
                        ss.m++;
                        ss.n = 0;
                    }
                    else
                    {
                        ungetc(c, f);
                        if (fscanf(f, "%f", &tmp) == 1)
                        {
                            (*m)->d[ss.m][ss.n++] = tmp;
                        }
                        else
                        {
                            if (!feof(f))
                            {
                                free_mtrx(m);
                                err = EFORMAT;
                            }
                            break;
                        }
                    }
                }
            }
        }
        fclose(f);
    }

    return err;
}
