#include "model.hpp"
#include "hls_math.h"
#include <stdio.h>

static const fixed_double PI = 3.14159265358979323846;

void TransferFunction(const sModelArgs args, const sModelParams params,
                      const fixed_double *freq_axis, complex_float *tf_out) {

    fixed_float alf = 0;
    fixed_double alf_1 = 0;
    fixed_double alf_2 = 0;

    complex_float k;
    fixed_double k_real = 0;
    fixed_double k_imag = 0;

    fixed_float Z1 = params.c1 * params.ro1;
    fixed_float Z2 = args.c2 * args.ro2;

    fixed_double numerator = 4 * Z1 * Z2;
    fixed_double denominator = 0;
    // fixed_float 

    // (Z1 - Z2).^2 = 1.4726e+12
    // (Z1 + Z2).^2 = 1.4746e+12


    tf_out[0] = 0;
    for (int i = 1; i < TRANSFER_FUNC_SIZE; i++) {
        /* Calculate alfa - attenuation coefficient */
        alf_1 = hls::abs(freq_axis[i]) / args.freq0;
        alf_2 = hls::pow((float)alf_1, (float)args.n);
        alf = args.alfa0 * alf_2;
        // printf("alf_1: %f, alf_2: %f, alf: %f\n", (float)alf_1, (float)alf_2, (float)alf);

        /* Calculate k - wave number */
        k_real = 2 * PI * freq_axis[i] / args.c2;
        k_imag = alf;
        k = complex_float(k_real, k_imag);
        printf("freq_axis[%d]: %f, k_real: %f, k_imag: %f\n", i, (double)freq_axis[i], (double)k_real, (double)k_imag);

        /* Calculate velocity */
        // veloc = args.c2

        /* Calculate transfer function */
//        tf_out[i] = numerator / denominator;
    }
}
