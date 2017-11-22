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
    int border = 0;

    list1_t *lb = list1_get(b->r, 0);
    list1_t *rb = list1_get(b->r, 1);
    for (int i = 0; i < a->els; i++)
    {
        border = rb->data - lb->data;
        lb = lb->next;
        rb = rb->next;
        for (int j = 0; j < border; j++)
        {
            printf("%d %d %d %d\n", i, a->j[i], j, b->j[j]);
            if ((el = smtrx_el_list(*c, 0, j)) == NULL)
            {
                _smtrx_add(*c, 0, j, a->a[i] * smtrx_el(b, a->j[i], b->j[j]));
            }
            else
            {
                *el += a->a[i] * smtrx_el(b, a->j[i], b->j[j]);
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
    list1_t *l = list1_get(m->r, i);
    list1_t *r = list1_get(m->r, i + 1);

    if (l == NULL || r == NULL)
        return NULL;

    for (int k = l->data; k < r->data; k++)
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
    DPRINT("add(i=%d, j=%d, data=%d)", i, j, data);
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
