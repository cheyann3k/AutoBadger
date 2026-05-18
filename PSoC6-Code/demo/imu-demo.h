#ifndef __IMU_DEMO_H__
#define __IMU_DEMO_H__

#include <stdio.h>

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include "../drivers/cli.h"
#include "../drivers/imu.h"
#include "../drivers/console.h"
#include "../drivers/command_types.h"

typedef enum {
    XH,
    XL,
    YH,
    YL,
    ZH,
    ZL,
    UNKNOWN
} orientation_t;

typedef enum {
    X,
    Y,
    Z
} dimension_t;

void cli_imu_temp_handler(cli_command_t* parent, char** argv, int argc);
void cli_imu_acc_handler(cli_command_t* parent, char** argv, int argc);
void cli_imu_or_handler(cli_command_t* parent, char** argv, int argc);
void cli_imu_handler(cli_command_t* parent, char** argv, int argc);

void cli_imu_acc_x_handler(cli_command_t* parent, char** argv, int argc);
void cli_imu_acc_y_handler(cli_command_t* parent, char** argv, int argc);
void cli_imu_acc_z_handler(cli_command_t* parent, char** argv, int argc);
void cli_imu_acc_all_handler(cli_command_t* parent, char** argv, int argc);

bool imu_init(void);
void imu_demo(void);

orientation_t get_orientation(void);
int16_t get_acc(dimension_t dimension);
void get_acc_all_dimensions(int16_t* acc);
int16_t get_temp();

const char* get_orientation_name(orientation_t orientation);

extern cli_command_t IMU_COMMANDS[];
extern const int IMU_COMMANDS_LEN;

extern cli_command_t IMU_ACC_COMMANDS[];
extern const int IMU_ACC_COMMANDS_LEN;

#endif // __IMU_DEMO_H__