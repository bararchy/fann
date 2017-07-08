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

#include "../src/include/fann.h"
#include <assert.h>
#include <memory.h>

#define MIMO_FANN

#undef ENABLE_RTRL			// To enable RTRL training (NOTE: DEBUGGING)

#define MAX_MSE		0.001	// MSE threshold to stop training
#define MAX_ITERS	100000	// Max iterations to stop training
#define LEARNING_RATE 0.2	// Learning rate during training


/* Generate training outputs where each output is a XOR
     of the two inputs 'delay' time steps ago. */
void rtrl_xor_data (struct fann_train_data *data, unsigned int delay)
{
	unsigned int pos = 0;

	/* Generate outputs */
	for (pos=0; pos<data->num_data; pos++)
	{
		data->input[pos][0] = floor(pos%4 / 2);
		data->input[pos][1] = pos%2;

		if (pos >= delay)
		{
			data->output[pos][0] = (int)data->input[pos-1][0] ^ (int)data->input[pos-1][1];
		}
		else
		{
			data->output[pos][0] = 0;
		}
	}
}


/* Run network so that we can test for correct
     MSE computation and run computation */
void TestRun(struct fann *ann)
{
	struct fann_layer  *layer  = NULL;
	struct fann_neuron *neuron = NULL;
	unsigned int i = 0;
	fann_type *desired_output = NULL;

	assert(ann != NULL);
	assert(ann->first_layer != NULL);

	layer = ann->first_layer;
	neuron = layer->first_neuron;

	// Create desired outputs of 0
	desired_output = calloc(ann->num_output, sizeof(fann_type));

	// Note that network outputs are the
	//   last 'ann->num_outputs' neurons!
	printf("\nNETWORK OUTPUTS (pre-run, last neuron only):\n");
	for (i=0; i<ann->num_output; i++)
	{
		printf("%f ", ann->output[i]);
	}
	printf("\n");

	// Compute MSE initially
	fann_reset_MSE(ann);
	fann_compute_MSE_fully_recurrent(ann, desired_output);
	printf("MSE: %f (counted %d)\n", fann_get_MSE(ann), ann->training_params->num_MSE);

	// Display weights
	for (neuron=ann->first_layer->first_neuron; neuron != ann->first_layer->last_neuron; neuron++)
	{
		printf("\nNeuron %d:\n", neuron - ann->first_layer->first_neuron);

		for (i=0; i<neuron->num_weights; i++)
		{
			// Inputs come from net inputs and neurons
			//   +1 for bias input neuron
			if (i < ann->num_input + 1)
			{
				printf("Weight %d: %f; Input %d: %f\n", 
					i, neuron->weights[i], i, layer->inputs[i]);
			}
			else
			{
				printf("Weight %d: %f; Neuron %d: %f\n",
					i, neuron->weights[i], i - ann->num_input - 1, 
					layer->outputs[i - ann->num_input - 1]);
			}
		}
	}

	// Run network
	layer->run(ann, layer);
	
	// Display afterward outputs
	printf("\nNETWORK OUTPUTS (post-run, last neuron only):\n");
	for (i=0; i<ann->num_output; i++)
	{
		printf("%f ", ann->output[i]);
	}
	printf("\n");

	// Display final MSE
	fann_reset_MSE(ann);
	fann_compute_MSE_fully_recurrent(ann, desired_output);
	printf("MSE: %f (counted %d)\n", fann_get_MSE(ann), ann->training_params->num_MSE);
}


int main()
{
	struct fann_train_data* data = NULL;
	struct fann *ann    = NULL;
    fann_type *calc_out = NULL;
	fann_type *output   = NULL;
	unsigned int i      = 0;

	/* Generate training data */
	data = fann_create_train(100000, 2, 1);
	rtrl_xor_data(data, 1);

	/* Create the recurrent net and train it */
	ann = fann_create_fully_recurrent(3, 2, 1);

	// Run the network for training
	TestRun(ann);

	// Run RTRL training if enabled
#ifdef ENABLE_RTRL
	printf("Training RTRL ...\n");
	fann_train_rtrl(ann, data, MAX_MSE, MAX_ITERS, LEARNING_RATE);

	// XOR Test after RTRL
	for(i = 0; i < fann_length_train_data(data); i++)
	{
		fann_reset_MSE(ann);
		calc_out = fann_test(ann, data->input[i], data->output[i]);
#ifdef FIXEDFANN
		printf("XOR test (%d, %d) -> %d, should be %d, difference=%f\n",
			   data->input[i][0], data->input[i][1], calc_out[0], data->output[i][0],
			   (float) fann_abs(calc_out[0] - data->output[i][0]) / fann_get_multiplier(ann));

		if((float) fann_abs(calc_out[0] - data->output[i][0]) / fann_get_multiplier(ann) > 0.2)
		{
			printf("Test failed\n");
			ret = -1;
		}
#else
		printf("XOR test (%f, %f) -> %f, should be %f, difference=%f\n",
			   data->input[i][0], data->input[i][1], calc_out[0], data->output[i][0],
			   (float) fann_abs(calc_out[0] - data->output[i][0]));
    }
#endif
#endif


	// Display final statistics
    /*fann_print_connections(ann);
    fann_print_parameters(ann);
	fann_destroy(ann); */

	return 0;
}

/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap noet
 */
