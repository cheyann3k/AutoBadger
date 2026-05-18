/**
 * @file pwm-demo.c
 * @author Juan Carlos Dorrejo Paulino (dorrejopauli@wisc.edu)
 * @brief Contains the cli handler for the PWM command
 * @version 0.1
 * @date 2024-04-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "../drivers/cli.h"
#include "../drivers/pwm.h"

#include "pwm-demo.h"

#define PWM_SUBCOMMANDS_LEN (sizeof(PWM_SUBCOMMANDS) / sizeof(cli_command_t))

cli_command_t PWM_SUBCOMMANDS[] = {
    [0] = {
        .name = "stop",
        .handler = cli_pwm_stop_handler,
        .init = NULL,
        .description = "Stops the PWM",
        .summary = "Stop the PWM",
        .flags = NULL,
        .flags_len = 0,
        .enabled = true
    }
};


cli_flag_t PWM_FLAGS[] = {
    [0] = {
        .long_flag = "--frequency",
        .short_flag = "-f",
        .summary = "Changes the frequency of the PWM",
        .type = type_int,
        .value = NULL,
        .value_len = NULL,
        .enabled = true
    },
    [1] = {
        .long_flag = "--duty_cycle",
        .short_flag = "-d",
        .summary = "Sets the duty cycle of the pwm. Negative for backwards rotation, Positive for forward",
        .type = type_float,
        .value = NULL,
        .value_len = NULL,
        .enabled = true 
    },
    [2] = {
        .long_flag = "--connector",
        .short_flag = "-c",
        .summary = "Sets the connector for the PWM",
        .type = type_int,
        .value = NULL,
        .value_len = NULL,
        .enabled = true
    }
};


static pwm_obj_t pwm; // PWM object
static int connector = 0; // Default connector is J300

void cli_pwm_handler(cli_command_t* parent, char** argv, int argc) {
    // Check if the help subcommand was called
    if(cli_help_handler(parent, argv, argc)) return;

    if (cli_command_handler(parent, PWM_SUBCOMMANDS, PWM_SUBCOMMANDS_LEN, argv, argc)) return;

    static bool initialized = false;
    
    // Check if no arguments were provided
    if(argc == 0) {
        printf("Error: No arguments provided.\n");
        return;
    }

    // Parse the flags
    cli_flags_parser(parent, argv, argc);


    //----------------Setting connector-----------------
    int newConnector;

    // Update the connector if a new one is provided
    if (PWM_FLAGS[2].value != NULL){
        newConnector = *(int *)PWM_FLAGS[2].value;

        if (newConnector < 0 || newConnector > J30_LEN - 1) {
            printf("Error: Invalid connector. Must be between 0 and %i inclusive\n", J30_LEN - 1);
            return;
        }
        
        if (connector != newConnector) {
            connector = newConnector;
            if (initialized) stop_pwm_connector(pwm); // Stop previous PWM
            initialized = false; // Reset initialization flag
        }
    }
    

    // Check if new initialization is needed
    if (!initialized) {
        pwm = init_pwm_connector(J30[connector]);
        initialized = true;
    }

    // ----------------Setting frequency and duty cycle-----------------
    static uint32_t frequency = 0;
    static float dutyCycle = 0.0f;
    bool update = false;

    // Check if there are any changes to the flags
    if (PWM_FLAGS[0].value != NULL) {
        uint32_t newFreq = *(uint32_t*)PWM_FLAGS[0].value;
        frequency = frequency != newFreq ? newFreq : frequency;
        update = true;
    }

    if (PWM_FLAGS[1].value != NULL) {
        float newDutyCycle = *(float*)PWM_FLAGS[1].value;
        dutyCycle = dutyCycle != newDutyCycle ? newDutyCycle : dutyCycle;
        update = true;
    }

    // -----------------Start PWM with the new settings----------------- 
    if (update) {
        edit_pwm_connector(pwm, dutyCycle, frequency);
    }
}


void cli_pwm_stop_handler(cli_command_t* parent, char** argv, int argc) {
    stop_pwm_connector(pwm);
}