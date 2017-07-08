Recurrent Neural Networks.

Fully Connected Shortcut Network MIMO Architecture.

Shortcut networks are layered and feedforward. However, instead of each layer's output only serving as input to the next layer, output from any given layer is input to all layers in front of it.

Using MIMO, this is implemented as follows. A single contiguous block of memory for storing *all* outputs is initially allocated. Then, this is partitioned between layers and neurons. The key is that the lowest memory region contains the inputs, and higher memory regions contain larger-numbered layers. Hence, by storing the number of inputs per layer and a pointer to the beginning of this block, no memory is wasted by duplicating outputs, and all layers can share the same set.


Fully Recurrent Network MIMO Architecture.

Fully recurrent networks are implemented as a single layer containing 'n' MIMO neurons each with one output, corresponding to the 'n' recurrent neurons of the network. The network also contains 'm+1' input lines (or input nodes), of which 'm' are fixed to input values and one bias line is fixed to the value '1'. Each input line connects with each neuron unidirectionally. Each recurrent neuron connects to each other recurrent neuron bidirectionally. The last 'o' nodes of the recurrent neurons are designated as output nodes.

It is important to distinguish how these values are stored in the MIMO data structure:
ann->num_inputs:    The number of inputs to the network, not including the bias.
ann->num_outputs:   The number of outputs from the network (the last neurons in the array).
layer->num_inputs:  The number of inputs to the layer (i.e. ann->num_inputs + 1 for the bias).
layer->num_outputs: The number of outputs from the layer (i.e. the 'n' recurrent neurons).

It is also important to distinguish how the weights array W (accessed as w_{i,j}) is stored:
+ Rank 'i' indicates recurrent neuron. (Hence, there are 'layer->num_outputs' ranks.) Each rank is stored separately in a single neuron data structure.
+ File 'j' indicates neuron which connects to the recurrent neuron. The first 'ann->num_inputs' are from the input lines. The next single file is the bias neuron. The remaining 'layer->num_outputs' are the recurrent neurons. The last 'ann->num_outputs' neurons are the outputs from the network (but are part of the recurrent neuron set).

The following files implement fully recurrent networks in FANN:

fann_recurrent.c
fann_base_fully_recurrent.c



Autoassociative Networks.

Autoassociative networks are fully recurrent. However, instead of there existing 'm' input lines, the 'n' nodes are directly initialized with values at time 0. Hence, autoassociative networks can be created using the recurrent network architecture by creating it with zero inputs and 'n' outputs. Then, before running the network, each node must be filled with a pattern. Autoassociative networks are often used for recalling patterns from partial data.

The Hopfield network is unique in that it can use a "one-shot learning" technique where network weights are learned in one pass given a set of input patterns to be recalled. It has been proven that N neurons can recall approximately 0.14N patterns with high precision. However, although the network will converge, fixed points may exist which are not patterns. For example, the inverse of patterns are often fixed points.



Testing Methodology.

Very few implementations of recurrent neural network algorithms are available on-line. Most testing has duplicated examples in research reports, comparing MSE graphs and iteration counts.





New functions:

// MIMO implementation of fully recurrent network
fann_create_fully_recurrent(num_neurons, num_inputs, num_outputs);

// MIMO implementation of shortcut network
fann_create_shortcut(num_layers, ...);


/**************************************************
 HOPFIELD NETWORK

 Fully recurrent N neuron network which is
 used for content-addressable memories. Uses
 one-shot learning for input patterns of length
 N. Then, given an input vector of length N,
 it will recall one of the initial patterns
 (or other fixed points such as their inverses).
 *************************************************/

// Run the network until a stable state occurs
FANN_EXTERNAL fann_type *FANN_API fann_run_hopfield(struct fann *ann, fann_type *input);

// Train the net on a sequence of patterns
FANN_EXTERNAL void FANN_API fann_train_hopfield(struct fann *ann, struct fann_train_data *pattern);

// Create a Hopfield network
FANN_EXTERNAL struct fann *FANN_API fann_create_hopfield(unsigned int num_neurons);


/* Creates a feedforward, layered net which is an "unrolled" recurrent network.
 For example, the recurrent net:
  A <-> B <-> C<- (where C autosynapses)
 Becomes (unrolled two time steps):
  A  B  C    input layer
   \/ \/|
  A  B  C    hidden layer I
   \/ \/|
  A  B  C    output layer
*/
struct fann *fann_create_unrolled_recurrent(
	unsigned int num_neurons, fann_type *weights, unsigned int time_steps);


/**************************************************
 REAL-TIME RECURRENT LEARNING

 Williams and Zipser, "A Learning Algorithm for
   Continually Running Fully Recurrent Neural
   Networks," Neural Computation, 1. (1989)

 NOTE: This function is still being debugged.
       MSE does not decrease properly.
 *************************************************/
FANN_EXTERNAL void FANN_API fann_train_rtrl(struct fann *ann, struct fann_train_data *pattern, 
						   float max_MSE, unsigned int max_iters, float rate);





