#include <stdio.h>

#include <libplayerc/playerc.h>
#include <fann.h>

#include "../plugins/fanndriver/fann_driver_commands.h"
void fann_callback(void *data);

int callback_activated = 0;


#define FANN_TRAIN   1  // 1 - train the ANN, 0 - Use the ANN for control


int
main(int argc, const char **argv) {
  playerc_client_t *client;
  playerc_position2d_t *position2d;
  playerc_laser_t *laser;
  playerc_opaque_t *opaque;
  
  // fann command structures
  //struct fann_command_save_network *fcs;
  struct fann_command_train *fctrain;
  struct fann_command_run *fcrun;

  player_opaque_data_t datapacket;  // message for opaque server

  // Create a client and connect it to the server.
  client = playerc_client_create(NULL, "localhost", 6665);
  if (0 != playerc_client_connect(client))
    return -1;

  // Set the data mode to PULL, i.e. replace new messages of a type already
  // received, so that the queue does not become full
  if (playerc_client_set_replace_rule (client, -1, -1, PLAYER_MSGTYPE_DATA, -1, 1) != 0)
  {
    fprintf(stderr, "error: %s\n", playerc_error_str());
    return -1;
  }

  // Create and subscribe to the position2d device.
  position2d = playerc_position2d_create(client, 0);
  if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE))
    return -1;
  
  // Create and subscribe to the laser device.
  laser = playerc_laser_create(client, 0);
  if (playerc_laser_subscribe(laser, PLAYER_OPEN_MODE))
    return -1;
  
  // create the opaque client for fann
  opaque = playerc_opaque_create(client, 0);
  if (playerc_opaque_subscribe(opaque, PLAYER_OPEN_MODE))
    return -1;
  
  
  
  // add a callback to handle replies from the ANN if we are in testing mode
  if (!FANN_TRAIN) {
    playerc_client_addcallback(client, &opaque->info, fann_callback, NULL);
    callback_activated = 0;
  }
  
  
  for (;;) {
    double newspeed = 0;
    double newturnrate = 0;
    
    // Wait for new data from server
    playerc_client_read(client);
    
    
    //  For training, the robot will learn to avoid obstacles detected by the laser
    //  A sample of the laser range readings serve as the input 
    //  A forward velocity and turn rate will serve as the output
    // laser obstacle avoidance from laserobstacleavoid.cc
    if (FANN_TRAIN) {
	
      double minR = 1e9;
      double minL = 1e9;
      uint count = 360;
      int j, i;

      // calculate obstacle avoidance using the laser ranges
      for (j = 0; j < count/2; j = j + 20) {
	if (minR > laser->ranges[j]) {
	  minR = laser->ranges[j];
	}
      }
      for (j = count/2; j < count; j = j + 20) {
	if (minL > laser->ranges[j]) {
	  minL = laser->ranges[j];
	}
      }
	
      double l = (1e5 * minR) / 500 - 100;
      double r = (1e5 * minL) / 500 - 100;
	
      if (l > 100) {
	l = 100;
      }
      if (r > 100) {
	r = 100;
      }
      newspeed = (r + l) / 1e3;
	
      // calculate the turning rate
      newturnrate = (r-l);
      if (newturnrate < -40.0) {
	newturnrate = -40.0;
      }
      if (newturnrate > 40.0) {
	newturnrate = 40.0;
      }
      newturnrate = newturnrate * M_PI / 180;
	
      // send commands to robot
      playerc_position2d_set_cmd_car(position2d, newspeed, newturnrate);
	
      // load up the input/ouput for FANN and send it to the driver
      datapacket.data_count = sizeof(int) + sizeof(fann_command_train);
      fctrain = (struct fann_command_train *)datapacket.data;
      for (i = 0; i < 18; i++) {
	fctrain->input[i] = laser->ranges[i * 20];
      }
      fctrain->output[0] = newspeed;
      fctrain->output[1] = newturnrate;
	
      fctrain->command =  FANND_TRAIN_INPUT;
      playerc_client_write(opaque->info.client, &opaque->info, PLAYER_OPAQUE_CMD, &datapacket, NULL);
    }
    

      
    // if we are using the trained ANN as part of the control loop, we send the ANN samples of
    // the laser readings and send the ANN's output to the robot
    if (!FANN_TRAIN) {
      int i;

      // collect the message from fann if we have one
      if (callback_activated == 1) {
	struct fann_result *fr = (struct fann_result *)opaque->data;
	newspeed = fr->data[0];
	newturnrate = fr->data[1];

	// send the command to the robot
	playerc_position2d_set_cmd_car(position2d, newspeed, newturnrate);
	callback_activated = 0;
      }
	
      // load up the input/ouput for FANN to find out what to do
      datapacket.data_count = sizeof(int) + sizeof(fann_command_run);
      fcrun = (struct fann_command_run *)datapacket.data;
      for (i = 0; i < 18; i++) {
	fcrun->input[i] = laser->ranges[i * 20];
      }
      fcrun->command =  FANND_RUN_INPUT;
      playerc_client_write(opaque->info.client, &opaque->info, PLAYER_OPAQUE_CMD, &datapacket, NULL);
    }
  }
  
    
  // Shutdown
  playerc_position2d_unsubscribe(position2d);
  playerc_position2d_destroy(position2d);
  playerc_laser_unsubscribe(laser);
  playerc_laser_destroy(laser);
  playerc_opaque_unsubscribe(opaque);
  playerc_opaque_destroy(opaque);
  playerc_client_disconnect(client);
  playerc_client_destroy(client);
    
  return 0;
}

void fann_callback(void *data) {

  callback_activated = 1;
}

