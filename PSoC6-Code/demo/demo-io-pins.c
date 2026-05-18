/**
 * @file demo-relay.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2024-02-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "../main.h"

#if defined(DEMO_IO_PINS)
void peripheral_init(void)
{
    cy_rslt_t rslt; 

    /* Initialize Push Button */
    rslt = cyhal_gpio_init(P5_6, CYHAL_GPIO_DIR_INPUT,CYHAL_GPIO_DRIVE_NONE, false);
    if(rslt != CY_RSLT_SUCCESS)
    {
        printf("Error initializing push button\n\r");
        while(1){};
    }

    /* Initialize LED */
    rslt = cyhal_gpio_init(P5_5, CYHAL_GPIO_DIR_OUTPUT,CYHAL_GPIO_DRIVE_STRONG, false);
    if(rslt != CY_RSLT_SUCCESS)
    {
        printf("Error initializing push button\n\r");
        while(1){};
    }


}

void main_app(void)
{
    bool button_logic_level = true;
    while(1)
    {
        /* If Push Button is pressed, turn on LED */
        button_logic_level = cyhal_gpio_read(P5_6);

        if( button_logic_level == false)
        {
            printf("Button Pressed\n\r");
            cyhal_gpio_write(P5_5, 1);
        }
        else
        {
            cyhal_gpio_write(P5_5, 0);
        }

        /* Sleep for 100mS*/
        cyhal_system_delay_ms(100);

    }
}

#endif