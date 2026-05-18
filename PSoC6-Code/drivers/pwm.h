/**
 * @file pwm.h
 * @author Juan Carlos Dorrejo Paulino (dorrejopauli@wisc.edu)
 * @brief This file contains the declarations of the PWM driver for the PSoC 6
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef PWM_H_
#define PWM_H_

#include "cyhal.h"

/**
 * @struct PWM_CONNECTOR_s
 * @brief The PWM connector struct. Defines the pins for the PWM.
 */
typedef struct PWM_CONNECTOR_s {
    cyhal_gpio_psoc6_01_43_smt_t pwm1; /**< The first PWM pin. */
    cyhal_gpio_psoc6_01_43_smt_t pwm2; /**< The second PWM pin. */
} PWM_CONNECTOR_t;

/**
 * @struct PWM_OBJ_s
 * @brief The PWM object struct. Contains the PWM objects for the pins.
 */
typedef struct PWM_OBJ_s {
	cyhal_pwm_t *obj1; /**< The PWM object for the first pin. */
	cyhal_pwm_t *obj2; /**< The PWM object for the second pin. */
} pwm_obj_t;


/**
 * @brief The PWM pins for the J300, J301, and J302 connectors. Index represents
 * the connector's last digit.
 */
extern const PWM_CONNECTOR_t J30[];
extern const int J30_LEN;

/*
 * @brief Initialize the PWM on the specified connector.
 * 
 * @param connector The connector to initialize the PWM on.
 * @return the PWM object.
 */
pwm_obj_t init_pwm_connector(PWM_CONNECTOR_t connector);

/**
 * @brief Edit the PWM object with the new duty cycle and frequency.
 * 
 * @pre The PWM object must be initialized.
 * 
 * @param pwm The PWM object to edit.
 * @param dutyCycle The new duty cycle.
 * @param frequency The new frequency.
 */
void edit_pwm_connector(pwm_obj_t pwm, float dutyCycle, uint32_t frequency);

/**
 * @brief Stop the specified pwm object. 
 * 
 * @param pwm The PWM object to stop.
 */
void stop_pwm_connector(pwm_obj_t pwm);


#endif