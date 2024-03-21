#pragma once

typedef struct sParticle_t {
    /* Position == ModelArgs */
    sModelArgs position;
    sModelArgs position_best;
    sModelArgs velocity;

    ap_fixed_32p16 fitness_current;
    ap_fixed_32p16 fitness_best;
} sParticle_t;

void pso_process(sModelArgs &args_estimate, const sModelParams &params,
                 const ap_fixed_32p16 *ref_signal,
                 const ap_fixed_64p32 *freq_axis, const uint16_t itterations);
