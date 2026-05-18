#include "commands.h"

cli_command_t COMMANDS[] = {
    [0] = {
        .name = "imu",
        .handler = cli_imu_handler,
        .init = imu_init,
        .description = "IMU commands",
        .summary = "IMU commands",
        .flags = NULL,
        .flags_len = 0,
        .enabled = false
    },
    [1] = {
        .name = "eeprom",
        .handler = cli_eeprom_handler,
        .init = eeprom_init,
        .description = "EEPROM commands",
        .summary = "EEPROM commands",
        .flags = NULL,
        .flags_len = 0,
        .enabled = false
    },
    [2] = {
        .name = "pwm",
        .handler = cli_pwm_handler,
        .init = NULL,
        .description = "Starts a PWM command. Format: pwm -c <connector> -dc <duty cycle> " \
                        "-f <frequency> <direction>",
        .summary = "PWM commands",
        .flags = PWM_FLAGS,
        .flags_len = PWM_FLAGS_LEN,
        .enabled = true
    },
    [3] = {
        .name = "cont",
        .handler = cli_continuous_handler,
        .init = NULL,
        .description = "Runs the command in a continuous loop. Format: cont <delay (in ms)> <command> <args>",
        .summary = "Run a command in a continuous loop",
        .flags = NULL,
        .flags_len = 0,
        .enabled = false
    },
    [4] = {
        .name = "stop",
        .handler = cli_stop_handler,
        .init = NULL,
        .description = "Stops the continuous loop if it is running. Format: stop",
        .summary = "Stop the continuous loop",
        .flags = NULL,
        .flags_len = 0,
        .enabled = false
    }
}; 

const int COMMANDS_LEN = sizeof(COMMANDS) / sizeof(cli_command_t);