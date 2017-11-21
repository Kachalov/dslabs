#pragma once

#include <stdlib.h>

#include "lib/list1.h"

typedef struct
{
    list1_t *a;
    list1_t *j;
    list1_t *r;
    size_t m;
    size_t n;
} smtrx_t;

typedef smtrx_t *smtrx_pt;

int smtrx_init(size_t m, size_t n, smtrx_pt *mtrx_p);
int smtrx_mul(smtrx_pt a, smtrx_pt b, smtrx_pt c_p);
