/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003 Steffen Nissen (lukesky@diku.dk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


/*This code is currently unsupported and is present only for reference purpose. */

#include "include/fann.h"

#include "fann_gpu.h"
#ifdef WIN32
#include "windows.h"
LARGE_INTEGER start, end, freq;
#else
#include <time.h>
double start, end, run_time;
#endif

layer *A, *B, *C, *D, *E;
float *input, *output;


/*dummy vector data*/
void fillVector_bench(layer *target);
void fillWeights_bench(layer *target);


void do_test(FILE* out, int laysize, char *file){
	int count = 2000;
	int n = count;
	float tot = 0.0f;

	A = generateLayer(file, laysize, laysize, 0);
	B = generateLayer(file, laysize, laysize, 0);
	C = generateLayer(file, laysize, laysize, 0);
	D = generateLayer(file, laysize, laysize, 0);
	E = generateLayer(0, laysize, 0, 0);
	
	setOutput(A, B);
	setOutput(B, C);
	setOutput(C, D);
	setOutput(D, E);


	/*dummy values.*/
	/*fill vectors with values.*/

	fillWeights_bench(A);
	fillWeights_bench(B);
	fillWeights_bench(C);
	fillWeights_bench(D);

	output = malloc(sizeof(float)*E->size_rgba*4);

	/*Execute the network N times.*/
	while (n-->0){
		fillVector_bench(A);
		/*glFinish(); //finish all operations before starting the clock*/
#ifdef WIN32
		QueryPerformanceCounter(&start);
#else
		start = clock();
#endif
		copyVectorToTexture(input, A);
		run(A);
		run(B);
		run(C);
		/*run(D);*/
		copyVectorFromTexture(output, D);
		/*printLayer(E);*/
		/*glFinish(); //finish all operations before stopping the clock*/
#ifdef WIN32
		QueryPerformanceCounter(&end);
		QueryPerformanceFrequency( &freq );
		/*printf("Time in s: %f\n", ((double)(end.QuadPart - start.QuadPart))/(double)freq.QuadPart);*/
		/*printf("ns per conn: %f\n", ((double)(end.QuadPart - start.QuadPart))/(double)freq.QuadPart * 1000000000.0 / laysize /laysize / 4);*/
		tot += ((double)(end.QuadPart - start.QuadPart))/(double)freq.QuadPart * 1000000000.0 / laysize /laysize / 3;
#else
		end = clock();
		run_time = (end-start)/CLOCKS_PER_SEC*1000;
		printf("Time in ms: %d\n", (int)run_time);
#endif
		free(input);
	}

	/*clean up*/
	destroyLayer(A);
	destroyLayer(B);
	destroyLayer(C);
	destroyLayer(D);
	destroyLayer(E);
	free(output);
	/*fprintf(out, "GPU:  layer size: %4d, total avg ns per connection: %f\n", laysize, tot/count);*/
	fprintf(out, "%d %.20e\n",laysize, tot/count);
	fflush(out);
}


/*dummy vector data*/
void fillVector_bench(layer *target){
	int i;
	input = malloc(sizeof(float) * target->size);
	for(i=0; i<target->size; i++){
		input[i] = RAND_UNI;
		/*input[i] = i;*/
		/*input[i] = 1;*/
	}
}

void fillWeights_bench(layer *target){
	int i,j;
	float *weight_matrix = calloc(target->out_size * target->size, sizeof(float));
	float *mask_matrix = calloc(target->out_size * target->size, sizeof(float));

	for(i=0; i<target->out_size; i++){
		for(j=0; j<target->size; j++){
			weight_matrix[j+i*target->size] = RAND_UNI;
			/*weight_matrix[j+i*target->size] = j+i*target->size;*/
			/*weight_matrix[j+i*target->size] = 1.0f;*/
			mask_matrix[j+i*target->size] = 1;
		}
	}
	copyWeightsToTexture(weight_matrix, target);
	copyMaskToTexture(mask_matrix, target);
	free(weight_matrix);
	free(mask_matrix);
}

void testFann(FILE* out, int laysize){
	int total_connections;
	int count = 2000;
	float tot;
	int i;
	struct fann *ann;
	/*if (laysize>1000)*/
		/*return;*/
	ann = fann_create_standard(4, laysize, laysize, laysize, laysize);
	fann_set_activation_function_hidden(ann, FANN_SIGMOID_STEPWISE);
	fann_set_activation_function_output(ann, FANN_SIGMOID_STEPWISE);
	total_connections = (laysize + 1) * laysize * 3;

	QueryPerformanceCounter(&start);

	for(i = 0; i<count; i++)
	{
		output = fann_run(ann, input);
	}

	QueryPerformanceCounter(&end);
	QueryPerformanceFrequency( &freq );
	/*printf("Time in s: %f\n", ((double)(end.QuadPart - start.QuadPart))/(double)freq.QuadPart);*/
	/*printf("ns per conn: %f\n", ((double)(end.QuadPart - start.QuadPart))/(double)freq.QuadPart * 1000000000.0 / laysize /laysize / 4);*/
	tot += ((double)(end.QuadPart - start.QuadPart))/(double)freq.QuadPart * 1000000000.0 / laysize /laysize / 3;
	/*fprintf(out, "FANN: layer size: %4d, total avg ns per connection: %f\n", laysize, tot/count);*/
	fprintf(out, "%d %.20e\n",laysize, tot/count);
	fann_destroy(ann);
	fflush(out);

}


int main(int argc, char **arg){
	int size = 8;
	float data1[24] = {5,10,20,30,40,50,60,70,80,90,
		100,200,300,400,500,600,700,800,900,
		1000,1500,2000,2500,3000};
	float data[8] = {1100,1200,1300,1400,1500,2000,2500,3000};
	int i,k;
	char* error;
	FILE *fann_out;
	FILE *gpu_out;
	FILE *gpu_out2;
	FILE *out;

	/*start OpenGL*/
	initOpenGL();

	/*testing system compatibility*/
	if ((error = test()) != 0){
		printf("Error: %s\n", error);
		return -1;
	}

	/*initializing system.*/
	if (!init()){
		printf("Init not successful...");
		return -1;
	}
	fann_out = fopen("fann_results.txt", "w");
	gpu_out = fopen("gpu_results.txt", "w");
	gpu_out2 = fopen("gpu_results_masked.txt", "w");
	out = stdout;
	
/*
	fprintf(out, "-- sum.fp -----------------------------------------\n");
	for (i=0; i<size; i++){
		do_test(out, data[i],"sum.fp");
		if(out != stdout){
			for (k=-1; k<i; k++)printf(".");
			printf("\n");
		}
		fflush(out);
	}
*/
	fprintf(out, "\n-- sigmoid_sum.fp ---------------------------------\n");
	for (i=0; i<size; i++){
		do_test(gpu_out, data[i],"sigmoid_sum.fp");
		do_test(gpu_out2, data[i],"sigmoid_sum_masked.fp");
		testFann(fann_out, data[i]);
		for (k=-1; k<i; k++)printf(".");
		printf("\n");
		/*fprintf(out,"\n");*/
	}

/*
	fprintf(out, "\n-- sigmoid_sum_masked.fp --------------------------\n");
	for (i=0; i<size; i++){
		do_test(gpu_out2, data[i],"sigmoid_sum_masked.fp");
		if(out != stdout){
			for (k=-1; k<i; k++)printf(".");
			printf("\n");
		}
		/*fprintf(out,"\n");*/
		fflush(out);
	}
*/
	fclose(out);
	fclose(gpu_out);
	fclose(gpu_out2);
	fclose(fann_out);
	return 1;

}