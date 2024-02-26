#include "model.hpp"
#include "hls_math.h"
#include <stdio.h>

static const fixed_double PI = 3.14159265358979323846;

void TransferFunction(const sModelArgs args, const sModelParams params,
                      const fixed_double *freq_axis, complex_float *tf_out) {
    
    fixed_double decimation = 1e9;
    complex_double imag_unit = complex_double(0, 1);

    fixed_double alf = 0;
    fixed_double alf_1 = 0;
    fixed_double alf_2 = 0;

    /* k - wave number */
    fixed_double k_real = 0;
    fixed_double k_imag = 0;

    fixed_double Z1 = params.c1 * params.ro1;
    fixed_double Z1_dec = Z1 / decimation;
    fixed_double Z2 = args.c2 * args.ro2;
    fixed_double Z2_dec = Z2 / decimation;
    PRINT(Z1, 0);
    PRINT(Z2, 0);

    fixed_double numerator = 2 * Z1 * Z2_dec; // DECIMATED!
    PRINT(numerator, 0);

    fixed_double denominator_sum_sqr = Z1 * Z1_dec + Z2 * Z2_dec; // DECIMATED!
    PRINT(denominator_sum_sqr, 0);

    fixed_double real_kh = 0;
    fixed_double imag_kh = 0;

    fixed_double cos_kh_real = 0;
    fixed_double cos_kh_imag = 0;
    complex_double cos_kh;

    fixed_double sin_kh_real = 0;
    fixed_double sin_kh_imag = 0;
    complex_double sin_kh;

    complex_double denominator;
    complex_double T;


    tf_out[0] = 0;
    for (int i = 1; i < TRANSFER_FUNC_SIZE; i++) {
        /* Calculate alfa - attenuation coefficient */
        alf_1 = hls::abs(freq_axis[i]) / args.freq0;
        // PRINT(alf_1, i);
        alf_2 = hls::pow((float)alf_1, (float)args.n);
        // PRINT(alf_2, i);
        alf = args.alfa0 * alf_2;
        // PRINT(alf, i);

        /* Calculate k - wave number */
        // PRINT(freq_axis[i], i);
        k_real = 2 * PI * freq_axis[i] / args.c2;
        k_imag = alf;
        // PRINT(k_real, i);
        // PRINT(k_imag, i);

        /* Calculate transfer function */
        /* ================================================================= */
        /* Brekhovskikh & Godin 1990 (2.4.18) */
        /* T =  (2 * Z1 * Z2) ./ 
                ((2 * Z1 * Z2) .* cos(k.*h) - 1i .* sin(k.*h) .* (Z1.^2 + Z2.^2)); */
        /* ================================================================= */

        real_kh = k_real * args.h;
        imag_kh = k_imag * args.h;
        // PRINT(real_kh, i);
        // PRINT(imag_kh, i);

        /* Apply Euler's Cosine Identity replacement */
        cos_kh_real = hls::cos(real_kh) * hls::cosh(imag_kh);
        cos_kh_imag = -1 * hls::sin(real_kh) * hls::sinh(imag_kh);
        cos_kh = complex_float(cos_kh_real, cos_kh_imag);
        PRINT_C(cos_kh, i);

        /* Apply Euler's Sine Identity replacement */
        sin_kh_real = hls::sin(real_kh) * hls::cosh(imag_kh);
        sin_kh_imag = hls::cos(real_kh) * hls::sinh(imag_kh);
        sin_kh = complex_float(sin_kh_real, sin_kh_imag);
        PRINT_C(sin_kh, i);

        denominator = (numerator * cos_kh) - (imag_unit * sin_kh * denominator_sum_sqr); // DECIMATED!
        PRINT_C(denominator, i);

        T = numerator / denominator;
        PRINT_C(T, i);
        tf_out[i] = T;
    }
}
