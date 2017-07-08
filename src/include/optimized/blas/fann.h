#ifndef _fann_optimized_blas_h_
#define _fann_optimized_blas_h_

static char const *const FANN_OPTIMIZED_BLAS_IMPLEMENTED_ALGORITHM_NAMES[] = {
	"BATCH SIGMOID",
	"BATCH SIGMOID_SYMMETRIC",
	"RPROP SIGMOID",
	"RPROP SIGMOID_SYMMETRIC"
};

#define implementation blas

#include "fann_batch.h"

#include "fann_rprop.h"

#undef implementation

#endif

/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap
 */
