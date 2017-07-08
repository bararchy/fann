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
#ifndef _fann_optimized_template_h_
#define _fann_optimized_template_h_

#ifdef FIXEDFANN
#define pow2(value,ann)\
			(value / (float) ann->multiplier) * (value / (float) ann->multiplier);
#else
#define pow2(value,ann)\
		(float) ((value)*(value));
#endif

/******************* ACTIVATION FUNCTIONS STUFF **************/

/*SIGMOID*/
#define fann_activation_sigmoid(neuron,out) \
	neuron->outputs[out] = (fann_type)fann_sigmoid_real(neuron->sums[out]); 
#define fann_activation_derived_sigmoid(neuron, out, res)\
{\
	fann_type value=neuron->outputs[out];\
	value = fann_clip(value, 0.01f, 0.99f);\
	res = fann_sigmoid_derive(neuron->activation_steepness, value);\
}
/*
#define fann_update_MSE_sigmoid(ann, neuron_diff) \
*/
static __inline__ void fann_update_MSE_sigmoid(struct fann * ann, struct fann_neuron* neuron, fann_type * neuron_diff) 
{
	fann_type neuron_diff2 = pow2(*neuron_diff,ann)
	ann->training_params->MSE_value += neuron_diff2;
	if(fann_abs(*neuron_diff) >= ann->training_params->bit_fail_limit)
		ann->training_params->num_bit_fail++;
}

/*SIGMOID SIMMETRIC*/
#define fann_activation_sigmoid_symmetric(neuron,out) \
	neuron->outputs[out] = (fann_type)fann_sigmoid_symmetric_real(neuron->sums[out]); 
#define fann_activation_derived_sigmoid_symmetric(neuron, out, res)\
{\
	fann_type value=neuron->outputs[out];\
	value = fann_clip(value, -0.98f, 0.98f);\
	res = fann_sigmoid_symmetric_derive(neuron->activation_steepness, value);\
}

/*
#define fann_update_MSE_sigmoid_symmetric( ann, neuron_diff)\
*/
static __inline__ void fann_update_MSE_sigmoid_symmetric(struct fann * ann, struct fann_neuron* neuron, fann_type * neuron_diff)
{
	fann_type neuron_diff2;
	*neuron_diff/=(fann_type)2.0;
	neuron_diff2 = pow2(*neuron_diff,ann)
	ann->training_params->MSE_value += neuron_diff2;
	if(fann_abs(*neuron_diff) >= ann->training_params->bit_fail_limit)
		ann->training_params->num_bit_fail++;
}

#endif

/*
 * vim: ts=2 smarttab smartindent shiftwidth=2 nowrap noet
 */

