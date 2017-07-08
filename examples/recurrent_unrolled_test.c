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
	unsigned int j = 0;
	unsigned int num_neurons = 0;
	unsigned int time_steps  = 0;
	
	fann_type weights[1000];

	fann_seed_rand();

	/* GENERATE RANDOM RECURRENT NETWORKS */
	for (i=0; i<NUM_TESTS; i++)
	{
		// Select number of neurons and time steps
		num_neurons = rand()%8 + 2;
		time_steps  = rand()%5 + 2;
		
		// Generate random weights
		for (j=0; j<num_neurons*(num_neurons+1); j++)
		{
			weights[j] = fann_rand(-10.0, 10.0);
		}
			
		// Create the unrolled network!
		printf("\n\nCreating unrolled recurrent network (neurons=%d, steps=%d) ...",
			num_neurons, time_steps);
		ann = fann_create_unrolled_recurrent(num_neurons, weights, time_steps);

		printf("\n-------------------------------------------\n");
		printf("NET %d - CONNECTION MATRIX:\n", i);
		printf("  (NOTE: Weight matrix is 'n+1' neurons\n");
		printf("      (for bias) times 'n' connections)\n");
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
