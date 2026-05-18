#ifndef __PWM_DEMO_H_
#define __IMU_DEMO_H_

#include "../drivers/command_types.h"

#define PWM_FLAGS_LEN 3

/**
 * @brief Subcommands for the PWM command.
 */
extern cli_command_t PWM_SUBCOMMANDS[];

/**
 * @brief Flags for the PWM command.
 */
extern cli_flag_t PWM_FLAGS[];

/**
 * @brief Handler for the 'pwm' command.
 * 
 * @param parent The command's Struct.
 * @param argv The arguments for the command.
 * @param argc The number of arguments.
 */
void cli_pwm_handler(cli_command_t* parent, char** argv, int argc);

/**
 * @brief Handler for the 'pwm stop' subcommand.
 * 
 * @param parent The parent command's Struct.
 * @param argv The arguments for the command. 
 * @param argc The number of arguments.
 */
void cli_pwm_stop_handler(cli_command_t* parent, char** argv, int argc);

#endif // __IMU_DEMO_H_