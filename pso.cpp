#include "pso.hpp"
#include "pso_utils.hpp"

#define PSO_DIMENSION ARGS_SIZE

typedef struct sParticle_t {
    /* Position == ModelArgs */
    sModelArgs position;
    sModelArgs velocity;
    ap_fixed_64p32 fitness_current;

    sModelArgs position_best;
    ap_fixed_64p32 fitness_best;
} sParticle_t;

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

ap_fixed_64p32 pso_fitness(const sModelArgs args, const sModelParams params,
                           const ap_fixed_32p16 *ref_signal,
                           const ap_fixed_64p32 *freq_axis) {
    /* Add your code here */
    ap_fixed_64p32 fitness = 0;
    /* TEST */
    fitness = args.alfa0 * args.alfa0 + args.freq0 * args.freq0 +
              args.c2 * args.c2 + args.n * args.n + args.ro2 * args.ro2 +
              args.h * args.h;
    return fitness;
}

/* PSO Functions */
static void pso_swarm_init(sParticle_t *swarm, const sModelParams &params,
                           const ap_fixed_32p16 *ref_signal,
                           const ap_fixed_64p32 *freq_axis) {
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

#define CALC_ARG_VELOCITY(arg, rng_1, rng_2)                                   \
    swarm.velocity.arg = args_const_lut.arg.inertia * swarm.velocity.arg +     \
                         args_const_lut.arg.personal_weight * rng_1 *          \
                             (swarm.position_best.arg - swarm.position.arg) +  \
                         args_const_lut.arg.global_weight * rng_2 *            \
                             (global_best.position.arg - swarm.position.arg);

static void pso_update_velocity(sParticle_t &swarm, sParticle_t &global_best) {
    ap_fixed_32p16 rand_val[2 * PSO_DIMENSION];
    for (int j = 0; j < 2 * PSO_DIMENSION; j++) {
        rand_val[j] = PRNG_16();
    }
    /* Update all arguments */
    CALC_ARG_VELOCITY(alfa0, rand_val[0], rand_val[1])
    CALC_ARG_VELOCITY(freq0, rand_val[2], rand_val[3])
    CALC_ARG_VELOCITY(c2, rand_val[4], rand_val[5])
    CALC_ARG_VELOCITY(n, rand_val[6], rand_val[7])
    CALC_ARG_VELOCITY(ro2, rand_val[8], rand_val[9])
    CALC_ARG_VELOCITY(h, rand_val[10], rand_val[11])
}

#define CALC_ARG_POSITION(arg) swarm.position.arg += swarm.velocity.arg;

static void pso_update_position(sParticle_t &swarm) {
    /* Update all arguments */
    CALC_ARG_POSITION(alfa0)
    CALC_ARG_POSITION(freq0)
    CALC_ARG_POSITION(c2)
    CALC_ARG_POSITION(n)
    CALC_ARG_POSITION(ro2)
    CALC_ARG_POSITION(h)
}

static void pso_update_fitness(sParticle_t &swarm, const sModelParams &params,
                               const ap_fixed_32p16 *ref_signal,
                               const ap_fixed_64p32 *freq_axis) {
    swarm.fitness_current =
        pso_fitness(swarm.position, params, ref_signal, freq_axis);
    if (swarm.fitness_current < swarm.fitness_best) {
        swarm.fitness_best = swarm.fitness_current;
        swarm.position_best = swarm.position;
    }
}

static void pso_swarm_update(sParticle_t *swarm, sParticle_t &global_best,
                             const sModelParams &params,
                             const ap_fixed_32p16 *ref_signal,
                             const ap_fixed_64p32 *freq_axis) {

    /* Update velocity and position */
    for (int i = 0; i < PSO_SWARM_SIZE; i++) {
        pso_update_velocity(swarm[i], global_best);
        pso_update_position(swarm[i]);
        pso_update_fitness(swarm[i], params, ref_signal, freq_axis);
        pso_util_print("swarm", i, swarm[i].position);
    }
}

/* Find best particle in the swarm and return its fitness */
static ap_fixed_32p16 pso_find_global_best(sParticle_t *swarm,
                                           sParticle_t &global_best) {
    for (int i = 0; i < PSO_SWARM_SIZE; i++) {
        if (swarm[i].fitness_best < global_best.fitness_best) {
            global_best = swarm[i];
        }
    }
    return global_best.fitness_best;
}

void pso_process(sModelArgs &args_estimate, const sModelParams &params,
                 const ap_fixed_32p16 *ref_signal,
                 const ap_fixed_64p32 *freq_axis, const uint16_t itterations) {

    sParticle_t swarm[PSO_SWARM_SIZE];
    sParticle_t global_best;
    ap_fixed_32p16 global_best_fitness = 32768;

    pso_swarm_init(swarm, params, ref_signal, freq_axis);
    global_best_fitness = pso_find_global_best(swarm, global_best);

    for (int iter = 0; iter < itterations; iter++) {
        pso_swarm_update(swarm, global_best, params, ref_signal, freq_axis);
        global_best_fitness = pso_find_global_best(swarm, global_best);
        args_estimate = global_best.position_best;
        // pso_util_print("best", args_estimate);
    }
}
