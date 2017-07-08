#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fann.h"
#include "ctimer.h"
#include "fann_som.h"

void quality_benchmark_fann_som(unsigned int width,
				unsigned int height,
				char *topology,
				char *neighborhood,
				char *decay,
				struct fann_train_data *train_data,
				struct fann_train_data *test_data,
				FILE * train_out,
				unsigned int num_input, 
				unsigned int max_training_examples, double seconds_between_reports)
{
	float test_error;
	double elapsed = 0;
	double total_elapsed = 0;
	struct fann *ann;
	unsigned int current_example = 0;

	
	ann = fann_create_som(width, height, num_input);

	/* Set the topology */
	if (!strcmp(topology, "hexagonal"))
	{
	        ann->som_params->som_topology = FANN_SOM_TOPOLOGY_HEXAGONAL;
	}
	else if (!strcmp(topology, "rectangular"))
	{
	        ann->som_params->som_topology = FANN_SOM_TOPOLOGY_RECTANGULAR;
	}
	else 
        {
	        fprintf(stderr, "Error: Unknown topology: %s\n", topology);
		exit(1);
	}
	
	/* Set the neighborhood */
	if (!strcmp(neighborhood, "distance"))
	{
	        ann->som_params->som_neighborhood = FANN_SOM_NEIGHBORHOOD_DISTANCE;
	}
	else if (!strcmp(neighborhood, "gaussian"))
	{
	        ann->som_params->som_neighborhood = FANN_SOM_NEIGHBORHOOD_GAUSSIAN;
	}
	else 
        {
	        fprintf(stderr, "Error: Unknown neighborhood: %s\n", neighborhood);
		exit(1);
	}

	/* Set the decay */
	if (!strcmp(decay, "linear"))
	{
	        ann->som_params->som_learning_decay = FANN_SOM_LEARNING_DECAY_LINEAR;
	}
	else if (!strcmp(decay, "inverse"))
	{
	        ann->som_params->som_learning_decay = FANN_SOM_LEARNING_DECAY_INVERSE;
	}
	else 
        {
	        fprintf(stderr, "Error: Unknown decay type: %s\n", decay);
		exit(1);
	}

	
	calibrate_timer();

	while (current_example < max_training_examples)
	{
		/* train */
		elapsed = 0;
		start_timer();
		while((elapsed < (double) seconds_between_reports) && (current_example < max_training_examples))
		{
		        fann_train_example_som(ann, train_data, current_example, max_training_examples);

			elapsed = time_elapsed();
			current_example++;
		}
		stop_timer();
		total_elapsed += getSecs();

		/* make report */
		test_error = fann_get_MSE_som(ann, test_data);

		/* output the statistics */
		fprintf(train_out, "%8.2f %8.6f %5d\n", total_elapsed, test_error, current_example);
		fprintf(stderr, "secs: %8.2f, test_error: %8.6f training_examples: %5d\r",
				total_elapsed, test_error, current_example);

	}

	fprintf(stdout, "\nTraining samples: %d, samples/sec: %f\n", max_training_examples, (float)max_training_examples / total_elapsed);

	fann_destroy(ann);
	}



int main(int argc, char *argv[])
{
	/* parameters */
	char output_file[256];

	if(argc != 12)
	{
		printf("usage %s train_file test_file output_file width height topology neighborhood decay max_training_examples seconds_between_reports number_of_runs\n", argv[0]);
		return -1;
	}

	unsigned int width = atoi(argv[4]);
	unsigned int height = atoi(argv[5]);
	unsigned int max_training_examples = atoi(argv[9]);
	double seconds_between_reports = atof(argv[10]);
	unsigned int number_of_runs = atoi(argv[11]);

	fann_train_data *train_data = fann_read_train_from_file(argv[1]);
	fann_train_data *test_data = fann_read_train_from_file(argv[2]);

	char *train_out_file = argv[3];
	FILE *train_out = 0;

	fann_seed_rand();

	for(unsigned int i = 0; i < number_of_runs; i++)
	{
	  /*		test_collector->newCollection();
	  		train_collector->newCollection();
	  */
		
		if(strlen(train_out_file) == 1 && train_out_file[0] == '-')
			train_out = stdout;
		else
		{
			if(number_of_runs == 1)
				train_out = fopen(train_out_file, "w");
			else
			{
				sprintf(output_file, "%s_%d_run", train_out_file, i);
				train_out = fopen(output_file, "w");
			}
		}
	
		fprintf(stdout, "Quality test of %s\n", argv[1]);
	
		quality_benchmark_fann_som(width, height, argv[6], argv[7], argv[8], train_data, test_data,
					   train_out, train_data->num_input, max_training_examples, seconds_between_reports);
		
		
		fclose(train_out);
	}

	fann_destroy_train(train_data);
	fann_destroy_train(test_data);

	return 0;
}
