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

#include <stdio.h>
#include "../src/include/fann.h"

#define NUM_TESTS 10


/* Generate randomly generated input, and
     test the Hopfield network using it */
void GenerateTests(struct fann *ann, struct fann_train_data *data)
{
	fann_type *output = NULL;
	fann_type *input  = NULL;
	unsigned int i    = 0;
	unsigned int j    = 0;

	input = calloc(data->num_input, sizeof(fann_type));

	/* RUN RANDOM TESTS */
	for (i=0; i<NUM_TESTS; i++)
	{
		// Generate 'length' numbers in range [-1.0, 1.0]
		for (j=0; j<data->num_input; j++)
		{
			input[j] = fann_rand(-1.0, 1.0);
		}

		// Run the Hopfield net
		output = fann_run_hopfield(ann, input);

		// Display the results!
		printf("TEST: (");
		for (j=0; j<data->num_input-1; j++)
		{
			printf("%0.2f, ", input[j]);
		}

		printf("%0.2f) -> (", input[data->num_input-1]);
		for (j=0; j<data->num_input-1; j++)
		{
			printf("%0.0f, ", output[j]);
		}

		printf("%0.0f)\n", output[data->num_input-1]);
	}
}


int main(void)
{
	struct fann *ann2 = NULL;
	struct fann *ann3 = NULL;
	struct fann_train_data *data2 = NULL;
	struct fann_train_data *data3 = NULL;

	unsigned int i = 0;


	/* LOAD TRAINING FILES */
	data2 = fann_read_train_from_file("hopfield_test2.data");
	data3 = fann_read_train_from_file("hopfield_test3.data");

	if (data2 == NULL || data3 == NULL)
	{
		printf("Error reading data files.\n");
		return 0;
	}

	printf("Num Data (2): %d\n", data2->num_data);
	printf("Num Data (3): %d\n", data3->num_data);


	/* CREATE HOPFIELD NET AND TRAIN ON THE 2-INPUT DATASET */
	/* Network should generate {-1,1} or {1,-1} */
	ann2 = fann_create_hopfield(data2->num_input);
	fann_train_hopfield(ann2, data2);
	GenerateTests(ann2, data2);

	printf("\n");

	/* TRAIN ON THE 3-INPUT DATASET */
	/* Network should generate {1,1,1} or {-1,-1,-1} */
	ann3 = fann_create_hopfield(data3->num_input);
	fann_train_hopfield(ann3, data3);
	GenerateTests(ann3, data3);


	/* CLEAN UP */
	fann_destroy(ann2);
	fann_destroy(ann3);
	return 0;
}
