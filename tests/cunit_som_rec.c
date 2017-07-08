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
#include "cunit_fann.h"
#include "fann.h"
#include "fann_som.h"


#define NUM_INPUT 1000

#define NEAREQUAL(x,y) (fabs(x - y) < 0.000001)

void cunit_som_rec()
{

	FILE *infile;
	const unsigned int width = 10;
	const unsigned int height = 10;
	const unsigned int num_dimensions = 3;
	unsigned int i, j;
	fann_type *cur_modelvector;
	struct fann_som_neuron_private_data *priv;
	fann_type file_weights[3];
	fann_type *modelvector;

	const float desired_error = (const float) 0.001;
	const unsigned int max_epochs = 5;
	const unsigned int epochs_between_reports = 6;
	int match = 1;

	struct fann *ann = fann_create_som(width, height, num_dimensions);
	unsigned int num_weight_val = ann->num_input;

	/* Make sure the initial weights match up with the test data */
	srand(1);
	fann_randomize_weights_som(ann, 0.0f, 1.0f);	

	ann->som_params->som_topology = FANN_SOM_TOPOLOGY_RECTANGULAR;
	ann->som_params->som_neighborhood = FANN_SOM_NEIGHBORHOOD_DISTANCE;
	ann->som_params->som_learning_decay	= FANN_SOM_LEARNING_DECAY_LINEAR;
	ann->som_params->som_learning_rate = 0.01;
	ann->som_params->som_learning_rate_constant = 0.01;

	fann_train_on_file(ann, "som_train.data", max_epochs, epochs_between_reports, desired_error);

	infile = fopen("cunit_som_rec.data", "r");
	if (!infile)
	{
		printf("Error: Could not open 'cunit_som_rec.data'\n");
		exit(0);
	}

	priv = (struct fann_som_neuron_private_data *)ann->first_layer->first_neuron->private_data;
	modelvector = ((struct fann_som_neuron_private_data *)priv)->som_model_vector;

	 /* check the weights */
	 for (j = 0; j < ann->som_params->som_height * ann->som_params->som_width; j++)
	 {
                 cur_modelvector = &modelvector[j * num_weight_val];
		 fscanf(infile, "%f %f %f \n", &file_weights[0], &file_weights[1], &file_weights[2]);
		 for (i = 0; i < num_weight_val; i++)
                 {
		        if (!NEAREQUAL(cur_modelvector[i], file_weights[i])) {
				match = 0;
			}
		 }
	 }

	fann_destroy(ann);

	fclose(infile);

	CU_ASSERT(match);

}
