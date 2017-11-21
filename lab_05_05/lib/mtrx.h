#pragma once

#include <inttypes.h>
#include <stdio.h>

typedef float mtrx_data_i_t;
typedef mtrx_data_i_t **mtrx_data_t;
typedef uint32_t mtrx_size_t;
typedef struct _mtrx_t
{
    mtrx_size_t m;
    mtrx_size_t n;
    mtrx_data_t d;
} mtrx_t;

typedef mtrx_t *mtrxp_t;
typedef mtrx_data_i_t (*apply_mtrx_f_t)(mtrxp_t, mtrx_size_t, mtrx_size_t, void *);

typedef struct
{
    mtrx_size_t m;
    mtrx_size_t n;
} mtrx_sizes_t;

int alloc_mtrx(mtrx_size_t m, mtrx_size_t n, mtrxp_t *mtrx_pp, apply_mtrx_f_t f, void *arg);
int free_mtrx(mtrxp_t *mtrx_pp);
void apply_mtrx(mtrxp_t mtrx_p, apply_mtrx_f_t f, void *arg);
void sprint_mtrx(mtrxp_t mtrx_p, char *str);
void print_mtrx(mtrxp_t mtrx_p);
int fprint_mtrx(mtrxp_t mtrx_p, char *fn);
int sum_mtrx(mtrxp_t a, mtrxp_t b, mtrxp_t *c_p);
int mul_mtrx(mtrxp_t a, mtrxp_t b, mtrxp_t *c_p);
mtrx_data_i_t mul_i_mtrx(mtrxp_t a, mtrxp_t b, mtrx_size_t i, mtrx_size_t j);
void swap_col_mtrx(mtrxp_t a, mtrx_size_t i, mtrx_size_t j);
mtrx_sizes_t size_file_mtrx(FILE *f);
int read_mtrx(char *fn, mtrxp_t *m);
