#pragma once

#include "mtrx.h"

mtrx_data_i_t apply_mtrx_numerate(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg);
mtrx_data_i_t apply_mtrx_e(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg);
mtrx_data_i_t apply_mtrx_copy(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg);
mtrx_data_i_t apply_mtrx_ndx(mtrxp_t mtrx_p, mtrx_size_t i, mtrx_size_t j, void *arg);
