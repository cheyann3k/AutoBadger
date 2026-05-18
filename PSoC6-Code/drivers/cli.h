#ifndef __CLI_H__
#define __CLI_H__

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "console.h"
#include "command_types.h"
#include "ble.h"

// Set the frequency of timer to 10000 Hz (0.1 ms period - seems to be max as 1 ms caused an error)
#define CONT_TIMER_FREQ 10000
#define CONT_TIMER_MS_DIV (CONT_TIMER_FREQ / 1000)

// Set the minimum delay for continuous commands to 25 ms
#define CONT_MIN_DELAY 25

bool cli_init(cli_command_t* COMMANDS, int COMMANDS_LEN, bool use_ble);
void cli_run(cli_command_t* COMMANDS, int COMMANDS_LEN);

bool cli_command_handler(cli_command_t* parent,  cli_command_t* commands, int commands_len, 
                         char** argv, int argc);
bool cli_help_handler(cli_command_t* command, char** argv, int argc);
void* cli_args_parser(char* arg, int* value_len, cli_arg_type_t type);
void cli_help(cli_command_t* command, cli_command_t* subcommands, int subcommands_len);
char** cli_arg_splitter(char* args, int* split_len, bool for_list);
void cli_flags_parser(cli_command_t* command, char** argv, int argc);

bool check_for_input();
char* get_input();
void clear_input();

void cli_continuous_handler(cli_command_t* parent, char** argv, int argc);
void cli_stop_handler(cli_command_t* parent, char** argv, int argc);
void stop_continuous();

bool is_int(char* arg);
bool is_float(char* arg);
void free_string_list(char** list, int len);


#endif // __CLI_H__