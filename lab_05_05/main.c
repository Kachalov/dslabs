#include <stdio.h>

#include "lib/list1.h"
#include "lib/mtrx.h"
#include "lib/errors.h"
#include "sparse_mtrx.h"

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
            }
        }
    }

    smtrx_t sa;
    mtrx_smtrx(a, &sa);

    printf("A: ");
    for (list1_t *l = sa.a; l; l = l->next)
        printf("%d ", l->data);
    printf("\n");

    printf("J: ");
    for (list1_t *l = sa.j; l; l = l->next)
        printf("%d ", l->data);
    printf("\n");

    printf("R: ");
    for (list1_t *l = sa.r; l; l = l->next)
        printf("%d ", l->data);
    printf("\n");

    free_mtrx(&a);
    free_mtrx(&b);
    free_mtrx(&c);

    printf("Err code: %d\n", err);


    return 0;
}
