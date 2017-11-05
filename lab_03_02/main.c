#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "stack.h"
#include "lib/errors.h"
#include "lib/list2.h"

#ifndef STACK_ARRAY_SIZE
    #define STACK_ARRAY_SIZE 6
#endif

#ifndef STACK_UP_NDX
    #define STACK_UP_NDX 2
#endif

int main(void)
{
    lab_stack_t *su = NULL;
    lab_stack_t *sd = NULL;
    lab_stack_t *sl = NULL;
    int16_t a[STACK_ARRAY_SIZE];
    int16_t x = 0;
    int16_t *tmp_ptr = NULL;
    char buf[80];
    char *arg;

    stack_init(&su, true, false,
               a, a + STACK_UP_NDX, sizeof(int16_t));
    stack_init(&sd, false, false,
               a + STACK_UP_NDX + 1, a + STACK_ARRAY_SIZE - 1, sizeof(int16_t));
    stack_init(&sl, false, true,
               NULL, NULL, sizeof(int16_t));

    srand((unsigned) time(NULL));

    printf("COM> ");
    while (fgets(buf, 80, stdin) != NULL)
    {
        if (strlen(buf) >= 1 && buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';

        arg = strtok(buf, " ");

        if (arg == NULL)
            arg = buf;

        if (strcmp(arg, "i") == 0)
        {
            x = rand() % (INT16_MAX - INT16_MIN);

            if (rand() % 2)
            {
                stack_push(rand() % 2 ? su : sd, &x);
                printf("push: %d\n", x);
            }
            else
            {
                x = STACK_POP(tmp_ptr, int16_t, 0, rand() % 2 ? su : sd);
                printf("pop: ");
                if (tmp_ptr)
                    printf("%d\n", x);
                else
                    printf("null\n");
            }
        }
        else if (strcmp(arg, "e") == 0)
        {
            break;
        }
        else if (strcmp(arg, "g") == 0)
        {
            arg = strtok(NULL, " ");
            if (arg == NULL)
                continue;

            x = strtod(arg, NULL);
            lab_stack_t *s = x == 1 ? su : x == 2 ? sd : sl;
            x = STACK_POP(tmp_ptr, int16_t, 0, s);
            printf("pop: ");
            if (tmp_ptr)
                printf("%d\n", x);
            else
                printf("null\n");
        }
        else if (strcmp(arg, "s") == 0)
        {
            arg = strtok(NULL, " ");
            if (arg == NULL)
                continue;

            int x = strtod(arg, NULL);
            lab_stack_t *s = x == 1 ? su : x == 2 ? sd : sl;

            arg = strtok(NULL, " ");
            if (arg == NULL)
                continue;

            x = strtod(arg, NULL);

            x = stack_push(s, &x);
            if (x == EOOM)
                printf("push: OOM\n");
            else
                printf("pushed\n");
        }
        else if (strcmp(arg, "h") == 0)
        {
            printf(
                "h           - for help\n"
                "i           - iterate\n"
                "s {N} {V}   - set value {V} to stack {N}\n"
                "              (1 - stack up, 2 - stack down, 3 - stack list)\n"
                "g {N}       - get value from stack {N}\n"
                "e           - exit\n"
            );
        }
        else
        {
            printf("Type 'h' for help\n");
        }

        printf("\n=== Stack on array ===\n");
        printf("  ");
        for (int i = 0; i < STACK_ARRAY_SIZE; i++)
        {
            printf("%6d ", a[i]);
            if (i == STACK_UP_NDX)
                printf("|  ");
        }
        printf("\n");

        for (int i = 0; i < STACK_ARRAY_SIZE + 2; i++)
        {
            if (i == ((char *)su->sp + sizeof(int16_t) - (char *)su->lp) / sizeof(int16_t)
                || STACK_ARRAY_SIZE - i + 1 ==
                    ((char *)sd->hp + sizeof(int16_t) - (char *)sd->sp) / sizeof(int16_t)
                )
            {
                printf("^");
            }

            if (i == STACK_UP_NDX + 1)
                printf(" |");

            printf("       ");
        }
        printf("\n");

        printf("\n=== Stack on list ===\n");
        for (list2_t *it = sl->lp; it != NULL; it = it->next)
        {
            if (it->data != NULL)
            {
                if (sl->sp == it)
                    printf("> ");
                else
                    printf("  ");

                printf("%p %d\n", (void *)it, *(int16_t *)it->data);
            }
        }
        printf("\n");

        printf("COM> ");
    }

    stack_delete(&su);
    stack_delete(&sd);
    stack_delete(&sl);

    return 0;
}
