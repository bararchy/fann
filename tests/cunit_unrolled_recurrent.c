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

//#include <CUnit/CUnit.h>
#include <stdio.h>
#include "floatfann.h"
#include "cunit_fann.h"

/* The recurrent net:
  A <-> B <-> C<- (where C autosynapses)
 Will become unrolled two time steps:
  A  B  C    input layer
   \/ \/|
  A  B  C    hidden layer I
   \/ \/|
  A  B  C    output layer
*/

void cunit_unrolled_recurrent(void)
{
	fann_type *calc_out;
	fann_type input[3];

	/* Specify the following recurrent net:
	   A <-> B <-> C<- (where C autosynapses)*/
	unsigned int num_neurons = 3;
	unsigned int num_time_steps = 2;
	fann_type recurrent_weights[3][3] = 
		{{0 ,  1,  0},
		 {-1,  0,  1},
		 {0 ,  1,  1}};

	struct fann *ann = fann_create_unrolled_recurrent(
		num_neurons, recurrent_weights, num_time_steps);

	CU_ASSERT_PTR_NOT_NULL_FATAL(ann);

	input[0] = 1;
	input[1] = 1;
	input[2] = 1;
	calc_out = fann_run(ann, input);
	CU_ASSERT(IS_NEG(calc_out[0]));
	CU_ASSERT(IS_POS(calc_out[1]));
	CU_ASSERT(IS_POS(calc_out[2]));

	fann_destroy(ann);
}
