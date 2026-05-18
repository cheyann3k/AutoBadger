#include "console-demo.h"

void console_demo(void) {
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

    printf("Ready to receive data\r\n");

    while(true) {
        if (ALERT_CONSOLE_RX)
        {
            // Clear the flag
            ALERT_CONSOLE_RX = false;

            // Process the received data
            printf("Received: %s\r\n", pcInputString);

            memset(pcInputString, 0, DEBUG_MESSAGE_MAX_LEN);
            cInputIndex = 0;
        }
    }
}