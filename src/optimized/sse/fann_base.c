#ifndef __fann_sse_base_c
#define __fann_sse_base_c

#include "fann.h"
#include <xmmintrin.h>
#include <sys/time.h>
#include <string.h>
#define EXCLUDE_BASE_RUN 1
#include "../../fann_base.c"
#undef EXCLUDE_BASE_RUN

void * _mm_calloc(size_t size, size_t alignment)
{
	void *ret= _mm_malloc(size,alignment);
	memset(ret,0,size);
	return ret;
}

#define ADDER(m0,m1)\
	xmm ## m0=_mm_load_ps(A_row); A_row+=4; \
	xmm ## m1=_mm_load_ps(Bp); Bp+=4; \
	xmm ## m1 = _mm_mul_ps(xmm ## m1, xmm ## m0);\
	xmm7 = _mm_add_ps(xmm7, xmm ## m1);

static __inline float sum_v4sp(__m128 v4sp)
{
	float *v = (float* )&v4sp;
	return v[0]+v[1]+v[2]+v[3];
}


static __inline__ void MatVectMult(float *A, float *B, float *S, int R, int C, int Cp)
{
	/* SSE RowMajor */
	__m128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
	float *Bp, *A_row;
	int  x, y;
	A_row=A;

	for (y=0; y< R; y++)
	{
		Bp=B;
		xmm7=_mm_set_ps1(0);
		for (x=0; x<=C-28;x+=28)
		{
			ADDER(0,1);
			ADDER(2,3);
			ADDER(4,5);
			ADDER(6,0);
			ADDER(1,2);
			ADDER(3,4);
			ADDER(5,6);
		}

		for (; x<=C-4;x+=4)
		{
			ADDER(0,1);
		}
		
		for (; x<C;x++)
		{
			S[y]+=A_row[0]*Bp[0]; A_row++; Bp++;
		}

		A_row +=  Cp-C;

		S[y]+=sum_v4sp(xmm7);
	}
}

static __inline int fann_base_sse_neuron_constructor(struct fann *ann, struct fann_layer *layer, 
		struct fann_neuron *neuron, struct fann_neuron_descr * descr)
{
	unsigned int i, j, num_inputs_padded;

#ifdef FIXEDFANN
	fann_type multiplier = ann->fixed_params->multiplier;
	neuron->activation_steepness = ann->fixed_params->multiplier / 2;
#else
	neuron->activation_steepness = 0.5;
#endif

	neuron->activation_function = FANN_SIGMOID_STEPWISE;
	neuron->num_outputs=descr->num_outputs;
	neuron->inputs=layer->inputs;
	neuron->num_inputs=layer->num_inputs;

	/* set the error array to null (lazy allocation) */
	neuron->train_errors=NULL;

	/* allocate the weights */
	num_inputs_padded= (neuron->num_inputs+3)>>2<<2;
	neuron->num_weights=neuron->num_outputs*num_inputs_padded;
	
	if ( (neuron->weights = (fann_type*) _mm_calloc(neuron->num_weights*sizeof(fann_type), 16)) == NULL)
		return 1;

	/* randomly initialize the weights */
	for (i=0; i<neuron->num_outputs; i++)
		for (j=0; j<neuron->num_inputs; j++)
			neuron->weights[i*num_inputs_padded+j] = (fann_type) fann_random_weight();

	/* allocate space for the dot products results */
	if ( (neuron->sums = (fann_type*) _mm_malloc(neuron->num_outputs*sizeof(fann_type), 16)) == NULL)
		return 1;
	return 0;
}

static __inline  void  fann_base_sse_neuron_train_initialize(struct fann *ann, struct fann_layer *layer, struct fann_neuron *neuron)
{
	neuron->num_backprop_done=0;

	/* allocate the weights_deltas */
	if(neuron->weights_deltas == NULL)
	{
		if ( (neuron->weights_deltas = (fann_type*) _mm_calloc(neuron->num_weights*sizeof(fann_type), 16)) == NULL)
		{
			fann_error((struct fann_error *) ann, FANN_E_CANT_ALLOCATE_MEM);
			return;
		}
	}
	else
	{
		/* clear the error variables */
		memset(neuron->weights_deltas, 0,  neuron->num_weights*sizeof(fann_type));
	}
}

#undef fann_base_neuron_backprop
#ifndef FIXEDFANN
#define  fann_base_neuron_backprop(ann, neuron, prev_layer_errors)\
{\
	unsigned int o, j;\
	fann_type *errors, *inputs, *outputs, *weights, *sums, *deltas;\
	fann_type tmp;\
	const unsigned int num_outputs = neuron->num_outputs;\
	const unsigned int num_inputs = neuron->num_inputs;\
	const unsigned int num_inputs_padded = (num_inputs+3)>>2<<2;\
	\
	/* some assignments to speed up things */\
	errors = neuron->train_errors;\
	inputs = neuron->inputs;\
	outputs = neuron->outputs;\
	weights = neuron->weights;\
	deltas = neuron->weights_deltas;\
	sums = neuron->sums;\
	\
	/* detect if we are on the first layer (if so we get no prev_layer_errors) */\
	if (prev_layer_errors != NULL)\
	{\
		for (o = 0; o < num_outputs; o++)\
		{\
			/* multiply errors with the activation function derivative. the errors array must have been already allocated. */\
			activation_derived_macro()(neuron, o, tmp);\
			errors[o] = errors[o] * tmp;\
			\
			for (j = 0; j < num_inputs; j++)\
			{\
				/* calculate the weight deltas */\
				deltas[j] += errors[o] * inputs[j];\
				\
				/* calculate the error for previous layer */\
				prev_layer_errors[j] += errors[o] * weights[j];\
			}\
			weights += num_inputs_padded;\
			deltas += num_inputs_padded;\
		}\
	}\
	else\
	{ /* If on the first layer, don't backpropagate the errros */\
		for (o = 0; o < num_outputs; o++)\
		{\
			/* multiply errors with the activation function derivative. the errors array must have been already allocated. */\
			activation_derived_macro()(neuron, o, tmp);\
			errors[o] = errors[o] * tmp;\
			\
			for (j = 0; j < num_inputs; j++)\
			{\
				/* calculate the weight deltas */\
				deltas[j] += errors[o] * inputs[j];\
			}\
			weights += num_inputs_padded;\
			deltas += num_inputs_padded;\
		}\
	}\
	\
	neuron->num_backprop_done++;\
}

#else /* FIXEDFANN */

/* BUG: Function body needed! */
#define fann_base_neuron_backprop(ann, neuron, prev_layer_errors)\
{\
}

#endif 

static __inline  int  fann_base_sse_layer_constructor(struct fann *ann, 
		struct fann_layer *layer, struct fann_layer_descr *descr)
{
	/* sanity checks are done before calling this function */
	struct fann_neuron_descr *neurons_descr = descr->neurons_descr;
	unsigned int i=0;
	fann_type *free_output;
	struct fann_neuron *n;
	
	/*ALIGN FANN_INPUT*/
	if (layer->inputs == ann->inputs && 
			( ((unsigned int)ann->inputs) & 15))
	{
		float *tmp=_mm_calloc((ann->num_input+1) * sizeof(fann_type), 16);
		tmp[ann->num_input]=1;
		layer->inputs=tmp;
		fann_safe_free(ann->inputs);
		ann->inputs=tmp;
	}
  
#ifdef FIXEDFANN
	fann_type multiplier = ann->fixed_params->multiplier;
#endif

	layer->num_neurons = descr->num_neurons;

	/* count the number of outputs for the layer */
	layer->num_outputs = 0;

	for (i=0; i< layer->num_neurons; i++)
	{
		layer->num_outputs += neurons_descr[i].num_outputs;
	}

	layer->num_outputs++;	/* +1 for bias */

	/* set the error array to null (lazy allocation)*/
	layer->train_errors=NULL;

	/* allocate the outputs array */
	free_output = layer->outputs = (fann_type*) _mm_calloc(layer->num_outputs*sizeof(fann_type),16);
	if( layer->outputs == NULL)
	{
		return 1;
	}

	/* set bias output to 1*/
#ifdef FIXEDFANN
	layer->outputs[layer->num_outputs-1]=multiplier;
#else
	layer->outputs[layer->num_outputs-1]=1;
#endif

	/* allocate the neurons array */
	if( (layer->first_neuron = (struct fann_neuron *) calloc(layer->num_neurons, sizeof(struct fann_neuron))) == NULL)
	{
		return 1;
	}

	layer->last_neuron=layer->first_neuron+layer->num_neurons;

	for (i=0; i< layer->num_neurons; i++)
	{
		n=layer->first_neuron+i;

		n->outputs=free_output;
		if( neurons_descr[i].constructor(ann, layer, n, neurons_descr+i) != 0)
		{
			return 1;
		}
		free_output+=n->num_outputs;
	}
	return 0;
}

#undef fann_base_neuron_constructor
#define fann_base_neuron_constructor(ann, layer, neuron, descr) fann_base_sse_neuron_constructor(ann, layer, neuron, descr)
#undef fann_base_neuron_compute_MSE
#define fann_base_neuron_compute_MSE(ann, neuron, desired_output) MAKE_NAME(base_neuron_compute_MSE)(ann, neuron, desired_output)
/*
#undef fann_base_neuron_train_initialize
#define fann_base_neuron_train_initialize(ann, layer, neuron) fann_base_sse_neuron_train_initialize(ann, layer, neuron)
*/
#undef fann_base_layer_constructor
#define fann_base_layer_constructor(ann, layer, descr) fann_base_sse_layer_constructor(ann, layer, descr) 
/*
#undef fann_base_layer_train_initialize
#define fann_base_layer_train_initialize(ann, layer) fann_base_sse_layer_train_initialize(ann, layer) 
*/


#endif /* __fann_sse_base_c */

/* OUTSIDE HEADER GUARD !!!!*/

static __inline  void  MAKE_NAME(base_neuron_run)(struct fann * ann, struct fann_neuron * neuron) 
{
	struct MAKE_NAME(neuron_private_data) *priv = (struct MAKE_NAME(neuron_private_data) *) neuron->private_data;
	unsigned int o, num_connections, num_inputs, num_inputs_padded, num_outputs;
	fann_type *neuron_sums, *outputs, *inputs, *weights;
	unsigned int activation_function;
	fann_type steepness;
	fann_type max_sum = 0;	

	/* Algorith for fully connected networks */
	activation_function = neuron->activation_function;
	steepness = neuron->activation_steepness;
  
	num_inputs = neuron->num_inputs;
	num_inputs_padded = priv->num_inputs_padded;
	inputs = neuron->inputs;
	num_outputs = neuron->num_outputs;
	outputs = neuron->outputs;
	num_connections = neuron->num_inputs;
	weights = neuron->weights;
	neuron_sums=neuron->sums;
	
	memset(neuron_sums,0,num_outputs*sizeof(fann_type));
	MatVectMult(weights, inputs, neuron_sums, num_outputs, num_inputs, num_inputs_padded);
  
	for (o=0; o<num_outputs ; o++)
	{
		neuron_sums[o] = fann_mult(steepness, neuron_sums[o]);
		
		max_sum = 150/steepness;
		if(neuron_sums[o] > max_sum)
			neuron_sums[o] = max_sum;
		else if(neuron_sums[o] < -max_sum)
			neuron_sums[o] = -max_sum;
		
		activation_macro()(neuron, o);
	}
}


static __inline  void  MAKE_NAME(base_neuron_compute_MSE)(struct fann *ann, struct fann_neuron *neuron, fann_type *desired_output)
{
	unsigned int o;
	fann_type neuron_value, neuron_diff;
	fann_type *error_it, *output_it, *sum_it;

	/* assign each neuron its piece of train_errors array */
	error_it = neuron->train_errors;
	output_it = neuron->outputs;
	sum_it = neuron->sums;

	if(ann->training_params->train_error_function)
		/* TODO make switch when more functions */
		for (o = 0; o < neuron->num_outputs; o++)
		{
			neuron_value = output_it[o];
			neuron_diff = desired_output[o] - neuron_value;

			update_MSE_macro()(ann, neuron, &neuron_diff);

			if(neuron_diff < -.9999999)
				neuron_diff = -17.0;
			else if(neuron_diff > .9999999)
				neuron_diff = 17.0;
			else
				neuron_diff = (fann_type) log((1.0 + neuron_diff) / (1.0 - neuron_diff));

			error_it[o] = neuron_diff;
			ann->training_params->num_MSE++;

		}
	else
		for (o = 0; o < neuron->num_outputs; o++)
		{
			neuron_value = output_it[o];
			neuron_diff = desired_output[o] - neuron_value;

			update_MSE_macro()(ann, neuron, &neuron_diff);

			error_it[o] = neuron_diff;
			ann->training_params->num_MSE++;

		}
}


/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap noet
 */
