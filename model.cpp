#include "model.hpp"
#include "hls_math.h"
#include <stdio.h>

static const fixed_double PI = 3.14159265358979323846;

void TransferFunction(const sModelArgs args, const sModelParams params,
                      const fixed_double *freq_axis, complex_float *tf_out) {
    
    fixed_big_big_float decimation = 1000000;
    complex_big_big_float imag_unit = complex_big_big_float(0, 1);

    fixed_double alf = 0;
    fixed_double alf_1 = 0;
    fixed_double alf_2 = 0;

    /* k - wave number */
    fixed_double k_real = 0;
    fixed_double k_imag = 0;

    fixed_big_big_float Z1 = params.c1 * params.ro1;
    fixed_big_big_float Z2 = args.c2 * args.ro2;

    fixed_big_big_float numerator = 2 * Z1 * Z2;
    fixed_big_big_float denominator_sum_sqr = Z1 * Z1 + Z2 * Z2;
    complex_big_big_float denominator;
    complex_big_big_float T;
    

    fixed_float real_kh = 0;
    fixed_float imag_kh = 0;

    fixed_float cos_kh_real = 0;
    fixed_float cos_kh_imag = 0;
    complex_big_big_float cos_kh;

    fixed_float sin_kh_real = 0;
    fixed_float sin_kh_imag = 0;
    complex_big_big_float sin_kh;

    printf("Z1: %f, Z2: %f, numerator: %f\n", (double)Z1, (double)Z2,
           (double)numerator);


    tf_out[0] = 0;
    for (int i = 1; i < TRANSFER_FUNC_SIZE; i++) {
        /* Calculate alfa - attenuation coefficient */
        alf_1 = hls::abs(freq_axis[i]) / args.freq0;
        alf_2 = hls::pow((float)alf_1, (float)args.n);
        alf = args.alfa0 * alf_2;
        // printf("alf_1: %f, alf_2: %f, alf: %f\n", (float)alf_1, (float)alf_2,
        // (float)alf);

        /* Calculate k - wave number */
        k_real = 2 * PI * freq_axis[i] / args.c2;
        k_imag = alf;
        // printf("freq_axis[%d]: %f, k_real: %f, k_imag: %f\n", i,
        //        (double)freq_axis[i], (double)k_real, (double)k_imag);

        /* Calculate transfer function */
        /* Brekhovskikh & Godin 1990 (2.4.18) */
        /* T =  (2 * Z1 * Z2) ./ 
                ((2 * Z1 * Z2) .* cos(k.*h) - 1i .* sin(k.*h) .* (Z1.^2 + Z2.^2)); */

        real_kh = k_real * args.h;
        imag_kh = k_imag * args.h;
        // printf("kh[%d]: %f, %f\n", i, (double)real_kh, (double)imag_kh);

        /* Apply Euler's Cosine Identity replacement */
        cos_kh_real = hls::cos(real_kh) * hls::cosh(imag_kh);
        cos_kh_imag = -1 * hls::sin(real_kh) * hls::sinh(imag_kh);
        cos_kh = complex_float(cos_kh_real, cos_kh_imag);
        // printf("cos_kh[%d]: %f, %f\n", i, (double)cos_kh.real(), (double)cos_kh.imag());

        /* Apply Euler's Sine Identity replacement */
        sin_kh_real = hls::sin(real_kh) * hls::cosh(imag_kh);
        sin_kh_imag = hls::cos(real_kh) * hls::sinh(imag_kh);
        sin_kh = complex_float(sin_kh_real, sin_kh_imag);
        // printf("sin_kh[%d]: %f, %f\n", i, (double)sin_kh.real(), (double)sin_kh.imag());

        denominator = (numerator * cos_kh) - (imag_unit * sin_kh * denominator_sum_sqr);
        printf("denominator[%d]: %f, %f\n", i, (double)denominator.real(), (double)denominator.imag());
        denominator = denominator / decimation;
        // printf("%f, %f\n", (double)(imag_unit * sin_kh).real(), (double)(imag_unit * sin_kh).imag());
        // printf("%e, %e\n", (double)denominator.real(), (double)denominator.imag());

        T = numerator / denominator;
        T = T / decimation;
        tf_out[i] = complex_float(T.real(), T.imag());
        printf("T[%d]: %f, %f\n", i, (double)T.real(), (double)T.imag());
        // printf("%e / %e %ei = %e %ei\n", (double)numerator,
        //        (double)denominator.real(), (double)denominator.imag(),
        //        (double)tf_out[i].real(), (double)tf_out[i].imag());
        // printf("%f / %f = %f\n", (double)numerator.imag(),
        // (double)denominator.imag(), (double)tf_out[i].imag());
        // printf("tf_out[%d]: %f, %f\n", i, (double)tf_out[i].real(),
        // (double)tf_out[i].imag());
    }
}
