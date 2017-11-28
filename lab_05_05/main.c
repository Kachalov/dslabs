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

int main(int argc, char **argv)
{
    int err = EOK;

    mtrxp_t a = NULL;
    mtrxp_t b = NULL;
    mtrxp_t c = NULL;

    err = read_mtrx(argv[1], &a);
    if (err == EOK)
    {
        err = read_mtrx(argv[2], &b);
        if (err == EOK)
        {
            err = mul_mtrx(a, b, &c);
            if (err == EOK)
            {
                printf("A mtrx:\n");
                print_mtrx(a);
                printf("\nB mtrx:\n");
                print_mtrx(b);
                printf("\nResult:\n");
                print_mtrx(c);
                printf("\n");

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
