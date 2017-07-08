#include "fann.h"
#include "optimized/scalar/fann.h"
#include "optimized/fann_train_optimized_template.h"

#define implementation scalar

#define activation_function_name sigmoid
#include "fann_scalar_mimo_batch.c"
#include "fann_scalar_mimo_rprop.c"
#undef activation_function_name 

#define activation_function_name sigmoid_symmetric
#include "fann_scalar_mimo_batch.c"
#include "fann_scalar_mimo_rprop.c"
#undef activation_function_name 

#undef implementation
