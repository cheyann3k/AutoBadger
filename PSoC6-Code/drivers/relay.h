/**
 * @file relay.h
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2024-02-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __RELAY_H__
#define __RELAY_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#define CONN_RELAY_J300
#undef  CONN_RELAY_J301
#undef  CONN_RELAY_J302

#if defined(CONN_RELAY_J300)
#define PIN_RELAY_1				P9_0 /* ADD CODE */
#define PIN_RELAY_2				P9_2 /* ADD CODE */
#define PIN_RELAY_3				P9_3 /* ADD CODE */
#elif defined(CONN_RELAY_J301)
#define PIN_RELAY_1				P10_0 /* ADD CODE */
#define PIN_RELAY_2				P10_2 /* ADD CODE */
#define PIN_RELAY_3				P10_3 /* ADD CODE */
#elif defined(CONN_RELAY_J302)
#define PIN_RELAY_1				P5_0 /* ADD CODE */
#define PIN_RELAY_2				P5_2 /* ADD CODE */
#define PIN_RELAY_3				P5_3 /* ADD CODE */
#else
#error "MUST DEFINE RELAY CONNECTOR"
#endif

// Pin definitions for the ECE453 Staff Dev board

void relay_init(void);


#endif