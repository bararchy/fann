/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2003
 *     Brian Gerkey
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*
 * A simple example of how to write a driver that will be built as a
 * shared object.
 */

// ONLY if you need something that was #define'd as a result of configure
// (e.g., HAVE_CFMAKERAW), then #include <config.h>, like so:
/*
#if HAVE_CONFIG_H
  #include <config.h>
#endif
*/

#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fann.h>

#include <libplayercore/playercore.h>
#include "fann_driver_commands.h"

#include "sharedstruct.h"

////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class FannDriver : public Driver
{
  public:



    // Constructor; need that
    FannDriver(ConfigFile* cf, int section);

    // Must implement the following methods.
    virtual int Setup();
    virtual int Shutdown();

    // This method will be invoked on each incoming message
    virtual int ProcessMessage(MessageQueue* resp_queue,
                               player_msghdr * hdr,
                               void * data);

  private:
    player_opaque_data_t *DataPacket;
    fann *Fann_Network;
    float *Fann_Output;
    float *Fann_Input;
    unsigned int Fann_CurTrainNum;
    unsigned int Fann_MaxTrainNum;
    struct fann_som_params Fann_Somparam;
    struct fann_gng_params Fann_Gngparam;
    const char *Fann_Nettype;
    const char *Fann_Netname;
  
    
    void Configure_SOM(fann_som_params *somconfig);    
    void Configure_GNG(fann_gng_params *gngconfig);
    void Configure_ANN(int numin, int numout, int numhidden);   
    void Fann_Train(struct fann_command_train *ftc, MessageQueue* resp_queue);
    void Fann_Run(struct fann_command_run *ftc, MessageQueue* resp_queue);
  

    // Main function for device thread.
    virtual void Main();

    // Update the data
    virtual void RefreshData();

    // This is the data we store and send
    player_opaque_data_t Data;
};

// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver*
FannDriver_Init(ConfigFile* cf, int section)
{
  // Create and return a new instance of this driver
  return((Driver*)(new FannDriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void FannDriver_Register(DriverTable* table)
{
  table->AddDriver("fanndriver", FannDriver_Init);
}

////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.
FannDriver::FannDriver(ConfigFile* cf, int section)
    : Driver(cf, section, false, PLAYER_MSGQUEUE_DEFAULT_MAXLEN,
             PLAYER_OPAQUE_CODE)
{
  Fann_CurTrainNum = 0;
  Fann_MaxTrainNum = 0;
  Fann_Nettype = (char *)cf->ReadString(section, "nettype", "NULL");
  if (!strcmp(Fann_Nettype, "NULL")) {
    fprintf(stderr, "Error: FannDriver: No nettype specified\n");
    exit(1);
  }

  // make sure it is a valid nettype
  if ((strcmp(Fann_Nettype, "GNG")) && (strcmp(Fann_Nettype, "SOM")) && (strcmp(Fann_Nettype, "ANN"))) {
    fprintf(stderr, "Error: FannDriver: Unknown nettype: %s\n", Fann_Nettype);
    exit(1);
  }

  Fann_Netname = cf->ReadString(section, "netname", "NULL");


  // if loading is enabled then load the net
  int loading = cf->ReadInt(section, "loadnet", 0);
  if (loading) {
    fprintf(stderr, "Error: FannDriver: Net loading not implemented yet\n");
    exit(1);
  }
  else {
    
    // if we aren't loading and we want a ANN we have a problem
    if (!strcmp(Fann_Nettype, "ANN")) {
      fprintf(stderr, "Warning: FannDriver: ANN only supports default functionality when not loading net\n");
    }
  }
  
  // get the number of inputs
  int numin = cf->ReadInt(section, "numinput", 0);
  if (numin == 0) {
    fprintf(stderr, "Error: FannDriver: No numinput specified\n");
    exit(1);
  }
  if (!strcmp(Fann_Nettype, "SOM")) {
    Fann_Somparam.som_num_inputs = numin;
  }
  else if (!strcmp(Fann_Nettype, "GNG")) {
    Fann_Gngparam.gng_num_inputs = numin;
  }
  else {
    int numout = cf->ReadInt(section, "numoutput", 0);
    if (numout == 0) {
      fprintf(stderr, "Error: FannDriver: No numoutput specified\n");
      exit(1);
    }
    int numhidden = cf->ReadInt(section, "numhidden", 0);
    if (numhidden == 0) {
      fprintf(stderr, "Error: FannDriver: No numhidden specified\n");
      exit(1);
    }
    Configure_ANN(numin, numout, numhidden);
    return;
  }

  if (!strcmp(Fann_Nettype, "SOM")) {
    Fann_Somparam.som_width = cf->ReadInt(section, "som_width", 10);
    Fann_Somparam.som_height = cf->ReadInt(section, "som_height", 10);
    Fann_Somparam.som_radius = cf->ReadFloat(section, "som_radius", Fann_Somparam.som_radius);
    Fann_Somparam.som_current_radius = cf->ReadFloat(section, "som_currentradius",  Fann_Somparam.som_radius);
    Fann_Somparam.som_learning_rate = cf->ReadFloat(section, "som_learningrate", 0.01);
    Fann_Somparam.som_learning_rate_constant = cf->ReadFloat(section, "som_learningrateconstant", 0.01);
    const char *top = cf->ReadString(section, "som_topology", "hex");
    if (!strcmp(top, "hex")) {
      Fann_Somparam.som_topology = FANN_SOM_TOPOLOGY_HEXAGONAL;
    }
    else if (!strcmp(top, "rec")) {
      Fann_Somparam.som_topology = FANN_SOM_TOPOLOGY_RECTANGULAR;
    }
    else {
      fprintf(stderr, "Error: FannDriver: Invalid somtopology: %s   (Valid: 'rec', 'hex')\n", top);
      exit(1);
    }
    const char *neigh = cf->ReadString(section, "som_neighborhood", "gaussian");
    if (!strcmp(neigh, "gaussian")) {
      Fann_Somparam.som_neighborhood = FANN_SOM_NEIGHBORHOOD_GAUSSIAN;
    }
    else if (!strcmp(neigh, "distance")) {
      Fann_Somparam.som_neighborhood = FANN_SOM_NEIGHBORHOOD_DISTANCE;
    }
    else {
      fprintf(stderr, "Error: FannDriver: Invalid somneighborhood: %s   (Valid: 'gaussian', 'distance')\n", neigh);
      exit(1);
    }

    const char *decay = cf->ReadString(section, "som_decay", "linear");
    if (!strcmp(decay, "linear")) {
      Fann_Somparam.som_learning_decay = FANN_SOM_LEARNING_DECAY_LINEAR;
    }
    else if (!strcmp(decay, "inverse")) {
      Fann_Somparam.som_learning_decay = FANN_SOM_LEARNING_DECAY_INVERSE;
    }
    else {
      fprintf(stderr, "Error: FannDriver: Invalid somneighborhood: %s   (Valid: 'gaussian', 'distance')\n", neigh);
      exit(1);
    }
    Fann_MaxTrainNum = cf->ReadInt(section, "som_maxtraining", 1000);


    Configure_SOM(&Fann_Somparam);
  }

  // otherwise we configure the gng
  else {
    Fann_Gngparam.gng_max_nodes = cf->ReadInt(section, "gng_maxnodes", 100);
    Fann_Gngparam.gng_max_age = cf->ReadInt(section, "gng_maxage", 100);
    Fann_Gngparam.gng_iteration_of_node_insert = cf->ReadInt(section, "gng_nodeinsertiteration", 300);
    Fann_Gngparam.gng_local_error_reduction_factor = cf->ReadFloat(section, "gng_localerrorreductionfactor", 0.05);
    Fann_Gngparam.gng_global_error_reduction_factor = cf->ReadFloat(section, "gng_globalerrorreductionfactor", 0.0006);
    Fann_Gngparam.gng_winner_node_scaling_factor = cf->ReadFloat(section, "gng_winningnodescalingfactor", 0.5);
    Fann_Gngparam.gng_neighbor_node_scaling_factor = cf->ReadFloat(section, "gng_neighbornodescalingfactor", 0.0005);

    Configure_GNG(&Fann_Gngparam);
  }


  return;
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int FannDriver::Setup()
{
  puts("Fann driver initialising");

  // Here you do whatever is necessary to setup the device, like open and
  // configure a serial port.


  puts("Fann driver ready");

  // Start the device thread; spawns a new thread and executes
  // FannDriver::Main(), which contains the main loop for the driver.
  StartThread();

  return(0);
}


////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
int FannDriver::Shutdown()
{
  puts("Shutting Fann driver down");

  // Stop and join the driver thread
  StopThread();

  // Here you would shut the device down by, for example, closing a
  // serial port.

  puts("Fann driver has been shutdown");

  return(0);
}

// Process messages here.  Send a response if necessary, using Publish().
// If you handle the message successfully, return 0.  Otherwise,
// return -1, and a NACK will be sent for you, if a response is required
int FannDriver::ProcessMessage(MessageQueue* resp_queue,
                                 player_msghdr* hdr,
                                 void* data)
{

 
  DataPacket = (player_opaque_data_t *)data;
  struct fann_com *fc = (struct fann_com *)DataPacket->data;
  
  if (fc->command == FANND_SAVE_NETWORK) {
    printf("Network saving not implemented yet!\n");
  }
  else if (fc->command == FANND_TRAIN_INPUT) {
    struct fann_command_train *ftc = (struct fann_command_train *)DataPacket->data;
    Fann_Train(ftc, resp_queue);
  }
  else if (fc->command == FANND_RUN_INPUT) {
    struct fann_command_run *frc = (struct fann_command_run *)DataPacket->data;
    Fann_Run(frc, resp_queue);
  }
  else {
    printf("Fann Device Recieved Unknown Message: %d\n", fc->command);
  }
  
  /*
  uint size = sizeof(mData) - sizeof(mData.data) + mData.data_count;
  Publish(device_addr, NULL,
          PLAYER_MSGTYPE_DATA, PLAYER_OPAQUE_DATA_STATE,
          reinterpret_cast<void*>(&mData), size, NULL);
  */
  return(0);
}



////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void FannDriver::Main()
{
  // The main loop; interact with the device here
  for(;;)
  {

    // test if we are supposed to cancel
    pthread_testcancel();

    // Process incoming messages.  FannDriver::ProcessMessage() is
    // called on each message.
    ProcessMessages();

    // Interact with the device, and push out the resulting data, using
    RefreshData();

    // Sleep (you might, for example, block on a read() instead)
    usleep(100000);
  }
}

void FannDriver::RefreshData()
{

}


// Initializes a som based on the received parameters
void FannDriver::Configure_SOM(fann_som_params *somconfig) {

  printf("Fanndriver: Creating a: %d x %d %d SOM\n", somconfig->som_width, somconfig->som_height, somconfig->som_num_inputs);
  Fann_Network = fann_create_som(somconfig->som_width, somconfig->som_height, somconfig->som_num_inputs);
  fann_set_som_config(Fann_Network, somconfig);
  
}

// Initializes a gng based on the received paramters
void FannDriver::Configure_GNG(fann_gng_params *gngconfig) {

  printf("Fanndriver: Creating a GNG with %d inputs\n", gngconfig->gng_num_inputs);
  Fann_Network = fann_create_gng(gngconfig->gng_num_inputs);
  fann_set_gng_config(Fann_Network, gngconfig);
}

// Initializes default ANN
void FannDriver::Configure_ANN(int numin, int numout, int numhid) {

  printf("Fanndriver: Creating a ANN with %d inputs, %d outputs, %d hidden \n", numin, numout, numhid);
  Fann_Network = fann_create_standard(3, numin, numhid, numout);
  fann_set_activation_steepness_hidden(Fann_Network, 1.0);
  fann_set_activation_steepness_output(Fann_Network, 1.0);
  fann_set_activation_function_hidden(Fann_Network, FANN_SIGMOID_SYMMETRIC_STEPWISE);
  fann_set_activation_function_output(Fann_Network, FANN_SIGMOID_SYMMETRIC_STEPWISE);
}



void FannDriver::Fann_Train(struct fann_command_train *ftc, MessageQueue* resp_queue) {
  if (Fann_Network->network_type == FANN_NETTYPE_SOM) {
    fann_train_example_array_som(Fann_Network, ftc->input, Fann_CurTrainNum, Fann_MaxTrainNum);
    Fann_CurTrainNum++;

      // send back the results
    Data.data_count = sizeof(struct fann_result);
    struct fann_result *fr = (struct fann_result *)Data.data;
    struct fann_neuron *neuron;
    struct fann_som_neuron_private_data *priv;
    neuron = Fann_Network->first_layer->first_neuron;
    priv = (struct fann_som_neuron_private_data *) neuron->private_data;

    fr->datasize = Fann_Network->num_input;
    fr->error = sqrt(Fann_Network->som_params->min_dist);
    for (unsigned int i = 0; i < Fann_Network->num_input; i++) {
      fr->data[i] = priv->som_model_vector[Fann_Network->som_params->som_closest_index * Fann_Network->num_input + i];
    }

    int size = sizeof(Data) - sizeof(Data.data) + Data.data_count;
    Publish(device_addr, NULL, PLAYER_MSGTYPE_DATA, PLAYER_OPAQUE_DATA_STATE,
	    reinterpret_cast<void*>(&Data), size, NULL);

  }
  else if (Fann_Network->network_type == FANN_NETTYPE_GNG) {
    fann_train_example_array_gng(Fann_Network, ftc->input, 0, 0);

    Data.data_count = sizeof(struct fann_result);
    struct fann_result *fr = (struct fann_result *)Data.data;
    fr->datasize = Fann_Network->num_input;

    struct fann_neuron *neuron;
    struct fann_gng_neuron_private_data *priv;
    neuron = Fann_Network->first_layer->first_neuron;
    priv = (struct fann_gng_neuron_private_data *) neuron->private_data;
    fr->datasize = Fann_Network->num_input;
    fr->error = sqrt(priv->gng_cell_error[Fann_Network->gng_params->gng_closest_index]);
    for (unsigned int i = 0; i < Fann_Network->num_input; i++) {
      fr->data[i] = priv->gng_cell_location[Fann_Network->gng_params->gng_closest_index][i];
    }
    int size = sizeof(Data) - sizeof(Data.data) + Data.data_count;
    Publish(device_addr, NULL, PLAYER_MSGTYPE_DATA, PLAYER_OPAQUE_DATA_STATE,
	    reinterpret_cast<void*>(&Data), size, NULL);
  }
  else {
    fann_train(Fann_Network, ftc->input, ftc->output);
    Data.data_count = sizeof(struct fann_result);
    struct fann_result *fr = (struct fann_result *)Data.data;
    fr->datasize = Fann_Network->num_output;
    for (unsigned int i = 0; i < Fann_Network->num_output; i++) {
      fr->data[i] = Fann_Network->output[i];
    }

    int size = sizeof(Data) - sizeof(Data.data) + Data.data_count;
    Publish(device_addr, NULL, PLAYER_MSGTYPE_DATA, PLAYER_OPAQUE_DATA_STATE,
	    reinterpret_cast<void*>(&Data), size, NULL);
    
  }
  
}

// run an input
void FannDriver::Fann_Run(struct fann_command_run *frc, MessageQueue* resp_queue) {
    fann_run(Fann_Network, frc->input);

    // send back the results
    Data.data_count = sizeof(struct fann_result);
    struct fann_result *fr = (struct fann_result *)Data.data;

    fr->error = 0;
    // SOM
    if (Fann_Network->network_type == FANN_NETTYPE_SOM) {
      struct fann_result *fr = (struct fann_result *)Data.data;
      struct fann_neuron *neuron;
      struct fann_som_neuron_private_data *priv;
      neuron = Fann_Network->first_layer->first_neuron;
      priv = (struct fann_som_neuron_private_data *) neuron->private_data;
      
      for (unsigned int i = 0; i < Fann_Network->num_input; i++) {
	fr->data[i] = priv->som_model_vector[Fann_Network->som_params->som_closest_index * Fann_Network->num_input + i];
      }
      fr->datasize = Fann_Network->num_input;
    }
    // GNG
    else if (Fann_Network->network_type == FANN_NETTYPE_GNG) {
      struct fann_neuron *neuron;
      struct fann_gng_neuron_private_data *priv;
      neuron = Fann_Network->first_layer->first_neuron;
      priv = (struct fann_gng_neuron_private_data *) neuron->private_data;
      for (unsigned int i = 0; i < Fann_Network->num_input; i++) {
	fr->data[i] = priv->gng_cell_location[Fann_Network->gng_params->gng_closest_index][i];
      }
      fr->datasize = Fann_Network->num_input;
    }
    // ANN 
    else {
      fr->datasize = Fann_Network->num_output;
      for (unsigned int i = 0; i < Fann_Network->num_output; i++) {
	fr->data[i] = Fann_Network->output[i];
      }
    }

    int size = sizeof(Data) - sizeof(Data.data) + Data.data_count;
    Publish(device_addr, NULL, PLAYER_MSGTYPE_DATA, PLAYER_OPAQUE_DATA_STATE,
	    reinterpret_cast<void*>(&Data), size, NULL);
}



////////////////////////////////////////////////////////////////////////////////
// Extra stuff for building a shared object.

/* need the extern to avoid C++ name-mangling  */
extern "C" {
  int player_driver_init(DriverTable* table)
  {
    puts("Fann driver initializing");
    FannDriver_Register(table);
    puts("Fann driver done");
    return(0);
  }
}

