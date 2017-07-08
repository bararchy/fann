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

#version 110
#define i_size 4.0
#define o_size 8.0
#define offset 8.0

//start HERE ¤

//o_size is the number of output neurons
//i_size is the number of (input neurons / 4), rounded up
//offset is the number that the program should decrease the coords with to get coords starting from 0.

//dynamic inputs to the program.
uniform sampler2D input_vector; // inputs
uniform sampler2D weights; //weight matrix
uniform sampler2D mask; //mask connections - not implemented yet


void main(void){
//compiler computations
  float i_delta = 1.0/i_size;
  float o_delta = 1.0/o_size;

//get the texture coordinates
  float col = gl_FragCoord.x-0.5-offset;
  float row = col*4.0*o_delta;
//initialize the sum vector
  vec4 sum = vec4(0.0,0.0,0.0,0.0);
  vec2 input_tuple;
  vec4 weight_tuple;
  vec4 weight_tuple2;
  vec4 input;
//iterate over the input vector, texel by texel
  for (float i=i_delta/2.0; i<1.0; i+=i_delta){
    input_tuple = vec2(i, 0.0);
	//compute weight texture coordinates
	weight_tuple = vec4(i, row+0.5*o_delta, i, row+1.5*o_delta);
	weight_tuple2 = vec4(i, row+2.5*o_delta, i, row+3.5*o_delta);
	//get input value
	input = texture2D(input_vector, input_tuple);
	//compute the sum for all 4 elements
	sum.r += dot(input, texture2D(weights, weight_tuple.xy));
	sum.g += dot(input, texture2D(weights, weight_tuple.zw));
	sum.b += dot(input, texture2D(weights, weight_tuple2.xy));
	sum.a += dot(input, texture2D(weights, weight_tuple2.zw));
  } 
  //return the sum
  gl_FragColor = sum;
}


