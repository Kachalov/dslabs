#pragma once

#include <stdlib.h>

#include "lib/list1.h"
#include "lib/mtrx.h"

typedef struct
{
    float *a;
    int *j;
    list1_t *r;
    size_t m;
    size_t n;
    size_t els;
    size_t len;
} smtrx_t;

typedef smtrx_t *smtrx_pt;

int smtrx_init(size_t m, size_t n, size_t elements, smtrx_pt *mtrx_pp);
void smtrx_delete(smtrx_pt *mtrx_pp);
int smtrx_mul(smtrx_pt a, smtrx_pt b, smtrx_pt *c);
float smtrx_el(smtrx_pt m, int i, int j);

int _smtrx_add(smtrx_pt m, int i, int j, float data);
mtrx_data_i_t apply_mtrx_smtrx(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg);
mtrx_data_i_t apply_mtrx_smtrx_count(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg);
int mtrx_smtrx(mtrxp_t f, smtrx_pt *t_p);
int smtrx_mtrx(smtrx_pt f, mtrxp_t *t_p);
float smtrx_sparse(smtrx_pt m);
