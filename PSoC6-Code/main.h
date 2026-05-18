/*
 * main.h
 *
 *  Created on: Aug 16, 2022
 *      Author: Joe Krachey
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include <stdio.h>

// #include "drivers/leds.h"
// #include "drivers/push_button.h"
// #include "drivers/console.h"
// #include "drivers/i2c.h"
// #include "drivers/spi.h"
// #include "drivers/imu.h"
// #include "drivers/eeprom.h"
// #include "drivers/leds.h"
// #include "drivers/relay.h"
// #include "drivers/led_driver.h"

#include "drivers/commands.h"
#include "drivers/cli.h"


#define CLI_USE_BLE 0

//#define DEMO_RELAY
//#define DEMO_IO_PINS
//#define DEMO_LED_DRIVER
//#define DEMO_SPI_EEPROM

// void peripheral_init(void);
// void main_app(void);

#endif /* MAIN_H_ */
