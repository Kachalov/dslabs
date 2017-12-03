#include <stdio.h>
#include <stdint.h>

#include "lib/list1.h"
#include "lib/mtrx.h"
#include "lib/errors.h"
#include "lib/time.h"
#include "sparse_mtrx.h"

void smtrx_print(smtrx_pt sc)
{
    printf("A: ");
    for (int i = 0; i < sc->len; i++)
        printf("%.2f ", sc->a[i]);
    printf("\n");

    printf("J: ");
    for (int i = 0; i < sc->len; i++)
        printf("%d ", sc->j[i]);
    printf("\n");

    printf("R: ");
    for (list1_t *l = sc->r; l; l = l->next)
        printf("%d ", l->data);
    printf("\n");
}

int mtrx_read(mtrxp_t *m_p)
{
    int err = EOK;

    int i, j;
    float x;
    char c;

    if (scanf("%d %d\n", &i, &j) !=2)
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
            if (err == EOK)
            {
                printf("A mtrx:\n");
                if (a->n <= 5)
                    print_mtrx(a);
                else
                    printf("Too wide");

                printf("\nB mtrx:\n");
                if (b->n <= 5)
                    print_mtrx(b);
                else
                    printf("Too wide");

                printf("\nResult:\n");
                if (c->n <= 5)
                    print_mtrx(c);
                else
                    printf("Too wide");
                printf("\n\n");

                smtrx_pt sa;
                mtrx_smtrx(a, &sa);
                
                smtrx_pt sb;
                mtrx_smtrx(b, &sb);

                smtrx_pt sc;
                err = smtrx_mul(sa, sb, &sc);

                printf("A mtrx:\n");
                smtrx_print(sa);

                printf("\nB mtrx:\n");
                smtrx_print(sb);

                if (err == EOK)
                {
                    printf("\nResult:\n");
                    smtrx_print(sc);
                }
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
