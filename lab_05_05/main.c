#include <stdio.h>

#include "lib/list1.h"
#include "lib/mtrx.h"
#include "lib/errors.h"
#include "lib/time.h"
#include "sparse_mtrx.h"

int main(int argc, char **argv)
{
    int err = EOK;

    mtrxp_t a = NULL;
    mtrxp_t b = NULL;
    mtrxp_t c = NULL;

    tick_t begin = 0;
    tick_t end = 0;

    err = read_mtrx(argv[1], &a);
    if (err == EOK)
    {
        err = read_mtrx(argv[2], &b);
        if (err == EOK)
        {
            begin = tick();
            err = mul_mtrx(a, b, &c);
            end = tick();
            if (err == EOK)
            {
                printf("A mtrx:\n");
                print_mtrx(a);
                printf("\nB mtrx:\n");
                print_mtrx(b);
                printf("\nResult:\n");
                print_mtrx(c);
                printf("Ticks: %zu\n", end - begin);
                printf("\n");

                smtrx_pt sa;
                mtrx_smtrx(a, &sa);

                smtrx_pt sb;
                mtrx_smtrx(b, &sb);

                begin = tick();
                smtrx_pt sc;
                err = smtrx_mul(sa, sb, &sc);
                end = tick();

                if (err == EOK)
                {
                    printf("A: ");
                    for (int i = 0; i < sc->len; i++)
                        printf("%d ", sc->a[i]);
                    printf("\n");

                    printf("J: ");
                    for (int i = 0; i < sc->len; i++)
                        printf("%d ", sc->j[i]);
                    printf("\n");

                    printf("R: ");
                    for (list1_t *l = sc->r; l; l = l->next)
                        printf("%d ", l->data);
                    printf("\n");

                    printf("Ticks: %zu\n", end - begin);
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
