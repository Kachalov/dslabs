#include "mtrx.h"
#include "mtrx_apply.h"

mtrx_data_i_t apply_mtrx_numerate(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg)
{
    return mtrx_p->n * i + j;
}

mtrx_data_i_t apply_mtrx_e(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg)
{
    return i == j;
}

mtrx_data_i_t apply_mtrx_copy(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg)
{
    mtrxp_t mc = (mtrxp_t) arg;
    return mc->d[i][j];
}

mtrx_data_i_t apply_mtrx_ndx(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg)
{
    return j;
}
