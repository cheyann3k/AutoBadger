#include "eeprom-demo.h"

cli_command_t EEPROM_COMMANDS[] = {
    [0] = {
        .name = "r",
        .handler = cli_eeprom_r_handler,
        .init = NULL,
        .description = "Reads a byte from the EEPROM at the specified address. Format: eeprom r <address>",
        .summary = "Read a byte from the EEPROM",
        .flags = NULL,
        .flags_len = 0
    },
    [1] = {
        .name = "w",
        .handler = cli_eeprom_w_handler,
        .init = NULL,
        .description = "Writes a byte to the EEPROM at the specified address. Format: eeprom w <address> <data>",
        .summary = "Write a byte to the EEPROM",
        .flags = NULL,
        .flags_len = 0
    },
    [2] = {
        .name = "test",
        .handler = cli_eeprom_test_handler,
        .init = NULL,
        .description = "Tests the EEPROM by writing and reading 20 bytes to and from the EEPROM. Format: eeprom test",
        .summary = "Test the EEPROM",
        .flags = NULL,
        .flags_len = 0
    }
};

const int EEPROM_COMMANDS_LEN = sizeof(EEPROM_COMMANDS) / sizeof(cli_command_t);

void eeprom_demo(void) {
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


    printf("EEPROM INIT\r\n");

    result = eeprom_cs_init();

    if (result != CY_RSLT_SUCCESS)
    {
        printf("EEPROM Issue: %lu\r\n", result);
        CY_ASSERT(0);
    }

    printf("EEPROM INITIALIZED\r\n");
    printf("Ready for EEPROM Data:\r\n");

    eeprom_test();
}

bool eeprom_init(void) {
    cy_rslt_t result;

    if (!spi_init_flag) {
        printf("SPI INIT\r\n");

        result = spi_init();

        if (result != CY_RSLT_SUCCESS)
        {
            printf("SPI Issue: %lu\r\n", result);
            CY_ASSERT(0);
            return false;
        }

        printf("SPI INITIALIZED\r\n");
    }


    printf("EEPROM INIT\r\n");

    result = eeprom_cs_init();

    if (result != CY_RSLT_SUCCESS)
    {
        printf("EEPROM Issue: %lu\r\n", result);
        CY_ASSERT(0);
        return false;
    }

    printf("EEPROM INITIALIZED\r\n");
    return true;
}

void cli_eeprom_handler(cli_command_t* parent, char** argv, int argc) {
    bool subcommand = cli_command_handler(parent, EEPROM_COMMANDS, EEPROM_COMMANDS_LEN, argv, argc);

    if (!subcommand) {
        printf("EEPROM Command not recognized - Must have subcommand\r\n");
    }
}

void cli_eeprom_r_handler(cli_command_t* parent, char** argv, int argc) {
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc == 1) {
            uint16_t address = (uint16_t)strtol(argv[0], NULL, 16);
            uint8_t data;

            cy_rslt_t result = eeprom_read_byte(address, &data);

            if (result != CY_RSLT_SUCCESS) {
                printf("EEPROM Read Failure: %lu\r\n", result);
            } else {
                printf("EEPROM Read Success: 0x%02X @ 0x%02X\r\n", data, address);
            }
        } else {
            if (argc > 1) {
                printf("EEPROM r (read) Command not recognized - Too Many Arguments (Expected 1)\r\n");
            } else {
                printf("EEPROM r (read) Command not recognized - Too Few Argument(s) (Expected 1)\r\n");
            }
        }
    }
}

void cli_eeprom_w_handler(cli_command_t* parent, char** argv, int argc) {
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc == 2) {
            uint16_t address = (uint16_t)strtol(argv[0], NULL, 16);
            uint8_t data = (uint8_t)strtol(argv[1], NULL, 16);

            cy_rslt_t result = eeprom_write_byte(address, data);

            if (result != CY_RSLT_SUCCESS) {
                printf("EEPROM Write Failure: %lu\r\n", result);
            } else {
                printf("EEPROM Write Success: 0x%02X @ 0x%02X\r\n", data, address);
            }
        } else {
            if (argc > 2) {
                printf("EEPROM w (write) Command not recognized - Too Many Arguments (Expected 2)\r\n");
            } else {
                printf("EEPROM w (write) Command not recognized - Too Few Argument(s) (Expected 2)\r\n");
            }
        }
    }
}

void cli_eeprom_test_handler(cli_command_t* parent, char** argv, int argc) {
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc == 0) {
            eeprom_test();
        } else {
            printf("EEPROM test Command not recognized - Too Many Arguments (Expected 0)\r\n");
        }
    }
}