/**
 * @file pwm.c
 * @author Juan Carlos Dorrejo Paulino (dorrejopauli@wisc.edu)
 * @brief This file contains the implementation of the PWM driver for the PSoC 6
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "cy_pdl.h"
#include "cybsp.h"
#include <stdlib.h>
#include <stdio.h>

#include "./pwm.h"


const PWM_CONNECTOR_t J30[] = { 
    { .pwm1 = P9_0, .pwm2 = P9_2 },
    { .pwm1 = P10_0, .pwm2 = P10_2 },
    { .pwm1 = P5_0, .pwm2 = P5_2 },
};
const int J30_LEN = sizeof(J30) / sizeof(PWM_CONNECTOR_t);


pwm_obj_t init_pwm_connector(PWM_CONNECTOR_t CONNECTOR) {
	printf("Initializing PWM on connector...\n");
    cy_rslt_t rslt;
	cyhal_pwm_t *pwm_obj1 = malloc(sizeof(cyhal_pwm_t));
	cyhal_pwm_t *pwm_obj2 = malloc(sizeof(cyhal_pwm_t));
	

	/* Initialize PWM on the supplied pin and assign a new clock */
	rslt = cyhal_pwm_init(pwm_obj1, CONNECTOR.pwm1, NULL);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    rslt = cyhal_pwm_init(pwm_obj2, CONNECTOR.pwm2, NULL);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);

	return (pwm_obj_t){ .obj1 = pwm_obj1, .obj2 = pwm_obj2 };
}

void stop_pwm_connector(pwm_obj_t pwm) {
	cy_rslt_t rslt;
	rslt = cyhal_pwm_stop(pwm.obj1);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
	rslt = cyhal_pwm_stop(pwm.obj2);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
}

void edit_pwm_connector(pwm_obj_t pwm, float dutyCycle, uint32_t frequency) {
	cy_rslt_t rslt;
	if (frequency > 200000) {
		printf("Error: The frequency %li is above recommended operating conditions.\n", frequency);
		printf("Aborting PWM setup...\n");
		return;
	}
	
	// Stop the currently running PWM obj.
	rslt = cyhal_pwm_stop(pwm.obj1);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
	rslt = cyhal_pwm_stop(pwm.obj2);
	CY_ASSERT(CY_RSLT_SUCCESS == rslt);
	
	printf("Changing frequency to: %li\n", frequency);
	printf("Changing dutyCycle to: %f\n", dutyCycle);

	/*
		For this driver: 
		    * forward: duty1 = 100, duty2 = frequency
		    * backward: duty1 = frequency, duty2 = duty1
		
		NOTE: output pwm is the opposite of the used frequency.
			  If freq = 25 => output is 75
	*/
    float duty1 = dutyCycle > 0.0f ? 100.0f - dutyCycle : 100.0f;
    float duty2 = dutyCycle < 0.0f ? 100.0f + dutyCycle : 100.0f; 

	// Assign new frequency and/or duty cycle.
	rslt = cyhal_pwm_set_duty_cycle(pwm.obj1, duty1, frequency);
	rslt = cyhal_pwm_set_duty_cycle(pwm.obj2, duty2, frequency);

	// Start PWM output
	rslt = cyhal_pwm_start(pwm.obj1);
	rslt = cyhal_pwm_start(pwm.obj2);
    printf("Starting pwm...\n");
    
    return;
}



