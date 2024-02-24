#pragma once

#include "common_defs.hpp"

void TransferFunction(const sModelArgs args, const sModelParams params,
                      const fixed_double *freq_axis, complex_float *tf_out);
