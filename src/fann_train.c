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
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "config.h"
#include "fann.h"

/*#define DEBUGTRAIN*/

#ifndef FIXEDFANN
/* INTERNAL FUNCTION
  Calculates the activation of a value, given an activation function
   and a steepness
*/
fann_type fann_activation(struct fann * ann, unsigned int activation_function, fann_type steepness,
						  fann_type value)
{
	value = fann_mult(steepness, value);
	fann_activation_switch(activation_function, value, value);
	return value;
}

/* 
 * Trains the network with the backpropagation algorithm.
 */
FANN_EXTERNAL void FANN_API fann_train(struct fann *ann, fann_type * input, fann_type * desired_output)
{
	fann_run(ann, input);
	fann_compute_MSE(ann, desired_output);
	fann_backpropagate_MSE(ann);
	fann_update_weights(ann);
}
#endif


/* Tests the network.
 */
FANN_EXTERNAL fann_type *FANN_API fann_test(struct fann *ann, fann_type * input,
											fann_type * desired_output)
{
	fann_type *output_begin = fann_run(ann, input);
	struct fann_layer *last_layer;
	struct fann_neuron *neuron_it, *last_neuron;

	last_layer = ann->last_layer - 1;
	if (last_layer->train_errors==NULL)
		last_layer->initialize_train_errors(ann, last_layer);
	last_neuron = last_layer->last_neuron;

	for(neuron_it = last_layer->first_neuron; neuron_it != last_neuron; neuron_it++)
		neuron_it->compute_error(ann, neuron_it, desired_output);
	
	return output_begin;
}

/* get the mean square error.
 */
FANN_EXTERNAL float FANN_API fann_get_MSE(struct fann *ann)
{
	if(ann->training_params->num_MSE)
	{
		return ann->training_params->MSE_value / (float) ann->training_params->num_MSE;
	}
	else
	{
		return 0;
	}
}

FANN_EXTERNAL unsigned int FANN_API fann_get_bit_fail(struct fann *ann)
{
	return ann->training_params->num_bit_fail;	
}

/* reset the mean square error.
 */
FANN_EXTERNAL void FANN_API fann_reset_MSE(struct fann *ann)
{
/*printf("resetMSE %d %f\n", ann->num_MSE, ann->MSE_value);*/
	ann->training_params->num_MSE = 0;
	ann->training_params->MSE_value = 0;
	ann->training_params->num_bit_fail = 0;
}

#ifndef FIXEDFANN

/* INTERNAL FUNCTION
	compute the error at the network output
	(usually, after forward propagation of a certain input vector, fann_run)
	the error is a sum of squares for all the output units
	also increments a counter because MSE is an average of such errors

	After this train_errors in the output layer will be set to:
	neuron_value_derived * (desired_output - neuron_value)
 */
void fann_compute_MSE(struct fann *ann, fann_type *desired_output)
{
	struct fann_layer *last_layer;
	struct fann_neuron *neuron_it, *last_neuron;

	/* if no room allocated for the error variables, allocate it now (lazy allocation) */
	last_layer = ann->last_layer - 1;
	if (last_layer->train_errors==NULL)
		last_layer->initialize_train_errors(ann, last_layer);

	last_neuron = last_layer->last_neuron;
	for(neuron_it = last_layer->first_neuron; neuron_it != last_neuron; neuron_it++)
		neuron_it->compute_error(ann, neuron_it, desired_output);
}

/* INTERNAL FUNCTION
   Propagate the error backwards from the output layer.

   After this the train_errors in the hidden layers will be:
   neuron_value_derived * sum(outgoing_weights * connected_neuron)
*/
void fann_backpropagate_MSE(struct fann *ann)
{
	struct fann_layer *layer_it, *first_layer, *last_layer;
	struct fann_neuron *neuron_it, *last_neuron;
	fann_type *prev_layer_errors;

	first_layer = ann->first_layer;
	last_layer = ann->last_layer;
	
	/* Note: the last layer has already been initialized using
   * Skip the first layer. It can't backpropagate, just needs to update the deltas. */
	for (layer_it = last_layer-1; layer_it != first_layer; layer_it--)
	{
    /* initailize the prevoius layer */
    if ((layer_it-1)->train_errors==NULL)
      (layer_it-1)->initialize_train_errors(ann, layer_it-1);
    else
    {
      /* clear just the error variables */
      memset((layer_it-1)->train_errors, 0, ((layer_it-1)->num_outputs) * sizeof(fann_type));
    }
		
    last_neuron = layer_it->last_neuron;
		prev_layer_errors = (layer_it - 1)->train_errors;
		for (neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
			neuron_it->backpropagate(ann, neuron_it, prev_layer_errors);
	}
	/* Iterate only over the first layer (layer_it == first_layer) */
	last_neuron = first_layer->last_neuron;
	for (neuron_it = first_layer->first_neuron; neuron_it != last_neuron; neuron_it++)
		neuron_it->backpropagate(ann, neuron_it, NULL);
}

/* INTERNAL FUNCTION
   Update weights for incremental training
*/
void fann_update_weights(struct fann *ann)
{
	struct fann_layer *layer_it, *first_layer, *last_layer;
	struct fann_neuron *neuron_it, *last_neuron;

	first_layer = ann->first_layer;
	last_layer = ann->last_layer;
	
	for (layer_it = first_layer; layer_it != last_layer; layer_it++)
	{
		last_neuron = layer_it->last_neuron;
		for (neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
            neuron_it->update_weights(ann, neuron_it);
	}
}

#if 0
/* INTERNAL FUNCTION
   Update slopes for batch training
   layer_begin = ann->first_layer+1 and layer_end = ann->last_layer-1
   will update all slopes.

*/
void fann_update_slopes_batch(struct fann *ann, struct fann_layer *layer_begin,
							  struct fann_layer *layer_end)
{
	struct fann_neuron *neuron_it, *last_neuron, *prev_neurons, **connections;
	fann_type tmp_error;
	unsigned int i, num_connections;

	/* store some variabels local for fast access */
	struct fann_neuron *first_neuron = ann->first_layer->first_neuron;
	fann_type *error_begin = ann->training_params->train_errors;
	fann_type *slope_begin, *neuron_slope;

	/* if no room allocated for the slope variabels, allocate it now */
	if(ann->rprop_params->train_slopes == NULL)
	{
		ann->rprop_params->train_slopes =
			(fann_type *) calloc(ann->cascade_params->total_connections_allocated, sizeof(fann_type));
		if(ann->rprop_params->train_slopes == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}

	if(layer_begin == NULL)
	{
		layer_begin = ann->first_layer + 1;
	}

	if(layer_end == NULL)
	{
		layer_end = ann->last_layer - 1;
	}

	slope_begin = ann->rprop_params->train_slopes;

#ifdef DEBUGTRAIN
	printf("\nupdate slopes\n");
#endif

	prev_neurons = first_neuron;

	for(; layer_begin <= layer_end; layer_begin++)
	{
#ifdef DEBUGTRAIN
		printf("layer[%d]\n", layer_begin - ann->first_layer);
#endif
		last_neuron = layer_begin->last_neuron;
		if(ann->connection_rate >= 1)
		{
			if(ann->network_type == FANN_NETTYPE_LAYER)
			{
				prev_neurons = (layer_begin - 1)->first_neuron;
			}

			for(neuron_it = layer_begin->first_neuron; neuron_it != last_neuron; neuron_it++)
			{
				tmp_error = error_begin[neuron_it - first_neuron];
				neuron_slope = slope_begin + neuron_it->first_con;
				num_connections = neuron_it->last_con - neuron_it->first_con;
				for(i = 0; i != num_connections; i++)
				{
					neuron_slope[i] += tmp_error * prev_neurons[i].value;
				}
			}
		}
		else
		{
			for(neuron_it = layer_begin->first_neuron; neuron_it != last_neuron; neuron_it++)
			{
				tmp_error = error_begin[neuron_it - first_neuron];
				neuron_slope = slope_begin + neuron_it->first_con;
				num_connections = neuron_it->last_con - neuron_it->first_con;
				connections = ann->connections + neuron_it->first_con;
				for(i = 0; i != num_connections; i++)
				{
					neuron_slope[i] += tmp_error * connections[i]->value;
				}
			}
		}
	}
}

/* INTERNAL FUNCTION
   Clears arrays used for training before a new training session.
   Also creates the arrays that do not exist yet.
 */
void fann_clear_train_arrays(struct fann *ann)
{
	unsigned int i;
	fann_type delta_zero;

	/* if no room allocated for the slope variabels, allocate it now
	 * (calloc clears mem) */
	if(ann->rprop_params->train_slopes == NULL)
	{
		ann->rprop_params->train_slopes =
			(fann_type *) calloc(ann->cascade_params->total_connections_allocated, sizeof(fann_type));
		if(ann->rprop_params->train_slopes == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}
	else
	{
		memset(ann->rprop_params->train_slopes, 0, 
			(ann->cascade_params->total_connections_allocated) * sizeof(fann_type));
	}

	/* if no room allocated for the variabels, allocate it now */
	if(ann->rprop_params->prev_steps == NULL)
	{
		ann->rprop_params->prev_steps = (fann_type *) 
			malloc(ann->cascade_params->total_connections_allocated * sizeof(fann_type));
		if(ann->rprop_params->prev_steps == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}

	if(ann->training_params->training_algorithm == FANN_TRAIN_RPROP)
	{
		delta_zero = ann->rprop_params->rprop_delta_zero;
		
		for(i = 0; i < ann->cascade_params->total_connections_allocated; i++)
			ann->rprop_params->prev_steps[i] = delta_zero;
	}
	else
	{
		memset(ann->rprop_params->prev_steps, 0, 
			(ann->cascade_params->total_connections_allocated) * sizeof(fann_type));
	}

	/* if no room allocated for the variabels, allocate it now */
	if(ann->rprop_params->prev_train_slopes == NULL)
	{
		ann->rprop_params->prev_train_slopes =
			(fann_type *) calloc(ann->cascade_params->total_connections_allocated, sizeof(fann_type));
		if(ann->rprop_params->prev_train_slopes == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}
	else
	{
		memset(ann->rprop_params->prev_train_slopes, 0, 
			(ann->cascade_params->total_connections_allocated) * sizeof(fann_type));
	}
}

/* INTERNAL FUNCTION
   Update weights for batch training
 */
void fann_update_weights_batch(struct fann *ann, unsigned int num_data, unsigned int first_weight,
							   unsigned int past_end)
{
	fann_type *train_slopes = ann->rprop_params->train_slopes;
	fann_type *weights = ann->weights;
	const float epsilon = ann->backprop_params->learning_rate / num_data;
	unsigned int i = first_weight;

	for(; i != past_end; i++)
	{
		weights[i] += train_slopes[i] * epsilon;
		train_slopes[i] = 0.0;
	}
}

/* INTERNAL FUNCTION
   The quickprop training algorithm
 */
void fann_update_weights_quickprop(struct fann *ann, unsigned int num_data,
								   unsigned int first_weight, unsigned int past_end)
{
	fann_type *train_slopes = ann->rprop_params->train_slopes;
	fann_type *weights = ann->weights;
	fann_type *prev_steps = ann->rprop_params->prev_steps;
	fann_type *prev_train_slopes = ann->rprop_params->prev_train_slopes;

	fann_type w, prev_step, slope, prev_slope, next_step;

	float epsilon = ann->backprop_params->learning_rate / num_data;
	float decay = ann->rprop_params->quickprop_decay;	/*-0.0001;*/
	float mu = ann->rprop_params->quickprop_mu;	/*1.75; */
	float shrink_factor = (float) (mu / (1.0 + mu));
	
	unsigned int i = first_weight;

	for(; i != past_end; i++)
	{
		w = weights[i];
		prev_step = prev_steps[i];
		slope = train_slopes[i] + decay * w;
		prev_slope = prev_train_slopes[i];
		next_step = 0.0;
		
		/* The step must always be in direction opposite to the slope. */
		if(prev_step > 0.001)
		{
			/* If last step was positive...  */
			if(slope > 0.0) /*  Add in linear term if current slope is still positive. */
				next_step += epsilon * slope;

			/*If current slope is close to or larger than prev slope...  */
			if(slope > (shrink_factor * prev_slope))
				next_step += mu * prev_step;	/* Take maximum size negative step. */
			else
				next_step += prev_step * slope / (prev_slope - slope);	/* Else, use quadratic estimate. */
		}
		else if(prev_step < -0.001)
		{
			/* If last step was negative...  */
			if(slope < 0.0) /*  Add in linear term if current slope is still negative. */
				next_step += epsilon * slope;

			/* If current slope is close to or more neg than prev slope... */
			if(slope < (shrink_factor * prev_slope))
				next_step += mu * prev_step;	/* Take maximum size negative step. */
			else
				next_step += prev_step * slope / (prev_slope - slope);	/* Else, use quadratic estimate. */
		}
		else /* Last step was zero, so use only linear term. */
			next_step += epsilon * slope; 

		/*
		if(next_step > 1000 || next_step < -1000)
		{
			printf("quickprop[%d] weight=%f, slope=%f, prev_slope=%f, next_step=%f, prev_step=%f\n",
				   i, weights[i], slope, prev_slope, next_step, prev_step);
			
			   if(next_step > 1000)
			   next_step = 1000;
			   else
			   next_step = -1000;
		}
    	*/

		/* update global data arrays */
		prev_steps[i] = next_step;

		w += next_step;

		if(w > 1500)
			weights[i] = 1500;
		else if(w < -1500)
			weights[i] = -1500;
		else
			weights[i] = w;

		/*weights[i] = w;*/

		prev_train_slopes[i] = slope;
		train_slopes[i] = 0.0;
	}
}

/* INTERNAL FUNCTION
   The iRprop- algorithm
*/
void fann_update_weights_irpropm(struct fann *ann, unsigned int first_weight, unsigned int past_end)
{
	fann_type *train_slopes = ann->rprop_params->train_slopes;
	fann_type *weights = ann->weights;
	fann_type *prev_steps = ann->rprop_params->prev_steps;
	fann_type *prev_train_slopes = ann->rprop_params->prev_train_slopes;

	fann_type prev_step, slope, prev_slope, next_step, same_sign;

	float increase_factor = ann->rprop_params->rprop_increase_factor;	/*1.2; */
	float decrease_factor = ann->rprop_params->rprop_decrease_factor;	/*0.5; */
	float delta_min = ann->rprop_params->rprop_delta_min;	/*0.0; */
	float delta_max = ann->rprop_params->rprop_delta_max;	/*50.0; */

	unsigned int i = first_weight;

	for(; i != past_end; i++)
	{
		prev_step = fann_max(prev_steps[i], (fann_type) 0.0001);	/* prev_step may not be zero because then the training will stop */
		slope = train_slopes[i];
		prev_slope = prev_train_slopes[i];

		same_sign = prev_slope * slope;

		if(same_sign >= 0.0)
			next_step = fann_min(prev_step * increase_factor, delta_max);
		else
		{
			next_step = fann_max(prev_step * decrease_factor, delta_min);
			slope = 0;
		}

		if(slope < 0)
		{
			weights[i] -= next_step;
			if(weights[i] < -1500)
				weights[i] = -1500;
		}
		else
		{
			weights[i] += next_step;
			if(weights[i] > 1500)
				weights[i] = 1500;
		}

		/*if(i == 2){
		 * printf("weight=%f, slope=%f, next_step=%f, prev_step=%f\n", weights[i], slope, next_step, prev_step);
		 * } */

		/* update global data arrays */
		prev_steps[i] = next_step;
		prev_train_slopes[i] = slope;
		train_slopes[i] = 0.0;
	}
}

/* INTERNAL FUNCTION
   The SARprop- algorithm
*/
void fann_update_weights_sarprop(struct fann *ann, unsigned int epoch, unsigned int first_weight, unsigned int past_end)
{
	/* TODO: seed random number generator; this should currently be done in the enduser apps */
	fann_type *train_slopes = ann->rprop_params->train_slopes;
	fann_type *weights = ann->weights;
	fann_type *prev_steps = ann->rprop_params->prev_steps;
	fann_type *prev_train_slopes = ann->rprop_params->prev_train_slopes;

	fann_type prev_step, slope, prev_slope, next_step = 0, same_sign;

	/* These should be set from variables */
	float increase_factor = ann->rprop_params->rprop_increase_factor;	/*1.2; */
	float decrease_factor = ann->rprop_params->rprop_decrease_factor;	/*0.5; */
	/* TODO: why is delta_min 0.0 in iRprop? SARPROP uses 1x10^-6 (Braun and Riedmiller, 1993) */
	float delta_min = 0.000001f;
	float delta_max = ann->rprop_params->rprop_delta_max;	/*50.0; */
	float weight_decay_shift = ann->rprop_params->sarprop_weight_decay_shift; /* ld 0.01 = -6.644 */
	float step_error_threshold_factor = ann->rprop_params->sarprop_step_error_threshold_factor; /* 0.1 */
	float step_error_shift = ann->rprop_params->sarprop_step_error_shift; /* ld 3 = 1.585 */
	float T = ann->rprop_params->sarprop_temperature;
	float MSE = fann_get_MSE(ann);
	float RMSE = sqrt(MSE);

	unsigned int i = first_weight;


	/* for all weights; TODO: are biases included? */
	for(; i != past_end; i++)
	{
		/* TODO: confirm whether 1x10^-6 == delta_min is really better */
		prev_step = fann_max(prev_steps[i], (fann_type) 0.000001);	/* prev_step may not be zero because then the training will stop */
		/* calculate SARPROP slope; TODO: better as new error function? (see SARPROP paper)*/
		slope = -train_slopes[i] - weights[i] * fann_exp2(-T * epoch + weight_decay_shift);

		/* TODO: is prev_train_slopes[i] 0.0 in the beginning? */
		prev_slope = prev_train_slopes[i];

		same_sign = prev_slope * slope;

		if(same_sign > 0.0)
		{
			next_step = fann_min(prev_step * increase_factor, delta_max);
			/* TODO: are the signs inverted? see differences between SARPROP paper and iRprop */
			if (slope < 0.0)
				weights[i] += next_step;
			else
				weights[i] -= next_step;
		}
		else if(same_sign < 0.0)
		{
			if(prev_step < step_error_threshold_factor * MSE)
				next_step = prev_step * decrease_factor + (float)rand() / RAND_MAX * RMSE * fann_exp2(-T * epoch + step_error_shift);
			else
				next_step = fann_max(prev_step * decrease_factor, delta_min);

			slope = 0.0;
		}
		else
		{
			if(slope < 0.0)
				weights[i] += prev_step;
			else
				weights[i] -= prev_step;
		}


		/*if(i == 2){
		 * printf("weight=%f, slope=%f, next_step=%f, prev_step=%f\n", weights[i], slope, next_step, prev_step);
		 * } */

		/* update global data arrays */
		prev_steps[i] = next_step;
		prev_train_slopes[i] = slope;
		train_slopes[i] = 0.0;
	}
}
#endif
#else

/* FIXME: Empty body for fixedfann */
void fann_compute_MSE(struct fann *ann, fann_type *desired_output)
{
}

#endif

FANN_EXTERNAL void FANN_API fann_set_activation_function_hidden(struct fann *ann,
																enum fann_activationfunc_enum activation_function)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *layer_it;
	struct fann_layer *last_layer = ann->last_layer - 1;	/* -1 to not update the output layer */

	for(layer_it = ann->first_layer; layer_it != last_layer; layer_it++)
	{
		last_neuron = layer_it->last_neuron;
		for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
		{
			neuron_it->activation_function = activation_function;
		}
	}
}

FANN_EXTERNAL struct fann_layer* FANN_API fann_get_layer(struct fann *ann, int layer)
{
	if(layer <= 0 || layer >= (ann->last_layer - ann->first_layer))
	{
		fann_error((struct fann_error *) ann, FANN_E_INDEX_OUT_OF_BOUND, layer);
		return NULL;
	}
	
	return ann->first_layer + layer;	
}

FANN_EXTERNAL struct fann_neuron* FANN_API fann_get_neuron_layer(struct fann *ann, struct fann_layer* layer, int neuron)
{
	if(neuron >= (layer->last_neuron - layer->first_neuron))
	{
		fann_error((struct fann_error *) ann, FANN_E_INDEX_OUT_OF_BOUND, neuron);
		return NULL;	
	}
	
	return layer->first_neuron + neuron;
}

FANN_EXTERNAL struct fann_neuron* FANN_API fann_get_neuron(struct fann *ann, unsigned int layer, int neuron)
{
	struct fann_layer *layer_it = fann_get_layer(ann, layer);
	if(layer_it == NULL)
		return NULL;
	return fann_get_neuron_layer(ann, layer_it, neuron);
}

FANN_EXTERNAL enum fann_activationfunc_enum FANN_API
    fann_get_activation_function(struct fann *ann, int layer, int neuron)
{
	struct fann_neuron* neuron_it = fann_get_neuron(ann, layer, neuron);
	if (neuron_it == NULL)
    {
		return (enum fann_activationfunc_enum)-1; /* layer or neuron out of bounds */
    }
    else
    {
	    return neuron_it->activation_function;
    }
}

FANN_EXTERNAL void FANN_API fann_set_activation_function(struct fann *ann,
																enum fann_activationfunc_enum
																activation_function,
																int layer,
																int neuron)
{
	struct fann_neuron* neuron_it = fann_get_neuron(ann, layer, neuron);
	if(neuron_it == NULL)
		return;

	neuron_it->activation_function = activation_function;
}

FANN_EXTERNAL void FANN_API fann_set_activation_function_layer(struct fann *ann,
																enum fann_activationfunc_enum
																activation_function,
																int layer)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *layer_it = fann_get_layer(ann, layer);
	
	if(layer_it == NULL)
		return;

	last_neuron = layer_it->last_neuron;
	for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
	{
		neuron_it->activation_function = activation_function;
	}
}


FANN_EXTERNAL void FANN_API fann_set_activation_function_output(struct fann *ann,
																enum fann_activationfunc_enum activation_function)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *last_layer = ann->last_layer - 1;

	last_neuron = last_layer->last_neuron;
	for(neuron_it = last_layer->first_neuron; neuron_it != last_neuron; neuron_it++)
	{
		neuron_it->activation_function = activation_function;
	}
}

FANN_EXTERNAL void FANN_API fann_set_activation_steepness_hidden(struct fann *ann,
																 fann_type steepness)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *layer_it;
	struct fann_layer *last_layer = ann->last_layer - 1;	/* -1 to not update the output layer */

	for(layer_it = ann->first_layer ; layer_it != last_layer; layer_it++)
	{
		last_neuron = layer_it->last_neuron;
		for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
		{
			neuron_it->activation_steepness = steepness;
		}
	}
}

FANN_EXTERNAL fann_type FANN_API
    fann_get_activation_steepness(struct fann *ann, int layer, int neuron)
{
	struct fann_neuron* neuron_it = fann_get_neuron(ann, layer, neuron);
	if(neuron_it == NULL)
    {
		return -1; /* layer or neuron out of bounds */
    }
    else
    {
        return neuron_it->activation_steepness;
    }
}

FANN_EXTERNAL void FANN_API fann_set_activation_steepness(struct fann *ann,
																fann_type steepness,
																int layer,
																int neuron)
{
	struct fann_neuron* neuron_it = fann_get_neuron(ann, layer, neuron);
	if(neuron_it == NULL)
		return;

	neuron_it->activation_steepness = steepness;
}

FANN_EXTERNAL void FANN_API fann_set_activation_steepness_layer(struct fann *ann,
																fann_type steepness,
																int layer)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *layer_it = fann_get_layer(ann, layer);
	
	if(layer_it == NULL)
		return;

	last_neuron = layer_it->last_neuron;
	for(neuron_it = layer_it->first_neuron; neuron_it != last_neuron; neuron_it++)
	{
		neuron_it->activation_steepness = steepness;
	}
}

FANN_EXTERNAL void FANN_API fann_set_activation_steepness_output(struct fann *ann,
																 fann_type steepness)
{
	struct fann_neuron *last_neuron, *neuron_it;
	struct fann_layer *last_layer = ann->last_layer - 1;

	last_neuron = last_layer->last_neuron;
	for(neuron_it = last_layer->first_neuron; neuron_it != last_neuron; neuron_it++)
	{
		neuron_it->activation_steepness = steepness;
	}
}


FANN_GET_SETP(enum fann_errorfunc_enum, training_params, train_error_function)
FANN_GET_SETP(float, rprop_params, quickprop_decay)
FANN_GET_SETP(float, rprop_params, quickprop_mu)
FANN_GET_SETP(float, rprop_params, rprop_increase_factor)
FANN_GET_SETP(float, rprop_params, rprop_decrease_factor)
FANN_GET_SETP(float, rprop_params, rprop_delta_min)
FANN_GET_SETP(float, rprop_params, rprop_delta_max)
FANN_GET_SETP(float, rprop_params, rprop_delta_zero)
FANN_GET_SETP(float, rprop_params, sarprop_weight_decay_shift)
FANN_GET_SETP(float, rprop_params, sarprop_step_error_threshold_factor)
FANN_GET_SETP(float, rprop_params, sarprop_step_error_shift)
FANN_GET_SETP(float, rprop_params, sarprop_temperature)
FANN_GET_SETP(enum fann_stopfunc_enum, training_params, train_stop_function)
FANN_GET_SETP(fann_type, training_params, bit_fail_limit)
FANN_GET_SETP(float, backprop_params, learning_momentum)
FANN_GET_SETP(float, backprop_params, learning_rate)
/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap
 */

