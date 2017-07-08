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

#include "fann.h"
#include <string.h>

#define NUM_INPUT 1000

int main(int argc, char *argv[])
{
	int i = 0;
	const unsigned int width = 10;
	const unsigned int height = 10;
	const unsigned int num_dimensions = 3;
	
	const float desired_error = (const float) 0.001;
	const unsigned int max_epochs = 500000;
	const unsigned int epochs_between_reports = 1000;

	struct fann *ann = fann_create_som(width, height, num_dimensions);

	ann->som_params->som_topology = FANN_SOM_TOPOLOGY_HEXAGONAL;
	ann->som_params->som_neighborhood = FANN_SOM_NEIGHBORHOOD_DISTANCE;
	ann->som_params->som_learning_decay	= FANN_SOM_LEARNING_DECAY_LINEAR;
	ann->som_params->som_learning_rate = 0.01;
	ann->som_params->som_learning_rate_constant = 0.01;

	/* Example using init_weights */
	struct fann_train_data *data = fann_read_train_from_file("som_train.data");

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-init") == 0)
			fann_init_weights_som(ann, data);
		else if (strcmp(argv[i], "-epoch") == 0) {
			fann_train_epoch(ann, data);
			return EXIT_SUCCESS;
		}
	}

	/* Example without init_weights (random initialization)*/
	fann_train_on_file(ann, "som_train.data", max_epochs, epochs_between_reports, desired_error);

	/*fann_save(ann, "som_train.net"); */

	fann_destroy(ann);

	printf("\n");
	return 0;
}
