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

// TODO: Change to 30
#define LFLOAT_MANTISSA_DIGITS 5
#define LFLOAT_MANTISSA_LEN (2 * LFLOAT_MANTISSA_DIGITS)
#define LFLOAT_EXP_MAX 99999
#define LFLOAT_FORMAT "%c0.%sE%+d"

#define OK 0
#define INVALID_INPUT 1
#define LFLOAT_OVERFLOW 2
#define ZERO_DIVISION 3

typedef struct 
{
    int32_t exp;
    uint8_t sign:1;
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

    //printf("SUM(");
    //print_lfloat(z);
    //printf(")\n");

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

        if (length > LFLOAT_MANTISSA_DIGITS)
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

    n->exp += power;
    n->len = length;

    if (length == 0)
    {
        n->len = 1;
        n->exp = 1;
    }

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
    n->sign = 1;

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
    int sign = cmp_lfloat(a, b);
    lfloat_t tmp;
    init_lfloat(r);

    //printf("SIGN: %d\n", sign);
    if (sign < 0)
    {
        tmp = a;
        a = b;
        b = tmp;
        a.sign = 1 - a.sign;
        b.sign = 1 - b.sign;
    }

    if ((err = equal_exp(&a, &b)) != OK)
        return err;

    print_lfloat(a); printf(" - "); print_lfloat(b); printf(" = ");

    for (int i = LFLOAT_MANTISSA_LEN - 1; i > LFLOAT_MANTISSA_LEN - 1 - a.len; i--)
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
    r->sign = sign > 0 ? 1 : (sign == 0 ? a.sign : 0);

    print_lfloat(*r); printf("\n=======\n");

    return err;
}

int div_lfloat(lfloat_t a, lfloat_t b, lfloat_t *r)
{
    int err = OK;
    int sign = ((1 - 2 * a.sign) * (1 - 2 * b.sign) + 1) / 2;
    int exp = a.exp - b.exp;
    int i = LFLOAT_MANTISSA_LEN;
    lfloat_t tmp, one, zero;

    init_lfloat(&tmp);
    init_lfloat(&one);
    init_lfloat(&zero);
    init_lfloat(r);

    one.mantissa[LFLOAT_MANTISSA_LEN - 1] = '1';

    if (b.len < 2 && mantissa(&b)[0] == '0')
    {
        r->exp = 0;
        r->len = 1;
        r->sign = b.sign;
        return ZERO_DIVISION;
    }

    //*r = a;
    //r->exp = a.exp - b.exp;
    r->exp = 0;
    r->len = 1;
    r->sign = 1;
    one.exp = -1;
    b.exp = a.exp;
    b.sign = 1;

    if (cmp_lfloat(a, b) < 0)
        exp++;

    //print_lfloat(a); printf(" : "); print_lfloat(b); printf("\n");
    while (i > 0)
    {
        if ((err = sub_lfloat(a, b, &tmp)) != OK)
            return err;

        if (tmp.sign == 0)
        {
            b.exp--;
            one.exp--;
            i--;
            //continue;
        }
        else
        {
            a = tmp;
            sum_lfloat(*r, one, r);
        }
        printf("sum: "); print_lfloat(*r); printf("\n");

        if (cmp_lfloat(tmp, zero) == 0 || tmp.len > LFLOAT_MANTISSA_LEN)
            break;
    }

    r->exp = exp;
    r->sign = sign;

    return err;
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
        if (n->len + offset > LFLOAT_MANTISSA_LEN)
            n->len = LFLOAT_MANTISSA_LEN;
        else
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
        if (n->len + offset > LFLOAT_MANTISSA_LEN)
            n->len = LFLOAT_MANTISSA_LEN;
        else
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
        return a.sign - b.sign;

    equal_exp(&a, &b);
    char *ma = mantissa(&a);
    char *mb = mantissa(&b);

    print_lfloat(a); printf(" ?= "); print_lfloat(b); printf("\n");

    //if (a.exp != b.exp)
    //    return (2 * a.sign - 1) * (a.exp - b.exp);

    for (int i = 0; i < strlen(ma); i++)
        if (ma[i] != mb[i])
            return (2 * a.sign - 1) * (ma[i] - mb[i]);

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
    #ifndef FULLMAN
        printf(LFLOAT_FORMAT, x.sign ? '+' : '-', mantissa(&x), (int) x.exp);
    #else
        char tmp[LFLOAT_MANTISSA_LEN + 1];
        strcpy(tmp, x.mantissa);
        tmp[LFLOAT_MANTISSA_LEN - x.len] = '\0';
        printf("%c0.\x1b[31m%s\x1b[0m%sE%+d", x.sign ? '+' : '-',
               tmp,
               mantissa(&x), (int) x.exp);
    #endif
}
