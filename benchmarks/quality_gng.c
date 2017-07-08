#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fann.h"
#include "ctimer.h"
#include "fann_gng.h"

void quality_benchmark_fann_gng(struct fann_train_data *train_data,
				struct fann_train_data *test_data,
				FILE * train_out,
				unsigned int num_input, 
				unsigned int seconds_of_training,
				double seconds_between_reports)
{
	float test_error;
	double elapsed = 0;
	double total_elapsed = 0;
	struct fann *ann;
	unsigned int current_example = 0;

	
	ann = fann_create_gng(num_input);
	ann->gng_params->gng_max_nodes = 2000000;

	calibrate_timer();

	while(total_elapsed < (double) seconds_of_training)
	{
		/* train */
		elapsed = 0;
		start_timer();
		while(elapsed < (double) seconds_between_reports)
		{
		        fann_train_example_gng(ann, train_data, current_example, 0);

			elapsed = time_elapsed();
			current_example++;
		}
		stop_timer();
		total_elapsed += getSecs();

		/* make report */
		test_error = fann_get_MSE_gng(ann, test_data);

		/* output the statistics */
		fprintf(train_out, "%8.2f %8.6f %5d\n", total_elapsed, test_error, current_example);
		fprintf(stderr, "secs: %8.2f, test_error: %8.6f training_examples: %5d    num_cells: %d\r",
			total_elapsed, test_error, current_example, ann->gng_params->gng_num_cells);

	}

	fprintf(stdout, "\nTraining samples: %d, samples/sec: %f\n", current_example, (float)current_example / total_elapsed);

	fann_destroy(ann);
	}




int main(int argc, char *argv[])
{
	/* parameters */
	char output_file[256];

	if(argc != 7)
	{
		printf("usage %s train_file test_file output_file seconds_of_training seconds_between_reports number_of_runs\n", argv[0]);
		return -1;
	}

	unsigned int seconds_of_training = atoi(argv[4]);
	double seconds_between_reports = atof(argv[5]);
	unsigned int number_of_runs = atoi(argv[6]);

	fann_train_data *train_data = fann_read_train_from_file(argv[1]);
	fann_train_data *test_data = fann_read_train_from_file(argv[2]);

	char *train_out_file = argv[3];
	FILE *train_out = 0;

	fann_seed_rand();

	for(unsigned int i = 0; i < number_of_runs; i++)
	{
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
	
		quality_benchmark_fann_gng(train_data, test_data, train_out, train_data->num_input, seconds_of_training, seconds_between_reports);
		
		
		fclose(train_out);
	}

	fann_destroy_train(train_data);
	fann_destroy_train(test_data);

	return 0;
}
