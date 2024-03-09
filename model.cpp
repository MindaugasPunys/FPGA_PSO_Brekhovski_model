#include "model.hpp"
#include "hls_math.h"
#include <stdio.h>

#define DECIMATION_SHIFT 30
// static const ap_fixed_64p32 PI = 3.14159265358979323846;
static const ap_fixed_32p16 PI_x_2 = 6.283185;

void TransferFunction(const sModelArgs args, const sModelParams params,
                      const ap_fixed_64p32 *freq_axis, ap_complex_32p16 *tf_out) {
    
    const ap_complex_32p16 imag_unit = ap_complex_32p16(0, 1);

    ap_fixed_32p16 alf = 0;
    ap_fixed_32p16 alf_1 = 0;
    ap_fixed_32p16 alf_2 = 0;

    /* k - wave number */
    ap_fixed_32p16 k_real = 0;
    ap_fixed_32p16 k_imag = 0;

    ap_fixed_32p16 Z1 = params.c1 * params.ro1;
    PRINT(Z1, 0);
    ap_fixed_32p16 Z1_dec = Z1 >> DECIMATION_SHIFT;
    PRINT(Z1_dec, 0);
    ap_fixed_64p32 Z2 = args.c2 * args.ro2;
    PRINT(Z2, 0);
    ap_fixed_32p16 Z2_dec = Z2 >> DECIMATION_SHIFT;
    PRINT(Z2_dec, 0);

    ap_fixed_64p32 numerator = Z1 * Z2_dec; // DECIMATED!
    PRINT(numerator, 0);

    ap_fixed_32p16 denominator_sum_sqr = Z1 * Z1_dec + Z2 * Z2_dec; // DECIMATED!
    PRINT(denominator_sum_sqr, 0);

    ap_fixed_32p16 real_kh = 0;
    ap_fixed_32p16 imag_kh = 0;

    ap_fixed_32p16 cos_kh_real = 0;
    ap_fixed_32p16 cos_kh_imag = 0;
    ap_complex_32p16 cos_kh;

    ap_fixed_32p16 sin_kh_real = 0;
    ap_fixed_32p16 sin_kh_imag = 0;
    ap_complex_32p16 sin_kh;

    ap_complex_64p32 denominator;
    ap_complex_64p32 T;

    tf_out[0] = 0;
    for (int i = 1; i < TRANSFER_FUNC_SIZE; i++) {
        /* Calculate alfa - attenuation coefficient */
        alf_1 = hls::abs(freq_axis[i]) / args.freq0;
        PRINT(alf_1, i);
        alf_2 = hls::pow((float)alf_1, (float)args.n);
        PRINT(alf_2, i);
        alf = args.alfa0 * alf_2;
        PRINT(alf, i);

        /* Calculate k - wave number */
        // PRINT(freq_axis[i], i);
        k_real = PI_x_2 * freq_axis[i] / args.c2;
        k_imag = alf;
        PRINT(k_real, i);
        PRINT(k_imag, i);

        /* Calculate transfer function */
        /* ================================================================= */
        /* Brekhovskikh & Godin 1990 (2.4.18) */
        /* T =  (2 * Z1 * Z2) ./ 
                ((2 * Z1 * Z2) .* cos(k.*h) - 1i .* sin(k.*h) .* (Z1.^2 + Z2.^2)); */
        /* ================================================================= */

        real_kh = k_real * args.h;
        imag_kh = k_imag * args.h;
        PRINT(real_kh, i);
        PRINT(imag_kh, i);

        /* Apply Euler's Cosine Identity replacement */
        cos_kh_real = hls::cos(real_kh) * hls::cosh(imag_kh);
        cos_kh_imag = -1 * hls::sin(real_kh) * hls::sinh(imag_kh);
        cos_kh = ap_complex_32p16(cos_kh_real, cos_kh_imag);
        PRINT_C(cos_kh, i);

        /* Apply Euler's Sine Identity replacement */
        sin_kh_real = hls::sin(real_kh) * hls::cosh(imag_kh);
        sin_kh_imag = hls::cos(real_kh) * hls::sinh(imag_kh);
        sin_kh = ap_complex_32p16(sin_kh_real, sin_kh_imag);
        PRINT_C(sin_kh, i);

        denominator = ((ap_fixed_32p16)numerator * cos_kh) - (imag_unit * sin_kh * denominator_sum_sqr); // DECIMATED!
        PRINT_C(denominator, i);

        T = numerator / denominator;
        PRINT_C(T, i);
        tf_out[i] = T;
    }
}

void WaveSynthesize(const sModelArgs args, const sModelParams params,
                    const ap_fixed_64p32
                    const ap_fixed_64p32 *freq_axis, ap_complex_32p16 *tf_out) {

    const ap_complex_32p16 imag_unit = ap_complex_32p16(0, 1);
    
}