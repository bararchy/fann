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

#include <stdlib.h>
#include "fann.h"
#include <string.h>

#define NUM_INPUT 1000

int main(int argc, char *argv[])
{	
	const float desired_error = (const float) 0.001;
	const unsigned int max_epochs = 500000;
	const unsigned int epochs_between_reports = 1000;
	const unsigned int num_input = 2;
	int i;

	struct fann *ann = (struct fann *) fann_create_gng( num_input );

        struct fann_train_data *data = (struct fann_train_data *) fann_read_train_from_file("gng_train.data");

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-init") == 0)
                        fann_init_weights_gng(ann, data);
                else if (strcmp(argv[i], "-epoch") == 0) {
                        fann_train_epoch(ann, data);
                        return EXIT_SUCCESS;
                }
        }


	fann_train_on_file(ann, "gng_train.data", max_epochs, epochs_between_reports, desired_error);

	/*fann_save(ann, "som_train.net"); */

	fann_destroy(ann);

	return 0;
}
