#include "fann.h"

#define I_DIM 2
#define O_DIM 1
#define S_DIM 2
#undef H_DIM
#define H_DIM 3

#define R_NUM 100

#define VERBOSE 0
#define USE_RPROP 1
#undef OPTIMIZE
#define OPTIMIZE 1
/*
#define RANDOMIZE 1
#define SPARSE 0.7
*/

#include "optimized/scalar/fann.h"

struct fann * setup_net(struct fann_train_data * data)
{
	struct fann *ann;
#if OPTIMIZE == 0
#ifdef SPARSE
	ann = fann_create_sparse( SPARSE, 3, data->num_input, H_DIM, data->num_output);
#else
	ann = fann_create_standard( 3, data->num_input, H_DIM, data->num_output);
#endif
	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
#endif

#if OPTIMIZE == 1
#if USE_RPROP
	ann = fann_create_standard_scalar_rprop_sigmoid_symmetric( 3, data->num_input, H_DIM, data->num_output);
#else
	ann = fann_create_standard_scalar_batch_sigmoid_symmetric( 3, data->num_input, H_DIM, data->num_output);
#endif
#endif

#if OPTIMIZE == 2
	unsigned int i, j;
	struct fann_descr *descr=(struct fann_descr*) calloc(1, sizeof(struct fann_descr));
	fann_setup_descr(descr, 2, data->num_input);


	i=0;

	fann_setup_layer_descr(
					&(descr->layers_descr[i]),
					fann_layer_constructor_connected_any_any,
					1,
					NULL
					);

		for (j=0; j< descr->layers_descr[i].num_neurons; j++)
		{
						fann_setup_neuron_descr(
							descr->layers_descr[i].neurons_descr+j,
							H_DIM,
							fann_neuron_constructor_scalar_rprop_sigmoid_symmetric,
							NULL
							);
		}

	i=1;

	fann_setup_layer_descr(
					&(descr->layers_descr[i]),
					fann_layer_constructor_connected_any_any,
					1,
					NULL
					);

		for (j=0; j< descr->layers_descr[i].num_neurons; j++)
		{
						fann_setup_neuron_descr(
							descr->layers_descr[i].neurons_descr+j,
							data->num_output,
							fann_neuron_constructor_scalar_rprop_sigmoid_symmetric,
							NULL
							);
		}
	ann = fann_create_from_descr( descr );
#endif

	fann_set_train_stop_function(ann, FANN_STOPFUNC_BIT);
	fann_set_bit_fail_limit(ann, 0.01f);
	fann_set_activation_steepness_hidden(ann, 1);
	fann_set_activation_steepness_output(ann, 1);

#ifdef RANDOMIZE
	fann_randomize_weights(ann,0,1);
#else
	fann_init_weights(ann, data);
#endif

#ifdef USE_RPROP
	fann_set_training_algorithm(ann, FANN_TRAIN_RPROP);
#else
	fann_set_training_algorithm(ann, FANN_TRAIN_BATCH);
#endif

	return ann;
}

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
			for (o=0; o<neuron_it->num_outputs; o++)
			{
				num_mimo_in=neuron_it->num_inputs;
				printf("\tNeuron %u\n", o);

				printf("\t\t - weights[%u][%u][%u] = [", iter, l, o );
				for (j=0; j<num_mimo_in; j++)
				{
					printf(" %.10e,",neuron_it->weights[o*num_mimo_in+j]);
				}
				printf("]\n");

				printf("\t\t -  inputs[%u][%u][%u] = [", iter, l, o );
				for (j=0; j<num_mimo_in; j++)
				{
					printf(" %.10e,",neuron_it->inputs[j]);
				}
				printf("]\n");

				printf("\t\t - tdeltas[%u][%u][%u] = [", iter, l, o );
				for (j=0; j<num_mimo_in; j++)
				{
					printf(" %.10e,", (neuron_it->weights_deltas)[o*num_mimo_in+j]);
				}
				printf("]\n");

#if USE_RPROP
				printf("\t\t - prev_de[%u][%u][%u] = [", iter, l, o );
				for (j=0; j<num_mimo_in; j++)
				{
					printf(" %.10e,", (priv->prev_weights_deltas)[o*num_mimo_in+j]);
				}
				printf("]\n");
				
				printf("\t\t - prev_st[%u][%u][%u] = [", iter, l, o );
				for (j=0; j<num_mimo_in; j++)
				{
					printf(" %.10e,", priv->prev_steps[o*num_mimo_in+j]);
				}
				printf("]\n");
#endif				
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

float train_epoch_debug(struct fann *ann, struct fann_train_data* data, unsigned int iter)
{
	unsigned int i;
#if VERBOSE>=3
	static unsigned int j=0;
#endif

	fann_reset_MSE(ann);

	for(i = 0; i < data->num_data; i++)
	{
		fann_run(ann, data->input[i]);
		fann_compute_MSE(ann, data->output[i]);
		fann_backpropagate_MSE(ann);

#if VERBOSE>=3
		printf("   ** %d:%d **-AFTER-DELTAS UPDATE-----------------------------------\n", iter, i);
		print_deltas(ann, j++);
#endif

	}
#if VERBOSE>=2
	printf("   ** %d **-BEFORE-WEIGHTS-UPDATE------------------------------------\n", iter);
	print_deltas(ann, j++);
#endif

	fann_update_weights(ann);

#if VERBOSE>=2
	printf("   ** %d **-AFTER-WEIGHTS-UPDATE-------------------------------------\n", iter);
	print_deltas(ann, j++);
#endif

	return fann_get_MSE(ann);
}

int main()
{
	float mse=1000;
	unsigned int num_train=R_NUM;
	struct fann_train_data* data ;
	unsigned int i;
	fann_type *calc_out;
	const float desired_error = (const float) 0;
	const unsigned int epochs_between_reports = 10;
	unsigned int bitf_limit=0;
	unsigned int bitf=bitf_limit+1;
	struct fann *ann;

	printf("Using xor.data\n");
	data=fann_read_train_from_file("xor.data");
	ann=setup_net(data);
	/*fann_print_parameters(ann);*/
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

	for(i = 0; i < fann_length_train_data(data); i++)
	{
		calc_out = fann_run(ann, data->input[i]);
		printf("XOR test (%f,%f) -> %f, should be %f, difference=%f\n",
				data->input[i][0], data->input[i][1], calc_out[0], data->output[i][0],
				fann_abs(calc_out[0] - data->output[i][0]));
	}
	return 0;
}

/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap noet
 */
