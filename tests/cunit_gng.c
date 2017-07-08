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
#include "fann_gng.h"

#define NEAREQUAL(x,y) (fabs(x - y) < 0.000001)

void cunit_gng()
{

	FILE *infile;
	const unsigned int num_dimensions = 2;
	unsigned int i, j, numcells, filecells;
	fann_type *cur_modelvector;
	struct fann_gng_neuron_private_data *priv;
	fann_type file_weights[2];
	fann_type *modelvector;
	int match = 1;

	const unsigned int max_epochs = 5;
	const unsigned int epochs_between_reports = 6;

	struct fann *ann = fann_create_gng(num_dimensions);
	unsigned int num_weight_val = ann->num_input;

	/* Make sure the initial weights match up with the test data */
	srand(1);
	fann_randomize_weights_gng(ann, 0.0f, 1.0f);	

	fann_train_on_file(ann, "gng_train.data", max_epochs, epochs_between_reports, desired_error);
	numcells = ann->gng_params->gng_num_cells;

	infile = fopen("cunit_gng.data", "r");
	if (!infile)
	{
		printf("Error: Could not open 'cunit_gng.data'\n");
		exit(0);
	}
	fscanf(infile, "%d\n", &filecells);
	if (filecells != numcells) {
		match = 0;
	}

	priv = (struct fann_gng_neuron_private_data *)ann->first_layer->first_neuron->private_data;

	/* check the weights */
	for (j = 0; j < numcells; j++)
	{
                 cur_modelvector = &priv->gng_cell_location[j];
	 	 fscanf(infile, "%f %f\n", &file_weights[0], &file_weights[1]);
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
