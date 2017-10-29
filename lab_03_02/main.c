#include <stdio.h>

#include "stack.h"

int main(void)
{
    stack_t *su = NULL;
    stack_t *sd = NULL;
    stack_t *sl = NULL;
    int16_t a[4];
    int16_t x = 0;

    stack_init(&su, true, false, a, a + 1, 2);
    stack_init(&sd, false, false, a + 2, a + 3, 2);
    stack_init(&sl, false, true, NULL, NULL, 2);

    printf("UP: %p %p %p\n", su->lp, su->sp, su->hp);
    printf("DOWN: %p %p %p\n", sd->lp, sd->sp, sd->hp);

    x = 10;
    stack_push(su, &x);

    x = 20;
    stack_push(su, &x);

    x = 30;
    stack_push(sd, &x);

    x = 40;
    stack_push(sd, &x);

    for (x = 0; x < 10; x++)
        stack_push(sl, &x);

    for (int i = 0; i < 4; i++)
        printf("%d ", a[i]);
    printf("\n");

    printf("%d\n", *(int16_t *)stack_pop(su));
    printf("%d\n", *(int16_t *)stack_pop(su));

    for (int i = 0; i < 10; i++)
        printf("%d ", *(int16_t *)stack_pop(sl));
    printf("\n");

    stack_delete(&su);
    stack_delete(&sd);
    stack_delete(&sl);

    return 0;
}
