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


#include "fann_gpu.h"
#ifdef WIN32
#include "windows.h"
LARGE_INTEGER start, end, freq;
#else
/*better unix timing?*/
#include <time.h>
double start, end, run_time;
#endif

layer *A, *B, *C, *D, *E;
float *input, *weight_matrix, *mask_matrix;
int i,j;


/*Dummy function for filling weights and mask with dummy data, it preserves the texture sizes wich is multiples of 4.*/
void fillWeights(layer *target);

/*dummy vector data*/
void fillVector(layer *target);


int main2(int argc, char **arg){
	char* error;
	int n = 3;

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

	/*create layers using the sigmoid_sum fragment program.*/
	A = generateLayer("sigmoid_sum_masked.fp", 4, 40, 0);
	B = generateLayer("sigmoid_sum_masked.fp", 40, 16, 0);
	C = generateLayer("sigmoid_sum_masked.fp", 40, 22, 16);
	D = generateLayer("sigmoid_sum_masked.fp", 38, 5, 0);
	E = generateLayer(0, 5, 0, 0);

	setOutput(A, B);
	setInput(C, A);
	setOutput(B, D);
	setOutput(C, D);
	setOutput(D, E);

	/*dummy values.*/
	/*fill vectors with values.*/

	fillWeights(A);
	copyWeightsToTexture(weight_matrix, A);
	copyMaskToTexture(mask_matrix, A);
	free(weight_matrix);
	free(mask_matrix);

	fillWeights(B);
	copyWeightsToTexture(weight_matrix, B);
	copyMaskToTexture(mask_matrix, B);
	free(weight_matrix);
	free(mask_matrix);

	fillWeights(C);
	copyWeightsToTexture(weight_matrix, C);
	copyMaskToTexture(mask_matrix, C);
	free(weight_matrix);
	free(mask_matrix);

	fillWeights(D);
	copyWeightsToTexture(weight_matrix, D);
	copyMaskToTexture(mask_matrix, D);
	free(mask_matrix);
	free(weight_matrix);

	/*Execute the network N times.*/
	while (n-->0){
		fillVector(A);
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
		run(D);
		printLayer(E);
		/*glFinish(); //finish all operations before stopping the clock*/
#ifdef WIN32
		QueryPerformanceCounter(&end);
		QueryPerformanceFrequency( &freq );
		printf("Time in s:%f\n", ((double)(end.QuadPart - start.QuadPart))/(double)freq.QuadPart);
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

	return 0;
}


/*Dummy function for filling weights and mask with dummy data, it preserves the texture sizes wich is multiples of 4.*/
void fillWeights(layer *target){
	weight_matrix = calloc(target->out_size * target->size, sizeof(float));
	mask_matrix = calloc(target->out_size * target->size, sizeof(float));

	for(i=0; i<target->out_size; i++){
		for(j=0; j<target->size; j++){
			weight_matrix[j+i*target->size] = RAND_UNI;
			/*weight_matrix[j+i*target->size] = j+i*target->size;*/
			/*weight_matrix[j+i*target->size] = 1.0f;*/
			mask_matrix[j+i*target->size] = 1;
		}
	}
}

/*dummy vector data*/
void fillVector(layer *target){
	input = malloc(sizeof(float)*target->size);
	for(i=0; i<target->size; i++){
		input[i] = RAND_UNI;
		/*input[i] = i;*/
		/*input[i] = 1;*/
	}
}
