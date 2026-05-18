/**
 * @file timestamp.h
 * @author Juan Carlos Dorrejo Paulino (dorrejopauli@wisc.edu)
 * @author Alex Arovas (aarovas@wisc.edu)
 * @brief This file contains the function signatures for the timestamp module.
 * @version 0.1
 * @date 2024-04-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdio.h>

/**
 * @brief Initializes the clock used for the timestamps.
 * 
 * The clock is the IMO (Internal Main Oscillator) which runs at 8 MHz.
 */
void timestamp_init(void);

/**
 * @brief Gets the current time in milliseconds since initialization.
 *
 * @pre The timestamp_init() function must be called before this function. 
 * @return uint32_t The current time in milliseconds since last initialization.
 */
uint32_t millis(void);


/**
 * @brief Gets the current time in microseconds since initialization.
 *
 * @pre The timestamp_init() function must be called before this function. 
 * @return uint32_t The current time in microseconds since last initialization.
 */
uint32_t micros(void);

#endif // TIMESTAMP_H_