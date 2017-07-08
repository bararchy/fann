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

#define NUM_TESTS 1


int main(void)
{
	struct fann *ann = NULL;
	unsigned int i = 0;
	unsigned int num_neurons = 0;
	unsigned int num_inputs  = 0;
	unsigned int num_outputs = 0;

	fann_seed_rand();

	/* GENERATE RANDOM RECURRENT NETWORKS */
	for (i=0; i<NUM_TESTS; i++)
	{
		num_inputs  = rand()%5;
		num_neurons = rand()%10 + 1;

		// Number of outputs must be <= number of neurons
		do { num_outputs = rand()%10; } while (num_outputs > num_neurons);

		ann = fann_create_fully_recurrent(num_neurons, num_inputs, num_outputs);

		printf("\n\nCreated recurrent network (neurons=%d, inputs=%d, outputs=%d)\n",
			num_neurons, num_inputs, num_outputs);

		printf("\n-------------------------------------------\n");
		printf("NET %d - CONNECTION MATRIX:\n", i);
		printf("  (I=input, b=bias, O=output, #=internal)\n");
		printf("-------------------------------------------\n");
		fann_print_connections(ann);

		printf("\n\n-------------------------------------------\n");
		printf("NET %d - ANN PARAMETERS:\n", i);
		printf("-------------------------------------------\n");
		fann_print_parameters(ann);

		/* CLEAN UP */
		fann_destroy(ann);
	}

	return 0;
}
