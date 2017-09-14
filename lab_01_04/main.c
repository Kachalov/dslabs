/**
 *  @author Alexey Kachalov <kachalov@kistriver.com>
 */

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
    #define DPRINT(...) {fprintf(stderr, __VA_ARGS__);}
#else
    #define DPRINT(...)
#endif

#define LFLOAT_MANTISSA_DIGITS 30
#define LFLOAT_MANTISSA_LEN (2 * LFLOAT_MANTISSA_DIGITS)
#define LFLOAT_EXP_MAX 99999
#define LFLOAT_FORMAT "%c0.%sE%+d"

#define OK 0
#define INVALID_INPUT 1
#define LFLOAT_OVERFLOW 2

typedef struct 
{
    int32_t exp;
    int32_t sign:1;
    int32_t len:31;
    char mantissa[LFLOAT_MANTISSA_LEN + 1];
} lfloat_t;

char *mantissa(lfloat_t *n);
int input_lfloat(char *num, lfloat_t *n);
int input_int_lfloat(char *num, lfloat_t *n);
void init_lfloat(lfloat_t *n);
int sum_lfloat(lfloat_t a, lfloat_t b, lfloat_t *r);
int sub_lfloat(lfloat_t a, lfloat_t b, lfloat_t *r);
int div_lfloat(lfloat_t a, lfloat_t b, lfloat_t *r);
int cmp_lfloat(lfloat_t a, lfloat_t b);
int equal_exp(lfloat_t *a, lfloat_t *b);
int offset_lfloat_mantissa(lfloat_t *n, int pow);
void print_lfloat(lfloat_t x);


int main(void)
{
    lfloat_t x, y, z;
    int err = 0;
    char buf[80];

    fgets(buf, 80, stdin);
    // if ((err = input_int_lfloat(buf, &x)) != OK)
    if ((err = input_lfloat(buf, &x)) != OK)
        goto failure;

    fgets(buf, 80, stdin);
    if ((err = input_lfloat(buf, &y)) != OK)
        goto failure;

    printf("X(");
    print_lfloat(x);
    printf(") ? \nY(");
    print_lfloat(y);
    printf(") = \n");

    if ((err = sum_lfloat(x, y, &z)) != OK)
        goto failure;

    printf("SUM(");
    print_lfloat(z);
    printf(")\n");

    if ((err = div_lfloat(x, y, &z)) != OK)
       goto failure;

    printf("DIV(");
    print_lfloat(z);
    printf(")\n");

    return 0;
    failure:
    fprintf(stderr, "Err code: %d\n", err);
    return 1;
}

int input_int_lfloat(char *num, lfloat_t *n)
{
    for (char *it = num; *it != '\n' && *it != '\r' && *it != '\0'; it++)
    {
        if (*it == '.' || *it == 'e' || *it == 'E')
            return INVALID_INPUT;
    }

    return input_lfloat(num, n);
}

int input_lfloat(char *num, lfloat_t *n)
{
    bool count_digits = false;
    bool int_part = true;
    bool cond = false;
    int8_t power = 0;
    int32_t exp = 0;
    uint8_t length = 0;
    char *it = num;

    init_lfloat(n);

    if (*it == '-' || *it == '+')
    {
        n->sign = *it == '-' ? 0 : 1;
        it++;
    }
    for (; *it != '\n' && *it != '\r' && *it != '\0'; it++)
    {
        cond = false;

        if (*it == '+' || *it == '-')
            return INVALID_INPUT;

        if (length >= LFLOAT_MANTISSA_DIGITS)
            return INVALID_INPUT;

        DPRINT("[%d][%c](%2X) ", length, *it, *it);
        if (*it == '.')
        {
            if (!int_part)
                return INVALID_INPUT;

            int_part = false;

            count_digits = power == 0;
            cond = true;
        }
        else if (*it == 'e' || *it == 'E')
        {
            if (sscanf(it + 1, "%d", &exp) != 1)
                return INVALID_INPUT;

            if (exp > LFLOAT_EXP_MAX || exp < -LFLOAT_EXP_MAX)
                return INVALID_INPUT;

            DPRINT("(EXP %d)", exp);
            n->exp = exp;
            DPRINT("\n");
            cond = true;
            break;
        }

        if (('0' <= *it && *it <= '9') || *it == '.')
        {
            if (int_part && !count_digits && *it != '0')
            {
                count_digits = true;
                DPRINT("(COUNT ON) ");
                cond = true;
            }

            if (!int_part && *it != '0' && *it != '.')
            {
                count_digits = false;
                DPRINT("(COUNT OFF) ");
                cond = true;
            }

            if ('0' <= *it && *it <= '9')
            {
                DPRINT("POW(%d) ", power);
                if (count_digits) {
                    power += 2 * int_part - 1;
                }

                if (*it != '0' || length != 0)
                    n->mantissa[length++] = *it;
                cond = true;
            }
        }

        if (!cond)
            return INVALID_INPUT;

        DPRINT("\n");
    }

    if (length == 0)
        n->len = 1;

    n->exp += power;
    n->len = length;

    DPRINT("Length: %d\n", length);
    for (int8_t i = LFLOAT_MANTISSA_LEN - 1; i >= 0; i--)
    {
        if (LFLOAT_MANTISSA_LEN <= i + length)
            n->mantissa[i] = n->mantissa[i + length - LFLOAT_MANTISSA_LEN];
        else
            n->mantissa[i] = '0';
    }

    return OK;
}

void init_lfloat(lfloat_t *n)
{
    n->exp = 0;
    n->len = 0;
    n->sign = 0;
    n->sign++;

    for (uint8_t i = 0; i < LFLOAT_MANTISSA_LEN; i++)
        n->mantissa[i] = '0';
    n->mantissa[LFLOAT_MANTISSA_LEN] = '\0';
}

char *mantissa(lfloat_t *n)
{
    return n->mantissa + LFLOAT_MANTISSA_LEN - n->len;
}

int sum_lfloat(lfloat_t a, lfloat_t b, lfloat_t *r)
{
    if (a.sign != b.sign)
    {
        b.sign = 1 - b.sign;
        return sub_lfloat(a, b, r);
    }

    int err = OK;
    int cell = 0;
    char overdigit = 0;
    init_lfloat(r);

    if ((err = equal_exp(&a, &b)) != OK)
        return err;

    for (int i = LFLOAT_MANTISSA_LEN - 1; i > 0; i--)
    {
        cell = a.mantissa[i] + b.mantissa[i] + cell - 2 * '0';
        r->mantissa[i] = cell % 10 + '0';
        //DPRINT("SUM [%i] if=%d cell=%d\n", i, LFLOAT_MANTISSA_LEN - 1 - a.len - i, cell);
        if (i <= LFLOAT_MANTISSA_LEN - a.len && cell > 9)
            overdigit++;
        cell /= 10;
    }
    r->len = a.len + overdigit;
    r->exp = a.exp + overdigit;
    r->sign = a.sign;

    return err;

}

int sub_lfloat(lfloat_t a, lfloat_t b, lfloat_t *r)
{
    if (a.sign > b.sign)
    {
        b.sign++;
        return sum_lfloat(a, b, r);
    }
    else if (a.sign < b.sign)
    {
        b.sign--;
        return sum_lfloat(a, b, r);
    }

    int err = OK;
    int cell = 0;
    char overdigit = 0;
    lfloat_t tmp;
    init_lfloat(r);

    if (cmp_lfloat(a, b) > 0)
    {
        tmp = a;
        a = b;
        b = tmp;
        a.sign = 1 - a.sign;
        b.sign = 1 - b.sign;
    }

    if ((err = equal_exp(&a, &b)) != OK)
        return err;

    for (int i = LFLOAT_MANTISSA_LEN - 1; i > 0; i--)
    {
        cell = a.mantissa[i] - b.mantissa[i] + cell;

        r->mantissa[i] = (cell + 10) % 10 + '0';
        //DPRINT("SUB [%i] if=%d cell=%d\n", i, LFLOAT_MANTISSA_LEN - 1 - a.len - i, cell);
        if (i <= LFLOAT_MANTISSA_LEN - 1 - a.len && cell < 0)
            overdigit++;
        cell = cell < 0 ? -1 : 0;
    }
    r->len = a.len - overdigit;
    r->exp = a.exp - overdigit;
    r->sign = a.sign;

    return err;
}

int div_lfloat(lfloat_t a, lfloat_t b, lfloat_t *r)
{
    return OK;
}

int offset_lfloat_mantissa(lfloat_t *n, int offset)
{
    //int len = strlen(mantissa(n));

    // TODO: Exponent overflow checking
    // if (offset + n->exp > LFLOAT_EXP_MAX || offset + n->exp < -LFLOAT_EXP_MAX)
    //     return LFLOAT_OVERFLOW;
    // plus <-
    // minus ->

    if (offset > 0)
    {
        n->len += offset;
        #ifdef DEBUG
            DPRINT("PLUS(%d): ", offset); print_lfloat(*n); printf("\n");
        #endif
        for (int8_t i = 0; i < LFLOAT_MANTISSA_LEN; i++)
        {
            if (offset <= i)
            {
                n->mantissa[i - offset] = n->mantissa[i];
                n->mantissa[i] = '0';
            }
            else
                n->mantissa[i] = '0';
        }
    }
    else if (offset < 0)
    {
        offset *= -1;
        n->len += offset;
        #ifdef DEBUG
            DPRINT("MINUS(%d): ", offset); print_lfloat(*n); printf("\n");
        #endif
        /*for (int8_t i = 0; i < LFLOAT_MANTISSA_LEN; i++)
        {
            if (offset <= i && i <= LFLOAT_MANTISSA_LEN - len + 2)
            {
                n->mantissa[i - offset] = n->mantissa[i];
                n->mantissa[i] = '0';
            }
            else
                n->mantissa[i] = '0';
        }*/
    }

    return OK;
}

int cmp_lfloat(lfloat_t a, lfloat_t b)
{
    if (a.sign != b.sign)
        return a.sign > b.sign;

    if (a.exp != b.exp)
        return (2*a.sign - 1) * (a.exp - b.exp);

    for (int i = LFLOAT_MANTISSA_LEN - 1 - a.len; i < LFLOAT_MANTISSA_LEN; i++)
        if (a.mantissa[i] != b.mantissa[i])
            return (2*a.sign - 1) * (a.mantissa[i] - b.mantissa[i]);

    return 0;
}

int equal_exp(lfloat_t *a, lfloat_t *b)
{
    int err = OK;

    if (a->exp >= b->exp)
    {
        if ((err = offset_lfloat_mantissa(b, b->exp - a->exp)) != OK)
            return err;

        b->exp = a->exp;

        if (a->len >= b->len)
        {
            if ((err = offset_lfloat_mantissa(b, a->len - b->len)) != OK)
                return err;
        }
        else
        {
            if ((err = offset_lfloat_mantissa(a, b->len - a->len)) != OK)
                return err;
        }
    }
    else
    {
        return equal_exp(b, a);
    }

    return err;
}

void print_lfloat(lfloat_t x)
{
    printf(LFLOAT_FORMAT, x.sign ? '+' : '-', mantissa(&x), (int) x.exp);
}
