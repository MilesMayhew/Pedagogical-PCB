/* Minimal FreeRTOS LED blink on MSPM0
 *
 * - Toggles GPIO pin PA10 every 1000 ms.
 * - Use a jumper wire from PA10 on the TI board to the LED input on your PCB.
 * - Configure the LED circuit with a current-limiting resistor.
 */

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "ti_msp_dl_config.h"   // TI driver library (includes DL_GPIO functions)

/* --- LED output configuration --- */
#define LED_PORT GPIO_PORT_A
#define LED_PIN  DL_GPIO_PIN_10        // Use PA10 as the LED output pin
#define BLINK_INTERVAL_MS 1000UL       // Blink period in milliseconds

/* --- Forward declaration of blink task --- */
static void vBlinkTask(void *pvParameters);

/* --- Main application entry called from main.c --- */
void main_blinky(void)
{
    /* Configure PA10 as a GPIO output */
    DL_GPIO_configPinsAsOutputs(LED_PORT, LED_PIN);

    /* Set PA10 HIGH to turn LED off (assuming common-anode LED).
       If your LED is active-high (common-cathode), use DL_GPIO_clearPins instead. */
    DL_GPIO_setPins(LED_PORT, LED_PIN);

    /* Create one FreeRTOS task to blink the LED */
    xTaskCreate(vBlinkTask, "Blink", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY + 1, NULL);

    /* Start the FreeRTOS scheduler (this does not return) */
    vTaskStartScheduler();

    /* Safety loop in case the scheduler exits */
    for (;;);
}

/* --- Blink task: toggles PA10 every 1 second --- */
static void vBlinkTask(void *pvParameters)
{
    (void) pvParameters;                      // Unused parameter
    const TickType_t xDelay = pdMS_TO_TICKS(BLINK_INTERVAL_MS);

    for (;;)
    {
        /* Toggle the output pin */
        DL_GPIO_togglePins(LED_PORT, LED_PIN);

        /* Wait 1 second before toggling again */
        vTaskDelay(xDelay);
    }
}
