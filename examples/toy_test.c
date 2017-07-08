#include "fann.h"

#define I_DIM 200
#define O_DIM 1
#define S_DIM 100

void f(fann_type *in, fann_type *out, unsigned int pos, unsigned int idim, unsigned int odim)
{
	int i,j;
	for (i=0; i< idim; i++)
	{
		in[i]+= (0.5 * pos)/S_DIM+(fann_type) + (0.5*i) / idim;
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

int main()
{
	float mse=1000;
	unsigned int num_train=100000000;
	fann_type **input = (fann_type **) malloc(S_DIM*sizeof(fann_type*));
	fann_type **output= (fann_type **) malloc(S_DIM*sizeof(fann_type*));
	unsigned int i, j;

	for ( i=0; i< S_DIM; i++)
	{
		input[i]  = (fann_type*) malloc(I_DIM*sizeof(fann_type));
		output[i] = (fann_type*) malloc(O_DIM*sizeof(fann_type));
		f(input[i], output[i], i, I_DIM, O_DIM); 
	}

#ifndef SPARSE
	struct fann *ann = fann_create_standard( 3, I_DIM, 20, O_DIM);
#else
	struct fann *ann = fann_create_sparse(0.3, 3, I_DIM, 20, O_DIM);
#endif
	
	for (j=0; mse>0.00001 && j!=num_train; j++)
	{
		fann_reset_MSE(ann);
		for ( i=0; i< S_DIM; i++)
		{
			fann_train( ann, input[i], output[i] );
			/* printf ("MSE Error :[%4u:%4u] \t%e\n", j, i, mse); */
		}

		mse=fann_get_MSE(ann);
		printf ("[ %7u ] MSE Error : %e\n", j, mse);
	}

	return 0;
}
