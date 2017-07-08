#include "fann.h"

#ifndef EXCLUDE_BLAS

#include "optimized/blas/fann.h"
#include "optimized/fann_train_optimized_template.h"

#define implementation blas

#define activation_function_name sigmoid
#include "fann_blas_mimo_batch.c"
#include "fann_blas_mimo_rprop.c"
#undef activation_function_name

#define activation_function_name sigmoid_symmetric
#include "fann_blas_mimo_batch.c"
#include "fann_blas_mimo_rprop.c"
#undef activation_function_name

#undef implementation

#endif
