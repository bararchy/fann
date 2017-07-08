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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>

FANN_EXTERNAL struct fann *FANN_API MAKE_NAME(create_standard)(unsigned int num_layers, ...)
{
	struct fann *ann;
	va_list layer_sizes;
	int i;
	unsigned int *layers = (unsigned int *) calloc(num_layers, sizeof(unsigned int));

	if(layers == NULL)
	{
		fann_error(NULL, FANN_E_CANT_ALLOCATE_MEM);
		return NULL;
	}

	va_start(layer_sizes, num_layers);
	for(i = 0; i < (int) num_layers; i++)
	{
		layers[i] = va_arg(layer_sizes, unsigned int);
	}
	va_end(layer_sizes);

	ann = MAKE_NAME(create_standard_array)(num_layers, layers);

	free(layers);

	return ann;
}

FANN_EXTERNAL struct fann *FANN_API MAKE_NAME(create_standard_array)(unsigned int num_layers, 
															   const unsigned int *layers)
{
	struct fann *ann;
	char *type = vals(implementation) "_" vals(algorithm) "_" vals(activation_function_name);
	ann=fann_create_standard_array_typed("connected_any_any", type, num_layers, layers);

	return ann;
}

/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap
 */
