#pragma once

#include "common_defs.hpp"

void TransferFunction(const sModelArgs args, const sModelParams params,
                      const ap_fixed_64p32 *freq_axis, ap_complex_32p16 *tf_out);
