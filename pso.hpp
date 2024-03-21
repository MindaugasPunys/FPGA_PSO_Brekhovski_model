#pragma once

#include "common_defs.hpp"

#define PSO_SWARM_SIZE 100
#define PSO_MAX_ITER 1000

void pso_process(sModelArgs &args_estimate, const sModelParams &params,
                 const ap_fixed_32p16 *ref_signal,
                 const ap_fixed_64p32 *freq_axis, const uint16_t itterations);
