#define algorithm batch

#include "fann.c"
#include "fann_base.c"
FANN_EXTERNAL int FANN_API MAKE_NAME(layer_constructor)(struct fann *ann, 
		struct fann_layer *layer, struct fann_layer_descr *descr)
{
	if (fann_base_layer_constructor(ann, layer, descr))
		return 1;
	/* Set the other function pointers */
	layer->destructor = fann_layer_destructor_connected_any_any;
	layer->initialize_train_errors = fann_layer_train_initialize_connected_any_any;
	layer->run = fann_layer_run_connected_any_any;
	layer->save = fann_layer_save_connected_any_any;
	layer->load = fann_layer_load_connected_any_any;
	layer->type = strdup(MAKE_TYPE());

	return 0;
}


#if 0
FANN_EXTERNAL void FANN_API MAKE_NAME(layer_destructor)(struct fann_layer* layer)
{
	fann_base_layer_destructor(layer);
}


FANN_EXTERNAL void FANN_API MAKE_NAME(layer_run)(struct fann *ann, struct fann_layer* layer)
{
	fann_base_layer_run(ann,layer);
}

FANN_EXTERNAL void FANN_API MAKE_NAME(layer_train_initialize)(struct fann *ann, struct fann_layer *layer)
{
	fann_base_layer_train_initialize(ann,layer);
}
#endif

FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_destructor) (struct fann_neuron* neuron)
{
	struct MAKE_NAME(neuron_private_data) *priv = (struct MAKE_NAME(neuron_private_data) *) neuron->private_data;
	fann_base_neuron_destructor(neuron);
	fann_safe_free(priv->backprop_params);
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
	priv->backprop_params= (struct fann_backprop_params*) malloc(sizeof(struct fann_backprop_params));
  priv->backprop_params->learning_rate=0.7;
	priv->backprop_params->learning_rate=0;
	priv->num_inputs_padded=(neuron->num_inputs+3)>>2<<2;
	
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
	fann_base_neuron_train_initialize(ann, layer, neuron);
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
	unsigned int o, j;
	fann_type *weights, *deltas;
	const unsigned int num_outputs = neuron->num_outputs;
	const unsigned int num_inputs = neuron->num_inputs;
	const unsigned int num_inputs_padded = priv->num_inputs_padded;
	float learning_rate = ann->backprop_params->learning_rate;
#ifdef FIXEDFANN 
	unsigned int decimal_point = ann->fixed_params->decimal_point;
#endif

	if (neuron->num_backprop_done==0)
	{
		fann_error(NULL, FANN_E_CANT_USE_TRAIN_ALG);
		return;
	}

	learning_rate=learning_rate/neuron->num_backprop_done;

	/* some assignments to speed up things */
	weights = neuron->weights;
	deltas = neuron->weights_deltas;

	for (o = 0; o < num_outputs; o++)
	{
		for (j = 0; j < num_inputs; j++)
		{
			/* adjust the weight */
			weights[j] += deltas[j] * learning_rate; /* FIXME add the learning momentum here */
			deltas[j]=0;
		}
		weights += num_inputs_padded;
		deltas += num_inputs_padded;
	}
	neuron->num_backprop_done=0;
}
#undef algorithm

/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap noet
 */
