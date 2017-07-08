#ifndef _fann_optimized_blas_batch_h_
#define _fann_optimized_blas_batch_h_

/* Struct: MAKE_NAME(neuron_private_data)
   This structure just stores the arrays needed to use the algorithm.
 */
#define private_data_struct() \
struct MAKE_NAME(neuron_private_data)\
{\
	struct fann_backprop_params * backprop_params;\
};

#define algorithm batch

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
