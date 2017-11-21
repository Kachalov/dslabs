#pragma once

#include <stdlib.h>

#include "lib/list1.h"
#include "lib/mtrx.h"

typedef struct
{
    list1_t *a;
    list1_t *j;
    list1_t *r;
    size_t m;
    size_t n;
} smtrx_t;

typedef smtrx_t *smtrx_pt;

void smtrx_init(size_t m, size_t n, smtrx_pt mtrx_p);
int smtrx_mul(smtrx_pt a, smtrx_pt b, smtrx_pt c_p);
int smtrx_el(smtrx_pt m, int i, int j);

int _smtrx_add(smtrx_pt m, int i, int j, int data);
mtrx_data_i_t apply_mtrx_smtrx(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg);
int mtrx_smtrx(mtrxp_t f, smtrx_pt t);
