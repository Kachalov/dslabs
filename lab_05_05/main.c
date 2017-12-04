#include <stdio.h>
#include <stdint.h>

#include "lib/list1.h"
#include "lib/mtrx.h"
#include "lib/errors.h"
#include "lib/time.h"
#include "sparse_mtrx.h"

extern tick_t ticks;

void smtrx_print(smtrx_pt sc)
{
    printf("A:  ");
    for (int i = 0; i < sc->len; i++)
        printf("%7.3f ", sc->a[i]);
    printf("\n");

    printf("JA: ");
    for (int i = 0; i < sc->len; i++)
        printf("%3d ", sc->j[i]);
    printf("\n");

    printf("IA: ");
    for (list1_t *l = sc->r; l; l = l->next)
        printf("%3d ", l->data);
    printf("\n");

    printf("Sparse: %7.3f%%\n", 100 * smtrx_sparse(sc));
}

int mtrx_read(mtrxp_t *m_p)
{
    int err = EOK;

    int i, j;
    float x;
    char c;

    if ((c = scanf("%d %d\n", &i, &j)) != 2)
        return EFORMAT;

    if ((err = alloc_mtrx(i, j, m_p, NULL, NULL)) != EOK)
        return err;

    while((c = getc(stdin)) <= '9' && c >= '0')
    {
        ungetc(c, stdin);
        if (scanf("%d %d %f\n", &i, &j, &x) != 3)
            return EFORMAT;

        i--;
        j--;

        if (i < 0 || (*m_p)->m <= i ||
            j < 0 || (*m_p)->n <= j)
            return EFORMAT;

        (*m_p)->d[i][j] = x;
    }

    return EOK;
}

void mtrx_read_format(char *name)
{
    printf("Input mtrx '%s' in format:\n\033[1mheight width\n{i j data} n times\033[0m\n", name);
}

int main(int argc, char **argv)
{
    int err = EOK;

    tick_t ticks_mtrx = 0;
    tick_t ticks_smtrx = 0;

    mtrxp_t a = NULL;
    mtrxp_t b = NULL;
    mtrxp_t c = NULL;

    mtrx_read_format("A");
    err = mtrx_read(&a);
    if (err == EOK)
    {
        mtrx_read_format("B");
        err = mtrx_read(&b);
        if (err == EOK)
        {
            err = mul_mtrx(a, b, &c);
            ticks_mtrx = ticks;
            if (err == EOK)
            {
                printf("A mtrx:\n");
                if (a->m <= 10 && a->n <= 5)
                    print_mtrx(a);
                else
                    printf("Too large");

                printf("\nB mtrx:\n");
                if (b->m <= 10 && b->n <= 5)
                    print_mtrx(b);
                else
                    printf("Too large");

                printf("\nResult:\n");
                if (c->m <= 10 && c->n <= 5)
                    print_mtrx(c);
                else
                    printf("Too large");
                printf("\n\n");

                smtrx_pt sa;
                mtrx_smtrx(a, &sa);

                smtrx_pt sb;
                mtrx_smtrx(b, &sb);

                smtrx_pt sc;
                err = smtrx_mul(sa, sb, &sc);
                ticks_smtrx = ticks;

                printf("A mtrx:\n");
                smtrx_print(sa);

                printf("\nB mtrx:\n");
                smtrx_print(sb);

                if (err == EOK)
                {
                    printf("\nResult:\n");
                    smtrx_print(sc);
                }

                smtrx_delete(&sa);
                smtrx_delete(&sb);
                smtrx_delete(&sc);

                printf("\nTicks  MTRX: \033[1;31m%"PRIu64"\033[0m\n", ticks_mtrx);
                printf("Ticks SMTRX: \033[1;32m%"PRIu64"\033[0m\n", ticks_smtrx);
            }
        }
    }

    free_mtrx(&a);
    free_mtrx(&b);
    free_mtrx(&c);

    if (err != EOK)
        printf("Err code: %d\n", err);

    return err != EOK;
}
