#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "stack.h"

#define STACK_ARRAY_SIZE 10
#define STACK_UP_NDX 3

#define STACK_POP(tmp, type, def, stack) (tmp = stack_pop(stack)) == NULL ? def : *(type *)tmp

int main(void)
{
    lab_stack_t *su = NULL;
    lab_stack_t *sd = NULL;
    lab_stack_t *sl = NULL;
    int16_t a[STACK_ARRAY_SIZE];
    int16_t x = 0;
    int16_t *tmp_ptr = NULL;

    stack_init(&su, true, false, a, a + STACK_UP_NDX, sizeof(int16_t));
    stack_init(&sd, false, false, a + STACK_UP_NDX + 1, a + STACK_ARRAY_SIZE - 1, sizeof(int16_t));
    stack_init(&sl, false, true, NULL, NULL, sizeof(int16_t));

    srand((unsigned) time(NULL));

    printf("Type 'h' for help\n");
    char buf[80];
    char *arg;
    while (fgets(buf, 80, stdin) != NULL)
    {
        if (strlen(buf) >= 1 && buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';

        arg = strtok(buf, " ");

        if (arg == NULL)
            arg = buf;


        if (strcmp(arg, "") == 0)
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

            printf("UP: %p %p %p\n", su->lp, su->sp, su->hp);
            printf("DOWN: %p %p %p\n", sd->lp, sd->sp, sd->hp);

            printf("| ");
            for (int i = 0; i < STACK_ARRAY_SIZE; i++)
            {
                printf("%6d ", a[i]);
                if (i == STACK_UP_NDX)
                    printf("|  ");
            }
            printf("\n");

            for (int i = 0; i < STACK_UP_NDX + 1; i++)
            {
                if (i == ((char *)su->sp + sizeof(int16_t) - (char *)su->lp) / sizeof(int16_t))
                    printf("^");
                printf("       ");
            }
            printf(" |");

            for (int i = 0; i < STACK_ARRAY_SIZE - (STACK_UP_NDX + 1); i++)
            {
                if (i == ((char *)sd->sp + sizeof(int16_t) - (char *)sd->lp) / sizeof(int16_t))
                    printf("^");
                printf("       ");
            }
            printf("\n");

            for (int i = 0; i < 10; i++)
                printf("%d ", STACK_POP(tmp_ptr, int16_t, 0, sl));
            printf("\n");
        }
        else if (strcmp(arg, "e") == 0)
        {
            break;
        }
        else if (strcmp(arg, "h") == 0)
        {
            printf(
                "h - for help\n"
                "[enter] - iterate\n"
                "e - exit\n"
            );
        }
        else
        {
            printf("Type 'h' for help\n");
        }
    }

    stack_delete(&su);
    stack_delete(&sd);
    stack_delete(&sl);

    return 0;
}
