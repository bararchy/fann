#include "fann.h"

#ifndef EXCLUDE_SSE

#include "optimized/sse/fann.h"
#include "optimized/fann_train_optimized_template.h"

#define implementation sse

#define activation_function_name sigmoid
#include "fann_sse_mimo_batch.c"
#include "fann_sse_mimo_rprop.c"
#undef activation_function_name

#define activation_function_name sigmoid_symmetric
#include "fann_sse_mimo_batch.c"
#include "fann_sse_mimo_rprop.c"
#undef activation_function_name

#undef implementation

#endif
