#include "common_defs.hpp"
#include "model.hpp"
#include "test_utils.hpp"

int main() {

	test_utils util;

	/* Read test input values */
	fixed_double in_args[ARGS_SIZE] = {0.0f};
	util.file_read_float(ARGS_DIR, &in_args[0], ARGS_SIZE);
	util.print_float("in_args", &in_args[0], ARGS_SIZE);
	sModelArgs args = {in_args[0], in_args[1], in_args[2], in_args[3], in_args[4], in_args[5]};

	fixed_double in_params[PARAMS_SIZE] = {0.0f};
	util.file_read_float(PARAMS_DIR, &in_params[0], PARAMS_SIZE);
	util.print_float("in_params", &in_params[0], PARAMS_SIZE);
	sModelParams params = {in_params[0], in_params[1], in_params[2]};

	fixed_double in_freq_axis[FREQ_AXIS_SIZE] = {0.0f};
	util.file_read_float(FREQ_AXIS_DIR, &in_freq_axis[0], FREQ_AXIS_SIZE);
	util.print_float("in_freq_axis", &in_freq_axis[0], 10);

	/* Read expected output values */
	complex_double tf_out[TRANSFER_FUNC_SIZE];
	util.file_read_complex(TRANSFER_FUN_RE_DIR, TRANSFER_FUN_IM_DIR, &tf_out[0], TRANSFER_FUNC_SIZE);
	util.print_complex("tf_out", &tf_out[0], 10);

	/* Call the model */
	complex_float tf_result[TRANSFER_FUNC_SIZE];
	TransferFunction(args, params, in_freq_axis, tf_result);
//	util.print_complex("tf_result", tf_result, 10);

	return 0;
}
