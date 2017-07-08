#define MAKE_NAME(name) F_CONCAT4(name, implementation, algorithm, activation_function_name)
#define F_CONCAT4(name,impl,algo,activation) __F_CONCAT4(name,impl,algo,activation)
#define __F_CONCAT4(n1,n2,n3,n4) fann_ ## n1 ## _ ## n2 ## _ ## n3 ## _ ## n4

#define MAKE_NAME_ACTIVATION(name) F_CONCAT2(activation, name)
#define activation_macro() F_CONCAT2(activation, activation_function_name)
#define activation_derived_macro() F_CONCAT2(activation_derived, activation_function_name)
#define update_MSE_macro() F_CONCAT2 (update_MSE, activation_function_name)
#define F_CONCAT2(n1,n2) __F_CONCAT2(n1,n2)
#define __F_CONCAT2(n1,n2) fann_ ## n1 ## _ ## n2 

#define MAKE_TYPE() F_CONCAT3(implementation, algorithm, activation_function_name)
#define F_CONCAT3(impl,algo,activation) __F_CONCAT3(impl,algo,activation)
#define __F_CONCAT3(n1,n2,n3) str(n1 ## _ ## n2 ## _ ## n3)

#define vals(v) str(v)

#ifdef __cplusplus
extern "C"
{
	
#ifndef __cplusplus
} /* to fool automatic indention engines */ 
#endif
#endif	/* __cplusplus */
FANN_EXTERNAL struct fann *FANN_API MAKE_NAME(create_standard)(unsigned int num_layers, ...);
FANN_EXTERNAL struct fann *FANN_API MAKE_NAME(create_standard_array)(unsigned int num_layers, 
															   const unsigned int *layers);

/* File: fann_optimized_template.h
   The fann optimized_is a MIMO neuron implementation.
 */

/* Function: MAKE_NAME(layer_constructor)
   Allocates room inside the layer for neurons and connections. 
	 Adds one bias output set to 1.
 */
FANN_EXTERNAL int FANN_API MAKE_NAME(layer_constructor)(struct fann *ann, 
		struct fann_layer *layer, struct fann_layer_descr *descr);

/*Function: MAKE_NAME(layer_run)
  Iterates over all neurons and calls the respective run functions.
	The implementation is responsible for setting up the input array.
  If data is not already there this function must copy it into the 
	inpuit array before calling neuron_it->run().
 */
FANN_EXTERNAL void FANN_API MAKE_NAME(layer_run)(struct fann *ann, struct fann_layer* layer);

/* Function: MAKE_NAME(neuron_constructor)
   Allocates room inside the neuron for the connections.
   Creates a fully connected neuron.
 */
FANN_EXTERNAL int FANN_API MAKE_NAME(neuron_constructor)(struct fann *ann,
	 	struct fann_layer *layer, struct fann_neuron *neuron, struct fann_neuron_descr * descr);

/* Function: MAKE_NAME(neuron_run)
   Does a forward iteration on the neuron. 
 */
FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_run)(struct fann *ann, 
		struct fann_neuron *neuron);

/* Function: MAKE_NAME(neuron_train_initialize)
   Allocates room inside the neuron for training data structures.
	 This function is called just when it is necessary to allocate the space
	 for training. If a network is not going to be trainied (when using an already
	 trainied network) all this stuff is not allocated.
 */
FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_train_initialize)(struct fann *ann, struct fann_layer *layer, struct fann_neuron *neuron);

/* Function: MAKE_NAME(compute_MSE)
   Compute the error on a MIMO Neuron after forward propagation of 
    a certain input vector i.e. after neuron->run().
    The error is the sum, over all the outputs, of the squared difference
    between the computed output and the desired target
    also increments a counter because MSE is an average of such errors
   
    After calling this function on a neuron the train_errors array is set to:
    (desired_output - neuron_value)
 */
FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_compute_MSE)(struct fann *ann, 
		struct fann_neuron *neuron, fann_type *desired_output);

/* 
   Function: MAKE_NAME(neuron_backprop)
   Train the MIMO neuron: this function backpropagates the error to the 
	 previous layer and computes the weight update. The weight update is not 
	 applied here see <MAKE_NAME(neuron_update>)
 */
FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_backprop)(struct fann *ann,
	 	struct fann_neuron *neuron, fann_type *prev_layer_errors);

/* 
   Function: MAKE_NAME(neuron_update)
   Apply the training on the MIMO neuron: the weight update stored in the neuron
	 is applied. This function is called once per input pattern when online training
	 is carried on and once per epoch when batch algoritms are selected
 */
FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_update)(struct fann *ann,
	 	struct fann_neuron *neuron);

/* Function: MAKE_NAME(layer_train_initialize)
   Allocates room inside the layer for training data structures.
	 This function is called just when it is necessary to allocate the space
	 for training. If a network is not going to be trainied (when using an already
	 trainied network) all this stuff is not allocated.
 */
FANN_EXTERNAL void FANN_API MAKE_NAME(layer_train_initialize)(struct fann *ann, struct fann_layer *layer);

FANN_EXTERNAL int FANN_API MAKE_NAME(neuron_load) (struct fann *ann, struct fann_layer *layer, struct fann_neuron *neuron, FILE *conf);
FANN_EXTERNAL void FANN_API MAKE_NAME(neuron_save) (struct fann *ann, struct fann_layer *layer, struct fann_neuron *neuron, FILE *conf);
FANN_EXTERNAL int FANN_API MAKE_NAME(layer_load) (struct fann *ann, struct fann_layer *layer, FILE *conf);
FANN_EXTERNAL void FANN_API MAKE_NAME(layer_save) (struct fann *ann, struct fann_layer *layer, FILE *conf);
#ifdef __cplusplus
#ifndef __cplusplus
{ /* to fool automatic indention engines */ 
#endif
}
	
#endif	/* __cplusplus */
/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap
 */
