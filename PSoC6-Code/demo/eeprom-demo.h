#ifndef __EEPROM_DEMO_H__
#define __EEPROM_DEMO_H__

#include <stdio.h>

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include "../drivers/console.h"
#include "../drivers/eeprom.h"
#include "../drivers/spi.h"
#include "../drivers/command_types.h"
#include "../drivers/cli.h"

bool eeprom_init(void);

void cli_eeprom_handler(cli_command_t* parent, char** argv, int argc);

void cli_eeprom_w_handler(cli_command_t* parent, char** argv, int argc);
void cli_eeprom_r_handler(cli_command_t* parent, char** argv, int argc);
void cli_eeprom_test_handler(cli_command_t* parent, char** argv, int argc);

extern cli_command_t EEPROM_COMMANDS[];
extern const int EEPROM_COMMANDS_LEN;


#endif // __EEPROM_DEMO_H__