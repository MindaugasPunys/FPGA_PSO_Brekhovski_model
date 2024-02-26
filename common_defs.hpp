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

#define TEST_DIR "D:/Magistras/Magistro projektas/Matlab_realization/Raw/test/TransferFunction/"
#define ARGS_DIR TEST_DIR "OriginalArgs.txt"
#define PARAMS_DIR TEST_DIR "acquisition_parameters.txt"
#define FREQ_AXIS_DIR TEST_DIR "freq_axis.txt"
#define TRANSFER_FUN_RE_DIR TEST_DIR "T_re.txt"
#define TRANSFER_FUN_IM_DIR TEST_DIR "T_im.txt"
/* ================================================================= */
/* MACROS */
/* ================================================================= */

#ifdef DEBUG
#define PRINT(variable, i) printf(#variable "[%d]: %f\n", i, (double)variable);
#define PRINT_C(variable, i)                                                   \
    printf(#variable "[%d]: %f, %f i\n", i, (double)variable.real(),             \
           (double)variable.imag());
#else
#define PRINT(variable, i)
#define PRINT_C(variable, i)
#endif

/* ================================================================= */
/* TYPEDEFS */
/* ================================================================= */
typedef ap_fixed<32, 16> fixed_float;
typedef ap_fixed<48, 32> fixed_big_float;
typedef ap_fixed<64, 48> fixed_big_big_float;
typedef ap_fixed<64, 32> fixed_double;

typedef std::complex<fixed_float> complex_float;
typedef std::complex<fixed_big_float> complex_big_float;
typedef std::complex<fixed_big_big_float> complex_big_big_float;
typedef std::complex<fixed_double> complex_double;

struct sModelArgs {
    fixed_float alfa0;
    fixed_double freq0;
    fixed_float c2;
    fixed_float n;
    fixed_float ro2;
    fixed_float h;
};

struct sModelParams {
    fixed_double fsampl;
    fixed_float c1;
    fixed_float ro1;
};
