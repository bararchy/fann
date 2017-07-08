#ifndef FANN_DRIVER_COMMANDS_H
#define FANN_DRIVER_COMMANDS_H

#include <fann_data.h>

// The commands that can be sent to the fann driver
#define FANND_SAVE_NETWORK              1
#define FANND_TRAIN_INPUT               2
#define FANND_RUN_INPUT                 3


#define MAX_FANN_INPUTARRAY_SIZE             1000

struct fann_com {
  int command;
  void *data;
} fann_com;

struct fann_command_save_network {
  int command;
} fann_command_save_network;

struct fann_command_train {
  int command;
  float input[MAX_FANN_INPUTARRAY_SIZE];
  float output[MAX_FANN_INPUTARRAY_SIZE];
} fann_command_train;

struct fann_command_run {
  int command;
  float input[MAX_FANN_INPUTARRAY_SIZE];
} fann_command_run;

struct fann_result {
  int datasize;
  float error;
  float data[MAX_FANN_INPUTARRAY_SIZE];
} fann_result;

#endif

