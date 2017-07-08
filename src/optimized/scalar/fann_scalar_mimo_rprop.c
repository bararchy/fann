#define algorithm rprop

#include "fann.c"
#include "../../fann_base.c"

FANN_EXTERNAL void FANN_API MAKE_NAME(layer_destructor)(struct fann_layer* layer)
{
	fann_base_layer_destructor(layer);
}

FANN_EXTERNAL int FANN_API MAKE_NAME(layer_constructor)(struct fann *ann,
		struct fann_layer *layer, struct fann_layer_descr *descr)
{
	if (fann_base_layer_constructor(ann, layer, descr))
		return 1;
	/* Set the other function pointers */
	layer->destructor = MAKE_NAME(layer_destructor);
	layer->initialize_train_errors = MAKE_NAME(layer_train_initialize);
	layer->run = MAKE_NAME(layer_run);
	return 0;
}

FANN_EXTERNAL void FANN_API MAKE_NAME(layer_run)(struct fann *ann, struct fann_layer* layer)
{
	fann_base_layer_run(ann,layer);
}

FANN_EXTERNAL void FANN_API MAKE_NAME(layer_train_initialize)(struct fann *ann, struct fann_layer *layer)
{
	fann_base_layer_train_initialize(ann,layer);
}

FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_destructor) (struct fann_neuron* neuron)
{
	struct MAKE_NAME(neuron_private_data) *priv = (struct MAKE_NAME(neuron_private_data) *) neuron->private_data;
	fann_base_neuron_destructor(neuron);
	fann_safe_free(priv->prev_weights_deltas);
	fann_safe_free(priv->prev_steps);
	fann_safe_free(priv->rprop_params);
	fann_safe_free(neuron->private_data);
}

FANN_EXTERNAL int FANN_API MAKE_NAME(neuron_constructor)(struct fann *ann, struct fann_layer *layer, 
		struct fann_neuron *neuron, struct fann_neuron_descr * descr)
{
	struct MAKE_NAME(neuron_private_data) *priv;

	if (fann_base_neuron_constructor(ann, layer, neuron, descr))
		return 1;

	/*allocate neuron private data*/
	if ( (neuron->private_data = priv =
				(struct MAKE_NAME(neuron_private_data) *) malloc (sizeof(struct MAKE_NAME(neuron_private_data)))) == NULL)
		return 1;

	priv = (struct MAKE_NAME(neuron_private_data) *) neuron->private_data;
	priv->prev_steps=NULL;
	priv->prev_weights_deltas=NULL;
	if ( ( priv->rprop_params = (struct fann_rprop_params*) malloc(sizeof(struct fann_rprop_params)) ) ==NULL)
		return 1;
	priv->rprop_params->rprop_increase_factor = 1.2f;
	priv->rprop_params->rprop_decrease_factor = 0.5f;
	priv->rprop_params->rprop_delta_min = 0.0f;
	priv->rprop_params->rprop_delta_max = 50.0f;
	priv->rprop_params->rprop_delta_zero = 0.1f;
	
	/* set the function pointers */
	neuron->destructor = MAKE_NAME(neuron_destructor);
	neuron->run = MAKE_NAME(neuron_run);
	neuron->backpropagate = MAKE_NAME(neuron_backprop);
	neuron->update_weights = MAKE_NAME(neuron_update);
	neuron->compute_error = MAKE_NAME(neuron_compute_MSE);
	neuron->train_initialize = MAKE_NAME(neuron_train_initialize);
	neuron->save = fann_neuron_save_connected_any_any;
	neuron->load = fann_neuron_load_connected_any_any;
	neuron->type = strdup(MAKE_TYPE());

	return 0;
}

FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_train_initialize)(struct fann *ann, struct fann_layer *layer, struct fann_neuron *neuron)
{
	unsigned int i;
	struct MAKE_NAME(neuron_private_data) *priv= (struct MAKE_NAME(neuron_private_data)*) neuron->private_data;
	fann_base_neuron_train_initialize(ann, layer, neuron);

	/* if no room allocated for the variabels, allocate it now */
	if(priv->prev_steps == NULL)
	{
		priv->prev_steps = (fann_type *) malloc(neuron->num_weights * sizeof(fann_type));
		if(priv->prev_steps == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}

	for(i = 0; i < neuron->num_weights; i++)
		priv->prev_steps[i] = priv->rprop_params->rprop_delta_zero;

	/* if no room allocated for the variabels, allocate it now */
	if(priv->prev_weights_deltas == NULL)
	{
		priv->prev_weights_deltas =
			(fann_type *) calloc(neuron->num_weights, sizeof(fann_type));
		if(priv->prev_weights_deltas == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}
	else
	{
		memset(priv->prev_weights_deltas, 0, (neuron->num_weights) * sizeof(fann_type));
	}
}

FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_run)(struct fann * ann, struct fann_neuron * neuron)
{
	fann_base_neuron_run(ann,neuron);
}

/*
 * Compute the error at the network output
 * (usually, after forward propagation of a certain input vector, neuron->run).
 * The error is a sum of squares for all the output units
 * also increments a counter because MSE is an average of such errors
 *
 * After this train_errors in the output layer will be set to:
 * (desired_output - neuron_value)
 */
FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_compute_MSE)(struct fann *ann, struct fann_neuron *neuron, fann_type *desired_output)
{
	fann_base_neuron_compute_MSE(ann, neuron, desired_output);
}

/*
 * Compute (neuron_value_derived * train_errors), then
 * Backpropagate the error in the previous layer, then
 * Adjust the weights.
 *
 * This function expect to find the errors array not empty 
 * (especially in the first call).
 */
FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_backprop)(struct fann *ann, struct fann_neuron *neuron, fann_type *prev_layer_errors)
{
	fann_base_neuron_backprop(ann, neuron, prev_layer_errors);
}

FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_update)(struct fann *ann, struct fann_neuron *neuron)
{
	struct MAKE_NAME(neuron_private_data) *priv = (struct MAKE_NAME(neuron_private_data) *) neuron->private_data;

	fann_type *weights = neuron->weights;
	fann_type *weights_deltas = neuron->weights_deltas;
	fann_type *prev_weights_deltas = priv->prev_weights_deltas;
	fann_type *prev_steps = priv->prev_steps;

	const unsigned int num_outputs = neuron->num_outputs;
	const unsigned int num_inputs = neuron->num_inputs;
	float increase_factor = priv->rprop_params->rprop_increase_factor;	/*1.2; */
	float decrease_factor = priv->rprop_params->rprop_decrease_factor;	/*0.5; */
	float delta_min = priv->rprop_params->rprop_delta_min;	/*0.0; */
	float delta_max = priv->rprop_params->rprop_delta_max;	/*50.0; */

	unsigned int o, i;
	fann_type prev_step, delta, prev_delta, next_step, same_sign;
	
	if (neuron->num_backprop_done==0)
	{
		fann_error(NULL, FANN_E_CANT_USE_TRAIN_ALG);
		return;
	}

	for (o = 0; o < num_outputs; o++)
	{
		for (i = 0; i < num_inputs; i++)
		{
			prev_step = fann_max(prev_steps[i], (fann_type) 0.0001);	/* prev_step may not be zero because then the training will stop */
			/* does 0.0001 make sense????*/
			delta = weights_deltas[i];
			prev_delta = prev_weights_deltas[i];

			same_sign = prev_delta * delta;

			if(same_sign >= 0.0)
				next_step = fann_min(prev_step * increase_factor, delta_max);
			else
			{
				next_step = fann_max(prev_step * decrease_factor, delta_min);
				delta = 0;
			}

			if(delta < 0)
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

			/* update data arrays */
			prev_steps[i] = next_step;
			prev_weights_deltas[i] = delta;
			weights_deltas[i] = 0.0;
		}
		weights += num_inputs;
		weights_deltas += num_inputs;
		prev_weights_deltas += num_inputs;
		prev_steps += num_inputs;
	}
	neuron->num_backprop_done=0;
}
#undef algorithm
/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap noet
 */
