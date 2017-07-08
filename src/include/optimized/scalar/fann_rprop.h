#ifndef _fann_optimized_scalar_rprop_h_
#define _fann_optimized_scalar_rprop_h_

/* Struct: MAKE_NAME(neuron_private_data)
   This structure just stores the arrays needed to use the algorithm.
 */
#define private_data_struct() \
struct MAKE_NAME(neuron_private_data)\
{\
	fann_type * prev_steps;\
	fann_type * prev_weights_deltas;\
	struct fann_rprop_params * rprop_params;\
};

#define algorithm rprop

#define activation_function_name sigmoid
#include "../fann_optimized_template.h"
private_data_struct()
#undef activation_function_name 

#define activation_function_name sigmoid_symmetric
#include "../fann_optimized_template.h"
private_data_struct()
#undef activation_function_name

#undef algorithm

#undef private_data_struct
#endif
