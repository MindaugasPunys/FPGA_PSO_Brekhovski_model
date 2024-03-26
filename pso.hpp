#pragma once

#include "common_defs.hpp"

#define PSO_ITERATIONS 100
#define PSO_SWARM_SIZE 200

void pso_process(ap_fixed_64p32 *args_estimate, const sModelParams &params,
                 const ap_fixed_32p16 *ref_signal,
                 const ap_fixed_64p32 *freq_axis, const uint16_t itterations);
