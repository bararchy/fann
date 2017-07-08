/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003 Steffen Nissen (lukesky@diku.dk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <CUnit/CUnit.h>
#include <stdio.h>

#include "fann.h"
#include "cunit_fann.h"

void cunit_xor_test(void)
{
	fann_type *calc_out = NULL;
	unsigned int i;
	int ret = 0;

	struct fann *ann = NULL;
	struct fann_train_data *data = NULL;

#ifdef FIXEDFANN
	ann = fann_create_from_file("xor_fixed.net");
#else
	ann = fann_create_from_file("xor_float.net");
#endif

	CU_ASSERT_PTR_NOT_NULL_FATAL(ann);

#ifdef FIXEDFANN
	data = fann_read_train_from_file("xor_fixed.data");
#else
	data = fann_read_train_from_file("xor.data");
#endif

	CU_ASSERT_PTR_NOT_NULL_FATAL(data);

	for(i = 0; i < fann_length_train_data(data); i++)
	{
		fann_reset_MSE(ann);
		calc_out = fann_test(ann, data->input[i], data->output[i]);

		CU_ASSERT_PTR_NOT_NULL_FATAL(calc_out);

#ifdef FIXEDFANN
		/*printf("XOR test (%d, %d) -> %d, should be %d, difference=%f\n",
			   data->input[i][0], data->input[i][1], calc_out[0], data->output[i][0],
			   (float) fann_abs(calc_out[0] - data->output[i][0]) / fann_get_multiplier(ann));*/

		if((float) fann_abs(calc_out[0] - data->output[i][0]) / fann_get_multiplier(ann) > 0.2)
		{
			CU_FAIL("XOR test failed.");
			ret = -1;
		}
#else
		/*printf("XOR test (%f, %f) -> %f, should be %f, difference=%f\n",
			   data->input[i][0], data->input[i][1], calc_out[0], data->output[i][0],
			   (float) fann_abs(calc_out[0] - data->output[i][0]));*/
#endif
	}

	fann_destroy_train(data);
	fann_destroy(ann);
}
