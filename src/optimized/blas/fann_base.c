#ifndef __fann_blas_base_c
#define __fann_blas_base_c

#include "fann.h"
#include <cblas.h>
#include <sys/time.h>
#include <string.h>
#define EXCLUDE_BASE_RUN 1
#include "../../fann_base.c"
#undef EXCLUDE_BASE_RUN

#undef fann_base_neuron_run
#ifdef FIXEDFANN 
#define fann_base_neuron_run(ann, neuron)\
/*static __inline  void  fann_base_neuron_run(struct fann * ann, struct fann_neuron * neuron)*/ \
{\
	unsigned int i, o, num_connections, num_inputs, num_outputs;\
	fann_type *neuron_sums, *outputs, *inputs, *weights;\
	unsigned int activation_function;\
	fann_type steepness;\
\
	int multiplier = ann->fixed_params->multiplier;\
	unsigned int decimal_point = ann->fixed_params->decimal_point;\
\
	/* values used for the stepwise linear sigmoid function */\
	fann_type r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0;\
	fann_type v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0, v6 = 0;\
\
	fann_type last_steepness = 0;\
	unsigned int last_activation_function = 0;\
\
	/* Algorith for fully connected networks */\
	activation_function = neuron->activation_function;\
	steepness = neuron->activation_steepness;\
\
	num_inputs = neuron->num_inputs;\
	inputs = neuron->inputs;\
	num_outputs = neuron->num_outputs;\
	outputs = neuron->outputs;\
	num_connections = neuron->num_inputs;\
	weights = neuron->weights;\
	neuron_sums=neuron->sums;\
\
	for (o=0; o<num_outputs ; o++)\
	{\
		neuron_sums[o]=0;\
		/* unrolled loop start */\
		i = num_connections & 3;	/* same as modulo 4 */\
		switch (i)\
		{\
			case 3:\
				neuron_sums[o] += fann_mult(weights[2], inputs[2]);\
			case 2:\
				neuron_sums[o] += fann_mult(weights[1], inputs[1]);\
			case 1:\
				neuron_sums[o] += fann_mult(weights[0], inputs[0]);\
			case 0:\
				break;\
		}\
\
		for(; i != num_connections; i += 4)\
		{\
			neuron_sums[o] +=\
				fann_mult(weights[i]    , inputs[i]    ) +\
				fann_mult(weights[i + 1], inputs[i + 1]) +\
				fann_mult(weights[i + 2], inputs[i + 2]) +\
				fann_mult(weights[i + 3], inputs[i + 3]);\
		}\
		weights += num_connections;\
		/* unrolled loop end */\
\
		neuron->sums[o] = fann_mult(steepness, neuron_sums[o]);\
\
		if(activation_function != last_activation_function || steepness != last_steepness)\
		{\
			switch (activation_function)\
			{\
				case FANN_SIGMOID:\
				case FANN_SIGMOID_STEPWISE:\
					r1 = ann->fixed_params->sigmoid_results[0];\
					r2 = ann->fixed_params->sigmoid_results[1];\
					r3 = ann->fixed_params->sigmoid_results[2];\
					r4 = ann->fixed_params->sigmoid_results[3];\
					r5 = ann->fixed_params->sigmoid_results[4];\
					r6 = ann->fixed_params->sigmoid_results[5];\
					v1 = ann->fixed_params->sigmoid_values[0] / steepness;\
					v2 = ann->fixed_params->sigmoid_values[1] / steepness;\
					v3 = ann->fixed_params->sigmoid_values[2] / steepness;\
					v4 = ann->fixed_params->sigmoid_values[3] / steepness;\
					v5 = ann->fixed_params->sigmoid_values[4] / steepness;\
					v6 = ann->fixed_params->sigmoid_values[5] / steepness;\
					break;\
				case FANN_SIGMOID_SYMMETRIC:\
				case FANN_SIGMOID_SYMMETRIC_STEPWISE:\
					r1 = ann->fixed_params->sigmoid_symmetric_results[0];\
					r2 = ann->fixed_params->sigmoid_symmetric_results[1];\
					r3 = ann->fixed_params->sigmoid_symmetric_results[2];\
					r4 = ann->fixed_params->sigmoid_symmetric_results[3];\
					r5 = ann->fixed_params->sigmoid_symmetric_results[4];\
					r6 = ann->fixed_params->sigmoid_symmetric_results[5];\
					v1 = ann->fixed_params->sigmoid_symmetric_values[0] / steepness;\
					v2 = ann->fixed_params->sigmoid_symmetric_values[1] / steepness;\
					v3 = ann->fixed_params->sigmoid_symmetric_values[2] / steepness;\
					v4 = ann->fixed_params->sigmoid_symmetric_values[3] / steepness;\
					v5 = ann->fixed_params->sigmoid_symmetric_values[4] / steepness;\
					v6 = ann->fixed_params->sigmoid_symmetric_values[5] / steepness;\
					break;\
				case FANN_THRESHOLD:\
					break;\
			}\
		}\
\
		switch (activation_function)\
		{\
			case FANN_SIGMOID:\
			case FANN_SIGMOID_STEPWISE:\
				neuron->outputs[o] =\
					(fann_type) fann_stepwise(v1, v2, v3, v4, v5, v6, r1, r2, r3, r4, r5, r6, 0,\
							multiplier, neuron_sums[o]);\
				break;\
			case FANN_SIGMOID_SYMMETRIC:\
			case FANN_SIGMOID_SYMMETRIC_STEPWISE:\
				neuron->outputs[o] =\
					(fann_type) fann_stepwise(v1, v2, v3, v4, v5, v6, r1, r2, r3, r4, r5, r6,\
							-multiplier, multiplier, neuron_sums[o]);\
				break;\
			case FANN_THRESHOLD:\
				neuron->outputs[o] = (fann_type) ((neuron_sums[o] < 0) ? 0 : multiplier);\
				break;\
			case FANN_THRESHOLD_SYMMETRIC:\
				neuron->outputs[o] = (fann_type) ((neuron_sums[o] < 0) ? -multiplier : multiplier);\
				break;\
			case FANN_LINEAR:\
				neuron->outputs[o] = neuron_sums[o];\
				break;\
			case FANN_LINEAR_PIECE:\
				neuron->outputs[o] = (fann_type)((neuron_sums[o] < 0) ? 0 : (neuron_sums[o] > multiplier) ? multiplier : neuron_sums[o]);\
				break;\
			case FANN_LINEAR_PIECE_SYMMETRIC:\
				neuron->outputs[o] = (fann_type)((neuron_sums[o] < -multiplier) ? -multiplier : (neuron_sums[o] > multiplier) ? multiplier : neuron_sums[o]);\
				break;\
			case FANN_ELLIOT:\
			case FANN_ELLIOT_SYMMETRIC:\
			case FANN_GAUSSIAN:\
			case FANN_GAUSSIAN_SYMMETRIC:\
			case FANN_GAUSSIAN_STEPWISE:\
			case FANN_SIN_SYMMETRIC:\
			case FANN_COS_SYMMETRIC:\
				fann_error((struct fann_error *) ann, FANN_E_CANT_USE_ACTIVATION);\
				break;\
		}\
		last_steepness = steepness;\
		last_activation_function = activation_function;\
	}\
}

#else /*FIXEDFANN*/

#define fann_base_neuron_run(ann, neuron)\
/*static __inline  void  fann_base_neuron_run(struct fann * ann, struct fann_neuron * neuron)*/ \
{\
	unsigned int o, num_connections, num_inputs, num_outputs;\
	fann_type *neuron_sums, *outputs, *inputs, *weights;\
	unsigned int activation_function;\
	fann_type steepness;\
\
	fann_type max_sum = 0;	\
\
	/* Algorith for fully connected networks */\
	activation_function = neuron->activation_function;\
	steepness = neuron->activation_steepness;\
\
	num_inputs = neuron->num_inputs;\
	inputs = neuron->inputs;\
	num_outputs = neuron->num_outputs;\
	outputs = neuron->outputs;\
	num_connections = neuron->num_inputs;\
	weights = neuron->weights;\
	neuron_sums=neuron->sums;\
	\
	memset(neuron_sums,0,num_outputs*sizeof(fann_type));\
	cblas_sgemv( CblasRowMajor, CblasNoTrans,\
			num_outputs, num_connections, 1.0f, weights, num_connections,\
			inputs, 1, 1.0f, \
			neuron_sums, 1 );\
\
	for (o=0; o<num_outputs ; o++)\
	{\
		neuron_sums[o] = fann_mult(steepness, neuron_sums[o]);\
		\
		max_sum = 150/steepness;\
		if(neuron_sums[o] > max_sum)\
			neuron_sums[o] = max_sum;\
		else if(neuron_sums[o] < -max_sum)\
			neuron_sums[o] = -max_sum;\
		\
		activation_macro()(neuron, o);\
	}\
}

#endif /*FIXEDFANN*/

#endif /*__fann_blas_base_c*/
/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap noet
 */
