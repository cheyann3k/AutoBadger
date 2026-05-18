#include "imu-demo.h"

cli_command_t IMU_COMMANDS[] = {
    [0] = {
        .name = "acc",
        .handler = cli_imu_acc_handler,
        .init = NULL,
        .description = "Print the accelerometer data from the IMU. Can get X, Y, Z, or all. Format: imu acc <direction>",
        .summary = "Get the accelerometer data from the IMU",
        .flags = NULL,
        .flags_len = 0
    },
    [1] = {
        .name = "or",
        .handler = cli_imu_or_handler,
        .init = NULL,
        .description = "Prints the orientation of the IMU in a plane direction. XH, XL, YH, etc. Format: imu or",
        .summary = "Get the orientation of the IMU",
        .flags = NULL,
        .flags_len = 0
    },
    [2] = {
        .name = "temp",
        .handler = cli_imu_temp_handler,
        .init = NULL,
        .description = "Prints the temperature of the IMU (NOTE: still unknown units). Format: imu temp",
        .summary = "Get the temperature from the IMU",
        .flags = NULL,
        .flags_len = 0
    }
};

const int IMU_COMMANDS_LEN = sizeof(IMU_COMMANDS) / sizeof(cli_command_t);

cli_command_t IMU_ACC_COMMANDS[] = {
    [0] = {
        .name = "x",
        .handler = cli_imu_acc_x_handler,
        .init = NULL,
        .description = "Prints the acceleration of the IMU in the X direction. Format: imu acc x",
        .summary = "Get the X-axis accelerometer data from the IMU",
        .flags = NULL,
        .flags_len = 0
    },
    [1] = {
        .name = "y",
        .handler = cli_imu_acc_y_handler,
        .init = NULL,
        .description = "Prints the acceleration of the IMU in the Y direction. Format: imu acc y",
        .summary = "Get the Y-axis accelerometer data from the IMU",
        .flags = NULL,
        .flags_len = 0
    },
    [2] = {
        .name = "z",
        .handler = cli_imu_acc_z_handler,
        .init = NULL,
        .description = "Prints the acceleration of the IMU in the Z direction. Format: imu acc z",
        .summary = "Get the Z-axis accelerometer data from the IMU",
        .flags = NULL,
        .flags_len = 0
    },
    [3] = {
        .name = "all",
        .handler = cli_imu_acc_all_handler,
        .init = NULL,
        .description = "Prints the acceleration of the IMU in all directions. Format: imu acc all",
        .summary = "Get all accelerometer data from the IMU",
        .flags = NULL,
        .flags_len = 0
    }
};

const int IMU_ACC_COMMANDS_LEN = sizeof(IMU_ACC_COMMANDS) / sizeof(cli_command_t);

void imu_demo(void) {
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    console_init();

    printf("\x1b[2J\x1b[;H");

    printf("******************\n\r");
    printf("* ECE453 Dev Platform\n\r");
    printf("******************\n\r");

	__enable_irq();

    printf("ECE453 Startup\r\n");
    printf("SPI INIT\r\n");

    result = spi_init();

    if (result != CY_RSLT_SUCCESS)
    {
        printf("SPI Issue: %lu\r\n", result);
        CY_ASSERT(0);
    }

    printf("SPI INITIALIZED\r\n");

    // IMU Test
    printf("IMU INIT\r\n");

    int imu_init_res = lsm6dsm_init();

    if (imu_init_res != 0)
    {
        printf("IMU Issue: %i\r\n", imu_init_res);
        CY_ASSERT(0);
    }

    printf("IMU INITIALIZED\r\n");
    printf("Ready for IMU Data:\r\n");

    lsm6dsm_orientation();
}

bool imu_init(void) {

    if (!spi_init_flag) {
        printf("SPI INIT\r\n");

        cy_rslt_t result = spi_init();

        if (result != CY_RSLT_SUCCESS)
        {
            printf("SPI Issue: %lu\r\n", result);
            CY_ASSERT(0);
            return false;
        }

        printf("SPI INITIALIZED\r\n");
    }

    // IMU Test
    printf("IMU INIT\r\n");

    int imu_init_res = lsm6dsm_init();

    if (imu_init_res != 0)
    {
        printf("IMU Issue: %i\r\n", imu_init_res);
        CY_ASSERT(0);
        return false;
    }

    printf("IMU INITIALIZED\r\n");

    return true;
}

void cli_imu_handler(cli_command_t* parent, char** argv, int argc) {
    bool subcommand = cli_command_handler(parent, IMU_COMMANDS, IMU_COMMANDS_LEN, argv, argc);

    if (!subcommand) {
        printf("IMU Command not recognized - Must have subcommand\r\n");
    }
}

void cli_imu_or_handler(cli_command_t* parent, char** argv, int argc) {
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc == 0) {
            orientation_t orientation = get_orientation();
            printf("IMU Orientation: %s\r\n", get_orientation_name(orientation));
        } else {
            printf("IMU Orientation Command not recognized - Extra Argument(s) (Expected 0)\r\n");
        }
    }
}

void cli_imu_acc_handler(cli_command_t* parent, char** argv, int argc) {
    bool subcommand = cli_command_handler(parent, IMU_ACC_COMMANDS, IMU_ACC_COMMANDS_LEN, argv, argc);

    if (!subcommand) {
        printf("IMU Acc Command not recognized - Must have subcommand\r\n");
    }
}

void cli_imu_acc_x_handler(cli_command_t* parent, char** argv, int argc) {
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc == 0) {
            int16_t acc = get_acc(X);
            printf("IMU Accelerometer X: %i\r\n", acc);
        } else {
            printf("IMU Accelerometer X Command not recognized - Extra Argument(s) (Expected 0)\r\n");
        }
    }
}

void cli_imu_acc_y_handler(cli_command_t* parent, char** argv, int argc) {
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc == 0) {
            int16_t acc = get_acc(Y);
            printf("IMU Accelerometer Y: %i\r\n", acc);
        } else {
            printf("IMU Accelerometer Y Command not recognized - Extra Argument(s) (Expected 0)\r\n");
        }
    }
}

void cli_imu_acc_z_handler(cli_command_t* parent, char** argv, int argc) {
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc == 0) {
            int16_t acc = get_acc(Z);
            printf("IMU Accelerometer Z: %i\r\n", acc);
        } else {
            printf("IMU Accelerometer Z Command not recognized - Extra Argument(s) (Expected 0)\r\n");
        }
    }
}

void cli_imu_acc_all_handler(cli_command_t* parent, char** argv, int argc) {
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc == 0) {
            int16_t acc[3];
            get_acc_all_dimensions(acc);
            printf("IMU Accelerometer All:\n\tX: %i\n\tY: %i\n\tZ: %i\r\n", acc[0], acc[1], acc[2]);
        } else {
            printf("IMU Accelerometer Z Command not recognized - Extra Argument(s) (Expected 0)\r\n");
        }
    }
}

void cli_imu_temp_handler(cli_command_t* parent, char** argv, int argc) {
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc == 0) {
            int16_t temp = get_temp();
            printf("IMU Temperature: %i\r\n", temp);
        } else {
            printf("IMU Temperature Command not recognized - Extra Argument(s) (Expected 0)\r\n");
        }
    }
}

orientation_t get_orientation(void) {
    lsm6dsm_all_sources_t all_source;

    /* Check if 6D Orientation events */
    lsm6dsm_all_sources_get(&dev_ctx, &all_source);

    if (all_source.d6d_src.xh) {
        return XH;
    } else if (all_source.d6d_src.xl) {
        return XL;
    } else if (all_source.d6d_src.yh) {
        return YH;
    } else if (all_source.d6d_src.yl) {
        return YL;
    } else if (all_source.d6d_src.zh) {
        return ZH;
    } else if (all_source.d6d_src.zl) {
        return ZL;
    } else {
        return UNKNOWN;
    }
}

const char* get_orientation_name(orientation_t orientation) {
    switch (orientation) {
        case XH: return "XH";
        case XL: return "XL";
        case YH: return "YH";
        case YL: return "YL";
        case ZH: return "ZH";
        case ZL: return "ZL";
        default: return "UNKNOWN";
    }
}

int16_t get_acc(dimension_t dimension) {
    int16_t acc[3];
    lsm6dsm_acceleration_raw_get(&dev_ctx, acc);

    switch (dimension) {
        case X:  return acc[0];
        case Y:  return acc[1];
        default: return acc[2]; // Z
    }
}

void get_acc_all_dimensions(int16_t* acc) {
    lsm6dsm_acceleration_raw_get(&dev_ctx, acc);
}

int16_t get_temp() {
    int16_t raw_temperature;
    lsm6dsm_temperature_raw_get(&dev_ctx, &raw_temperature);
    return raw_temperature;
}