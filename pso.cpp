#include "pso.hpp"
#include "common_defs.hpp"
#include "pso_utils.hpp"

#define PSO_DIMENSION ARGS_SIZE
#define PSO_SWARM_SIZE 100
#define PSO_MAX_ITER 100

#define CALC_ARG_VELOCITY(i, arg)                                              \
    swarm[i].velocity.arg =                                                    \
        (args_const_lut.arg.inertia * swarm[i].velocity.arg) +                 \
        (args_const_lut.arg.personal_weight * rand_val[2 * i] *                \
         (swarm[i].position_best.arg - swarm[i].position.arg)) +               \
        (args_const_lut.arg.global_weight * rand_val[2 * i + 1] *              \
         (global_best->position.arg - swarm[i].position.arg));

/* Constants */
static const ap_fixed_32p16 PI_x_2 = 6.28318530717958647692;

static const sModelArgsMinMax_t args_const_lut = {
/*   MIN    DEFAULT     MAX         INERTIA     PERSONAL_W      GLOBAL_W        ARG     UNIT        */
    {0,     20,         100,        0.5,        1,              1       },  /*  alfa0   [-]         */
    {0,     500000,     10000000,   0.5,        1,              1       },  /*  freq0   [Hz]        */
    {0,     1000,       2000,       0.5,        1,              1       },  /*  c2      [m/s]       */
    {0,     2,          10,         0.5,        1,              1       },  /*  n       [-]         */
    {0,     1000,       2000,       0.5,        1,              1       },  /*  ro2     [kg/m^3]    */
    {0,     0.0010,     1,          0.5,        1,              1       }   /*  h       [m]         */
};

ap_fixed_32p16 pso_fitness(const sModelArgs args, const sModelParams params,
                           const ap_fixed_32p16 *ref_signal,
                           const ap_fixed_64p32 *freq_axis) {
    /* Add your code here */
    return 0;
}

/* PSO Functions */
static void pso_swarm_init(sParticle_t *swarm) {
    /* Initiate swarm values */
    for (int i = 0; i < PSO_SWARM_SIZE; i++) {
        swarm[i].position.alfa0 = args_const_lut.alfa0.max * PRNG_16();
        swarm[i].position.freq0 = args_const_lut.freq0.max * PRNG_32();
        swarm[i].position.c2 = args_const_lut.c2.max * PRNG_16();
        swarm[i].position.n = args_const_lut.n.max * PRNG_16();
        swarm[i].position.ro2 = args_const_lut.ro2.max * PRNG_16();
        swarm[i].position.h = args_const_lut.h.max * PRNG_32();

        swarm[i].position_best = swarm[i].position;
        
        swarm[i].velocity.alfa0 = 0;
        swarm[i].velocity.freq0 = 0;
        swarm[i].velocity.c2 = 0;
        swarm[i].velocity.n = 0;
        swarm[i].velocity.ro2 = 0;
        swarm[i].velocity.h = 0;

        swarm[i].fitness_current =
            pso_fitness(swarm[i].position, params, ref_signal, freq_axis);
        swarm[i].fitness_best = swarm[i].fitness_current;
    }
}

static void pso_swarm_update(sParticle_t *swarm, sParticle_t *global_best) {
    /* Add your code here */
    ap_fixed_32p16 rand_val[2 * PSO_DIMENSION];

    /* Update velocity and position */
    for (int i = 0; i < PSO_SWARM_SIZE; i++) {
        for (int j = 0; j < 2 * PSO_DIMENSION; j++) {
            rand_val[j] = PRNG_16();
        }

        CALC_ARG_VELOCITY(i, alfa0)
        CALC_ARG_VELOCITY(i, freq0)
        CALC_ARG_VELOCITY(i, c2)
        CALC_ARG_VELOCITY(i, n)
        CALC_ARG_VELOCITY(i, ro2)
        CALC_ARG_VELOCITY(i, h)

        swarm[i].position[j] += swarm[i].velocity[j];
    }

    /* Update fitness */
    for (int i = 0; i < PSO_SWARM_SIZE; i++) {
        swarm[i].fitness_current =
            Fitness(swarm[i].position[0], swarm[i].position[1]);
        if (swarm[i].fitness_current < swarm[i].fitness_best) {
            swarm[i].fitness_best = swarm[i].fitness_current;
            for (int j = 0; j < DIMENSION; j++) {
                swarm[i].position_best[j] = swarm[i].position[j];
            }
        }
    }
}

/* Find best particle in the swarm and return its fitness */
static ap_fixed_32p16 pso_find_global_best(sParticle_t *swarm, sParticle_t *global_best) {
    for (int i = 0; i < PSO_SWARM_SIZE; i++) {
        if (swarm[i].fitness_best < global_best->fitness_best) {
            global_best = swarm[i];
        }
    }
    return global_best->fitness_best;
}

void pso_process(sModelArgs &args_estimate, const sModelParams &params,
                 const ap_fixed_32p16 *ref_signal,
                 const ap_fixed_64p32 *freq_axis, const uint16_t itterations) {

    sParticle_t swarm[PSO_SWARM_SIZE];
    sModelArgs global_best;
    ap_fixed_32p16 global_best_fitness = 32768;

    pso_swarm_init(swarm);

    for (int iter = 0; iter < itterations; iter++) {
        global_best_fitness = pso_find_global_best(swarm, &global_best);
        pso_swarm_update(swarm, &global_best);
        args_estimate = global_best;
    }
}

/** TODO: To be adapted*/
void PSO_AP(float_f &position_X, float_f &position_Y, float_f &fitness,
            const char itterations) {

    static sParticle_t swarm[SWARM_SIZE];
    static float_f global_best[DIMENSION];
    static float_f global_best_fitness = 9999;

    inertia_weight = INERTIA_WEIGHT;
    dimension_max = DIMENSION_MAX;

    swarm_init(swarm);

    for (int iter = 0; iter < itterations; iter++) {
        // Update the global best
        swarm_update_global(swarm, global_best, global_best_fitness);
        swarm_update(swarm, global_best);

        /* Update output */
        position_X = global_best[0];
        position_Y = global_best[1];
        fitness = global_best_fitness;
    }
}
