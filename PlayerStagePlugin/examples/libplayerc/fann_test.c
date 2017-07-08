#include <stdio.h>

#include <libplayerc/playerc.h>
#include <fann.h>

#include "../plugins/fanndriver/fann_driver_commands.h"
void fann_callback(void *data);

int callback_activated = 0;


int
main(int argc, const char **argv)
{
  int i;
  playerc_client_t *client;
  playerc_position2d_t *position2d;
  playerc_opaque_t *opaque;
  float position[3];         // our input array
  
  // fann command structures
  struct fann_command_load_network *flnc;
  struct fann_command_som_config *fsc; 
  struct fann_command_gng_config *fgc; 
  struct fann_command_set_training_max *ftmc; 
  struct fann_command_train *fct;
  struct fann_command_test *fctest;
  struct fann_result *fr;            

  int sentdata = 0;

  player_opaque_data_t datapacket;  // message for opaque server
  player_opaque_data_t rp;

 

  // Create a client and connect it to the server.
  client = playerc_client_create(NULL, "localhost", 6665);
  if (0 != playerc_client_connect(client))
    return -1;

  // Create and subscribe to a position2d device.
  position2d = playerc_position2d_create(client, 0);
  if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE))
    return -1;

  // create the opaque client for fann
  opaque = playerc_opaque_create(client, 0);
  if (playerc_opaque_subscribe(opaque, PLAYER_OPEN_MODE))
    return -1;
 
  // add a callback to handle replies from the som
  playerc_client_addcallback(client, &opaque->info, fann_callback, NULL);
  callback_activated = 0;



  // initialize our input array to -1
  position[0] = -1;
  position[1] = -1;
  position[2] = -1;

  // setup/marshell the som configuration data
  //  datapacket.info = opaque->info;
  datapacket.data_count = sizeof(fann_command_som_config);
  fsc = (struct fann_command_som_config *)datapacket.data;
  fsc->command = FANND_CONFIGURE_SOM;
  fsc->somparam.som_num_inputs = 3;
  fsc->somparam.som_width = 10;
  fsc->somparam.som_height = 10;
  fsc->somparam.som_radius = 10;
  fsc->somparam.som_current_radius = 10;
  fsc->somparam.som_learning_rate = 0.01;
  fsc->somparam.som_learning_rate_constant = 0.01;
  fsc->somparam.som_topology = FANN_SOM_TOPOLOGY_HEXAGONAL;
  fsc->somparam.som_neighborhood = FANN_SOM_NEIGHBORHOOD_GAUSSIAN;
  fsc->somparam.som_learning_decay = FANN_SOM_LEARNING_DECAY_LINEAR;

  // send the command 
  playerc_client_write(opaque->info.client, &opaque->info, PLAYER_OPAQUE_CMD, &datapacket, NULL);

  // setup the max training command
  datapacket.data_count = sizeof(fann_command_som_config); 
  ftmc = (struct fann_command_set_training_max *)datapacket.data;
  ftmc->command = FANND_SET_SOM_TRAINING_MAX;
  ftmc->max = 1000;
  
  // send the command
  playerc_client_write(opaque->info.client, &opaque->info, PLAYER_OPAQUE_CMD, &datapacket, NULL);


  // Make the robot spin!
  if (0 != playerc_position2d_set_cmd_vel(position2d, 0, 0, DTOR(40.0), 1))
    return -1;

 
  for (;;)
  {
    


    // Wait for new data from server
    playerc_client_read(client);

    if (callback_activated == 1) {
      struct fann_result *fr = (struct fann_result *)opaque->data;
      
      printf("Received output size: %d,  Error: %f\n", fr->datasize, fr->error);
      printf("Output: ");
      for (i = 0; i < fr->datasize; i++) {
	printf("%f ", fr->data[i]);
      }
      printf("\n\n");
      callback_activated = 0;
    }
    
    // set position values for fann input
    position[0] = position2d->px;
    position[1] = position2d->py;
    position[2] = position2d->pa;

    // send the current position to the som
    datapacket.data_count = sizeof(int) + sizeof(float) * 3;
    fct = (struct fann_command_train *)datapacket.data;
    fct->command =  FANND_TRAIN_INPUT;
    fct->input[0] = position[0];
    fct->input[1] = position[1];
    fct->input[2] = position[2];
    playerc_client_write(opaque->info.client, &opaque->info, PLAYER_OPAQUE_CMD, &datapacket, NULL);

  }

  // Shutdown
  playerc_position2d_unsubscribe(position2d);
  playerc_position2d_destroy(position2d);
  playerc_opaque_unsubscribe(opaque);
  playerc_opaque_destroy(opaque);
  playerc_client_disconnect(client);
  playerc_client_destroy(client);

  return 0;
}

void fann_callback(void *data) {
  int i;

  callback_activated = 1;


  
}

