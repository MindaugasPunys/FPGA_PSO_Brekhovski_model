#ifndef MODEL_HPP
#define MODEL_HPP

#include "common_defs.hpp"

struct sModelArgs {
    fixed_float alfa0;
    fixed_big_float freq0;
    fixed_float c2;
    fixed_float n;
    fixed_float ro2;
    fixed_float h;
};

struct sModelParams {
    fixed_big_float fsampl;
    fixed_float c1;
    fixed_float ro1;
};

void TransferFunction(const sModelArgs args, const sModelParams params,
                      const fixed_big_float freq_axis[FREQ_AXIS_SIZE],
                      complex_float tf_out[TRANSFER_FUNC_SIZE]);

#endif // MODEL_HPP
