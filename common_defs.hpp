#pragma once

#include "ap_fixed.h"
#include <complex>
/* ================================================================= */
/* DEFINES */
/* ================================================================= */
#define DEBUG

#define ARGS_SIZE 6
#define PARAMS_SIZE 3
#define FREQ_AXIS_SIZE 2048
#define TRANSFER_FUNC_SIZE 2048

#define TEST_DIR                                                               \
    "D:/Magistras/Magistro "                                                   \
    "projektas/Matlab_realization/Raw/test/TransferFunction/"
#define ARGS_DIR TEST_DIR "OriginalArgs.txt"
#define PARAMS_DIR TEST_DIR "acquisition_parameters.txt"
#define FREQ_AXIS_DIR TEST_DIR "freq_axis.txt"
#define TRANSFER_FUN_RE_DIR TEST_DIR "T_re.txt"
#define TRANSFER_FUN_IM_DIR TEST_DIR "T_im.txt"

#define WAVE_DIR TEST_DIR "Wave.txt"
#define REFRENCE_DIR TEST_DIR "Reference.txt"
/* ================================================================= */
/* MACROS */
/* ================================================================= */

#ifdef DEBUG
#define PRINT(variable, i) printf(#variable "[%d]: %f\n", i, (double)variable);
#define PRINT_C(variable, i)                                                   \
    printf(#variable "[%d]: %f, %f i\n", i, (double)variable.real(),           \
           (double)variable.imag());
#define PRINT_ARRAY(variable, size)                                            \
    for (int i = 0; i < size; i++) {                                           \
        printf(#variable "[%d]: %f\n", i, (double)variable[i]);                \
    }
#define PRINT_ARRAY_C(variable, size)                                          \
    for (int i = 0; i < size; i++) {                                           \
        printf(#variable "[%d]: %f, %f i\n", i, (double)variable[i].real(),    \
               (double)variable[i].imag());                                    \
    }
#else
#define PRINT(variable, i)
#define PRINT_C(variable, i)
#define PRINT_ARRAY(variable, size)
#define PRINT_ARRAY_C(variable, size)
#endif

/* ================================================================= */
/* TYPEDEFS */
/* ================================================================= */
typedef ap_fixed<32, 16> ap_fixed_32p16; // ap_fixed_32p16
typedef ap_fixed<48, 32> ap_fixed_48p32; // ap_fixed_48p32
typedef ap_fixed<64, 48> ap_fixed_64p48; // ap_fixed_64p48
typedef ap_fixed<64, 32> ap_fixed_64p32; // ap_fixed_64p32

typedef std::complex<ap_fixed_32p16> ap_complex_32p16; // ap_complex_32p16
typedef std::complex<ap_fixed_48p32> ap_complex_48p32; // ap_complex_48p32
typedef std::complex<ap_fixed_64p48> ap_complex_64p48; // ap_complex_64p48
typedef std::complex<ap_fixed_64p32> ap_complex_64p32; // ap_complex_64p32

struct sModelArgs {
    ap_fixed_32p16 alfa0; /* Attenuation coefficient */
    ap_fixed_64p32 freq0; /* Normalized resonant frequency */
    ap_fixed_32p16 c2;    /* Speed of sound in the second medium */
    ap_fixed_32p16 n;     /* Attenuation exponent */
    ap_fixed_32p16 ro2;   /* Density of the second medium */
    ap_fixed_64p32 h;     /* Thickness of the second medium */
};

struct sModelParams {
    ap_fixed_64p32 fsampl; /* Sampling frequency */
    ap_fixed_32p16 c1;     /* Speed of sound in the first medium */
    ap_fixed_32p16 ro1;    /* Density of the first medium */
};
