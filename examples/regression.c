#include "fann.h"
#include <sys/time.h>

#ifdef MIMO_FANN
#undef MIMO_FANN
#define MIMO_FANN 1
#else
#define MIMO_FANN 0
#endif

#define I_DIM 2
#define O_DIM 1
#ifndef H_DIM
#define H_DIM 2
#endif
#define S_DIM 2
#define E_DES 0
#define N_EPR 10
#define R_NUM 100
#define T_NUM 100

#define USE_XOR_DATA 1
#define USE_RPROP 1
#define INIT_WEIGHTS 0

#ifndef VERBOSE
#define VERBOSE 3
#endif

#ifndef OPTIMIZE
#define OPTIMIZE 4
#endif

/*
#define SPARSE 0.7
*/

#define vals(v) str(v)
#define concat2(v1,v2,v3) str(v1 ## _ ## v2 ## _ ## v3)
#define concat(v1,v2,v3) concat2(v1,v2,v3)

#if USE_RPROP == 1
#define Algorithm rprop
#else
#define Algorithm batch
#endif

#define Activation sigmoid_symmetric

#if MIMO_FANN
#if OPTIMIZE == 1 || OPTIMIZE == 2
#include "optimized/scalar/fann.h"
#define Implementation scalar
#define layer_type "connected_any_any"
#endif
#if OPTIMIZE == 3
#include "optimized/blas/fann.h"
#define Implementation blas
#define layer_type "connected_any_any"
#endif
#if OPTIMIZE == 4
#include "optimized/sse/fann.h"
#define Implementation sse
#define layer_type concat(Implementation, Algorithm, Activation)
#endif
#endif

#define neuron_type concat(Implementation, Algorithm, Activation)

/*a regresion test*/

struct timeval tv_start;
struct timeval tv_now;

void report(const char* title, float * S)
{
	printf("%s: \n ", title);
	printf("\t - Elapsed: %f sec\n", ( 
				( tv_now.tv_sec   * 1000000.0  + tv_now.tv_usec   ) - 
				( tv_start.tv_sec * 1000000.0  + tv_start.tv_usec )
				)/1000000.0
	);
}

void f(struct fann_train_data *data, unsigned int pos)
{
	fann_type *in=data->input[pos], *out=data->output[pos];
	unsigned int idim=data->num_input, odim=data->num_output;
	unsigned int size=data->num_data;
	int i,j;
	for (i=0; i< idim; i++)
	{
		in[i]+= (0.5 * pos)/size+(fann_type) + (0.5*i) / idim;
	}

	for (j=0; j<odim; j++)
	{
		out[j]=0;
		for (i=0; i< idim; i++)
		{
			out[j] +=(0.3/idim) *in[i];
		}
	}
}

void f1(struct fann_train_data *data, unsigned int pos)
{
	fann_type *in=data->input[pos], *out=data->output[pos];
	unsigned int idim=data->num_input, odim=data->num_output;
	unsigned int size=data->num_data;
	int i,j;
	for (i=0; i< idim; i++)
	{
		in[i]+= ((float)pos)/size;
	}

	for (j=0; j<odim; j++)
	{
		out[j]=1;
		for (i=0; i< idim; i++)
		{
			out[j] *= in[i]>0?1:0;
		}
	}
}


#if MIMO_FANN
void print_deltas(struct fann *ann, unsigned int iter)
{
	struct fann_layer *layer_it;
	struct fann_neuron *neuron_it;
	unsigned int l, n, o, j;
	unsigned int num_mimo_in;
	struct fann_neuron_private_data_connected_any_any *priv;

	l=0;
	for (layer_it=ann->first_layer; layer_it!= ann->last_layer; layer_it ++, l++)
	{
		printf("   Layer %u\n", l);
		for (n=0; n<layer_it->num_neurons; n++)
		{
			neuron_it=layer_it->first_neuron+n;
			priv = (struct fann_neuron_private_data_connected_any_any *) neuron_it->private_data;
#if OPTIMIZE == 4
			num_mimo_in=(neuron_it->num_inputs+3)>>2<<2 ;
#else
			num_mimo_in=neuron_it->num_inputs;
#endif
			for (o=0; o<neuron_it->num_outputs; o++)
			{
				printf("\tNeuron %u\n", o);

				printf("\t\t - weights[%u][%u][%u] = [", iter, l, o );
				for (j=0; j<neuron_it->num_inputs; j++)
					printf(" %.10e,",neuron_it->weights[o*num_mimo_in+j]);
				printf("]\n");

				printf("\t\t -  inputs[%u][%u][%u] = [", iter, l, o );
				for (j=0; j<neuron_it->num_inputs; j++)
				{
					printf(" %.10e,",neuron_it->inputs[j]);
				}
				printf("]\n");

				printf("\t\t - tdeltas[%u][%u][%u] = [", iter, l, o );
				for (j=0; j<neuron_it->num_inputs; j++)
					printf(" %.10e,", (neuron_it->weights_deltas)[o*num_mimo_in+j]);
				printf("]\n");

#if USE_RPROP
				printf("\t\t - prev_de[%u][%u][%u] = [", iter, l, o );
				for (j=0; j<neuron_it->num_inputs; j++)
					printf(" %.10e,", (priv->prev_weights_deltas)[o*num_mimo_in+j]);
				printf("]\n");
				
				printf("\t\t - prev_st[%u][%u][%u] = [", iter, l, o );
				for (j=0; j<neuron_it->num_inputs; j++)
				{
					printf(" %.10e,", priv->prev_steps[o*num_mimo_in+j]);
				}
				printf("]\n");
#endif			/*USE_RPROP*/	
				printf("\t\t -     sum[%u][%u][%u] = ", iter, l, o );
				printf(" %.10e", neuron_it->sums[o]);
				printf("\n");

				printf("\t\t -  output[%u][%u][%u] = ", iter, l, o );
				printf(" %.10e", layer_it->outputs[o]);
				printf("\n");

				printf("\t\t -   error[%u][%u][%u] = ", iter, l, o );
				printf(" %.10e", neuron_it->train_errors[o]);
				printf("\n");
			}
		}
	}
}
#else
void print_deltas(struct fann *ann, unsigned int iter)
{
	struct fann_layer *layer_it;
	struct fann_neuron *neuron_it;
	unsigned int l, n, c;
	unsigned int num_connections, num_neurons;

	l=0;
	for (layer_it=ann->first_layer+1; layer_it!= ann->last_layer; layer_it ++, l++)
	{
		printf("   Layer %u\n", l);
		num_neurons=layer_it->last_neuron - layer_it->first_neuron-1; /*skip bias*/
		for (n=0; n<num_neurons; n++)
		{
			neuron_it=layer_it->first_neuron+n;
			printf("\tNeuron %u\n", n);
			num_connections=neuron_it->last_con - neuron_it->first_con;

			printf("\t\t - weights[%u][%u][%u] = [", iter, l, n );
			for (c=0; c<num_connections; c++)
			{
				printf(" %.10e,",ann->weights[neuron_it->first_con+c]);
			}
			printf("]\n");

			printf("\t\t -  inputs[%u][%u][%u] = [", iter, l, n );
			for (c=0; c<num_connections; c++)
			{
				printf(" %.10e,", ann->connections[neuron_it->first_con+c]->value);
			}
			printf("]\n");

			printf("\t\t - tdeltas[%u][%u][%u] = [", iter, l, n );
			if (ann->train_slopes)
			{
				for (c=0; c<num_connections; c++)
				{
					printf(" %.10e,",ann->train_slopes[neuron_it->first_con+c]);
				}
			}
			printf("]\n");
			
#if USE_RPROP
			printf("\t\t - prev_de[%u][%u][%u] = [", iter, l, n );
			if (ann->prev_train_slopes)
			{
				for (c=0; c<num_connections; c++)
				{
					printf(" %.10e,",ann->prev_train_slopes[neuron_it->first_con+c]);
				}
			}
			printf("]\n");

			printf("\t\t - prev_st[%u][%u][%u] = [", iter, l, n );
			if (ann->prev_steps)
			{
				for (c=0; c<num_connections; c++)
				{
					printf(" %.10e,",ann->prev_steps[neuron_it->first_con+c]);
				}
			}
			printf("]\n");
#endif
			printf("\t\t -     sum[%u][%u][%u] = ", iter, l, n );
			printf(" %.10e", neuron_it->sum);
			printf("\n");

			printf("\t\t -  output[%u][%u][%u] = ", iter, l, n );
			printf(" %.10e", neuron_it->value);
			printf("\n");

			printf("\t\t -   error[%u][%u][%u] = ", iter, l, n );
			printf(" %.10e", ann->train_errors[neuron_it-ann->first_layer->first_neuron]);
			printf("\n");
		}
	}
}

#endif

float train_epoch_debug(struct fann *ann, struct fann_train_data* data, unsigned int iter)
{
	unsigned int i;
#if VERBOSE>=2
	static unsigned int j=0;
#endif

#if ! MIMO_FANN
	if (ann->prev_train_slopes==NULL)
		fann_clear_train_arrays(ann);
#endif

	fann_reset_MSE(ann);

	for(i = 0; i < data->num_data; i++)
	{
		fann_run(ann, data->input[i]);
		fann_compute_MSE(ann, data->output[i]);
		fann_backpropagate_MSE(ann);
#if ! MIMO_FANN
		fann_update_slopes_batch(ann, ann->first_layer + 1, ann->last_layer - 1);
#endif

#if VERBOSE>=3
		printf("   ** %d:%d **-AFTER-DELTAS UPDATE-----------------------------------\n", iter, i);
		print_deltas(ann, j++);
#endif

	}
#if VERBOSE>=2
	printf("   ** %d **-BEFORE-WEIGHTS-UPDATE------------------------------------\n", iter);
	print_deltas(ann, j++);
#endif

#if ! MIMO_FANN
#if USE_RPROP
	fann_update_weights_irpropm(ann, 0, ann->total_connections);
#else
	fann_update_weights_batch(ann, data->num_data, 0, ann->total_connections);
#endif
#else /* MIMO_FANN */
	fann_update_weights(ann);
#endif

#if VERBOSE>=1
	printf("   ** %d **-AFTER-WEIGHTS-UPDATE-------------------------------------\n", iter);
	print_deltas(ann, j++);
#endif

	return fann_get_MSE(ann);
}

static __inline void fann_run_data(struct fann *ann, struct fann_train_data *data)
{
	unsigned int i;
	
	for(i = 0; i != data->num_data; i++)
	{
		fann_run(ann, data->input[i]);
	}
}

struct fann * setup_net(struct fann_train_data * data)
{
	struct fann *ann;
#if MIMO_FANN
#if OPTIMIZE == 0
	ann = fann_create_standard( 3, data->num_input, H_DIM, data->num_output);
	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
#endif

#if OPTIMIZE == 1
	unsigned int i, j;
	struct fann_descr *descr=(struct fann_descr*) calloc(1, sizeof(struct fann_descr));
	fann_setup_descr(descr, 2, data->num_input);


	i=0;

	fann_setup_layer_descr(
					&(descr->layers_descr[i]),
					"connected_any_any",
					1,
					NULL
					);

		for (j=0; j< descr->layers_descr[i].num_neurons; j++)
		{
						fann_setup_neuron_descr(
							descr->layers_descr[i].neurons_descr+j,
							H_DIM,
							"scalar_rprop_sigmoid_symmetric",
							NULL
							);
		}

	i=1;

	fann_setup_layer_descr(
					&(descr->layers_descr[i]),
					"connected_any_any",
					1,
					NULL
					);

		for (j=0; j< descr->layers_descr[i].num_neurons; j++)
		{
						fann_setup_neuron_descr(
							descr->layers_descr[i].neurons_descr+j,
							data->num_output,
							"scalar_rprop_sigmoid_symmetric",
							NULL
							);
		}
	ann = fann_create_from_descr( descr );
#endif

#if OPTIMIZE >= 2
	{
		unsigned int layers[] = { data->num_input, H_DIM, data->num_output };
		/*char *type;
		asprintf(&type, "%s_%s_%s", vals(implementation), vals(algorithm), vals(activation));*/

		ann = fann_create_standard_array_typed(layer_type, neuron_type, 3,  layers);

	}
#endif
#else /*MIMO_FANN*/

#ifdef SPARSE
	ann = fann_create_sparse( SPARSE, 3, data->num_input, H_DIM, data->num_output);
#else
	ann = fann_create_standard( 3, data->num_input, H_DIM, data->num_output);
#endif
	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

#endif /*MIMO_FANN*/ 

	fann_set_train_stop_function(ann, FANN_STOPFUNC_BIT);
	fann_set_bit_fail_limit(ann, 0.01f);
	fann_set_activation_steepness_hidden(ann, 1);
	fann_set_activation_steepness_output(ann, 1);

#if INIT_WEIGHTS == 1
	fann_randomize_weights(ann,0,1);
#endif
#if INIT_WEIGHTS == 2
	fann_init_weights(ann, data);
#endif

#ifdef USE_RPROP
	fann_set_training_algorithm(ann, FANN_TRAIN_RPROP);
#else
	fann_set_training_algorithm(ann, FANN_TRAIN_BATCH);
#endif

	return ann;
}

int main( int argc, char ** argv)
{
	float mse=1000;
	unsigned int num_train=R_NUM;
	unsigned int num_test=T_NUM;
	struct fann_train_data* data ;
	unsigned int i;
	const float desired_error = (const float) E_DES;
	const unsigned int epochs_between_reports = N_EPR;
	unsigned int bitf_limit=0;
	unsigned int bitf=bitf_limit+1;
	struct fann *ann;

#if MIMO_FANN
	printf("MIMO fann\n");
#else
	printf("Old fann\n");
#endif

#ifdef USE_XOR_DATA
	if (argc<2)
	{
		printf("Error: please supply a data file\n");
		return -1;
	}
	printf("Using %s\n", argv[1]);
	data=fann_read_train_from_file(argv[1]);
#else
	printf("Generating training data\n");
	data = fann_create_train(S_DIM, I_DIM, O_DIM);
	for ( i=0; i< S_DIM; i++)
	{
		f1(data, i); 
	}
#endif

	ann=setup_net(data);

#if VERBOSE
	fann_print_parameters(ann);
#endif

	for (i=0; mse>desired_error && i!=num_train && bitf>bitf_limit; i++)
	{
#if VERBOSE
		mse=train_epoch_debug(ann, data, i);
#else 
		mse=fann_train_epoch(ann, data);
#endif
		bitf=fann_get_bit_fail(ann);
		if ( !((i) % epochs_between_reports))
			printf("Epochs     %8d. Current error: %.10f. Bit fail: %u\n", i+(!i), mse, bitf);
			/*printf ("[ %7u ] MSE Error : %.10e ###################\n", i, mse);*/
	}
	printf("Epochs     %8d. Current error: %.10f. Bit fail: %u\n", i+(!i), mse, bitf);

	printf("Testing network. %f\n", fann_test_data(ann, data));
	
	gettimeofday(&tv_start,NULL);
	for (i=0; i!=num_test; i++)
 		fann_run_data(ann, data);
	gettimeofday(&tv_now,NULL);
	report("---",0);	
	

#if 1
	printf("Trying to save network\n");
#if MIMO_FANN 
	fann_save(ann, "saved_mimo.net");
	fann_destroy(ann);
	ann=fann_create_from_file("saved_mimo.net");
	fann_save(ann, "saved_mimo2.net");
	fann_destroy(ann);
#else
	fann_save(ann, "saved_old.net");
#endif
#endif

	return 0;
}

/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap noet
 */
