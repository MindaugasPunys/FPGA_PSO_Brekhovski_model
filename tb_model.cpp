//#include "model.hpp"

#include "common_defs.hpp"
#include "test_utils.hpp"

int main() {

	test_utils util;

	/* Read test input values */
	fixed_double in_args[ARGS_SIZE] = {0.0f};
	util.file_read_float(ARGS_DIR, &in_args[0], ARGS_SIZE);
	util.print_float("in_args", &in_args[0], ARGS_SIZE);

	fixed_double in_params[PARAMS_SIZE] = {0.0f};
	util.file_read_float(PARAMS_DIR, &in_params[0], PARAMS_SIZE);
	util.print_float("in_params", &in_params[0], PARAMS_SIZE);

	fixed_double in_freq_axis[FREQ_AXIS_SIZE] = {0.0f};
	util.file_read_float(FREQ_AXIS_DIR, &in_freq_axis[0], FREQ_AXIS_SIZE);
	util.print_float("in_freq_axis", &in_freq_axis[0], 10);

	/* Read expected output values */
	complex_double out_complex[TRANSFER_FUNC_SIZE];
	util.file_read_complex(TRANSFER_FUN_RE_DIR, TRANSFER_FUN_IM_DIR, &out_complex[0], TRANSFER_FUNC_SIZE);
	util.print_complex("out_complex", &out_complex[0], 10);

	/* Call the model */

	/* Compare */

	return 0;
}
