/**
 * @file timestamp.c
 * @author Juan Carlos Dorrejo Paulino (dorrejopauli@wisc.edu)
 * @author Alex Arovas (aarovas@wisc.edu)
 * @brief This file contains the implementation of the timestamp driver used for timekeeping 
 * @version 0.1
 * @date 2024-04-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "timestamp.h"

#include "cybsp.h"

#include <stdbool.h>

#define SEC_TO_MILLIS 1000    // 1 second = 1000 milliseconds
#define SEC_TO_MICROS 1000000 // 1 second = 1000000 microseconds

#define MILLIS_FREQ (CY_SYSCLK_IMO_FREQ / SEC_TO_MILLIS) // Frequency in mHz (milliHertz)
#define MICROS_FREQ (CY_SYSCLK_IMO_FREQ / SEC_TO_MICROS) // Frequency in uHz (microHertz)

#define INITIAL_COUNTDOWN_VALUE (CY_SYSCLK_IMO_FREQ - 1) // The initial value of the SysTick counter

// The number of ticks that have elapsed since the last SysTick interrupt
#define ELAPSED_TICKS (INITIAL_COUNTDOWN_VALUE - Cy_SysTick_GetValue()) 


static uint32_t seconds = 0; // Increments every second since initialization
static bool initialized = false;

/*********************************************************************************/

/**
 * @brief Interrupt service routine for the SysTick timer. Get's triggered every second; When the 
 * timer reaches 0.
 */
void systick_isr(void) {
    seconds++;
}

void timestamp_init() {
    if (initialized) return;
    initialized = true;

    // Initialize and enable the SysTick resource
    Cy_SysTick_Init(CY_SYSTICK_CLOCK_SOURCE_CLK_IMO, CY_SYSCLK_IMO_FREQ - 1);
    Cy_SysTick_SetCallback(0, systick_isr);
}

uint32_t millis() {
    unsigned long elapsedMilliSeconds = (ELAPSED_TICKS / MILLIS_FREQ);
    return elapsedMilliSeconds + (seconds * SEC_TO_MILLIS);
}

uint32_t micros() {
    unsigned long elapsedMicroSeconds = (ELAPSED_TICKS / MICROS_FREQ);
    return elapsedMicroSeconds + (seconds * SEC_TO_MICROS);
}