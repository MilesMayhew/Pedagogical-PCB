#include "main.h"
#include "lcd.h"

// Includes the MSPM0 system initialization and delay functions
#include "system_mspm0_init.c"

// --- Global LCD Handle ---
LCD_HandleTypeDef lcd;

/**
 * @brief Initialize all configured peripherals (MSPM0 wrapper)
 */
void MX_MSPM0_System_Init(void)
{
    // MSPM0 SDK typically initializes the clock (MCLK=32MHz) in startup code.
    // Call user-defined peripheral initialization functions.
    MX_GPIO_Init();
    MX_SPI_Init();
}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    // Configure the system clock and peripherals
    MX_MSPM0_System_Init();

    // Initialize the LCD driver with hardware base addresses and pin assignments
    LCD_Init(&lcd, LCD_SPI_PERIPHERAL, LCD_CTRL_PORT,
             LCD_CS_PIN, LCD_DC_PIN, LCD_RES_PIN);

    // --- Application Infinite Loop ---
    while (1)
    {
        // Cycle through colors filling the screen, demonstrating SPI communication
        LCD_FillScreen(&lcd, 0xF800); // Red (5-6-5 format)
        delay_ms(1000);
        LCD_FillScreen(&lcd, 0x07E0); // Green
        delay_ms(1000);
        LCD_FillScreen(&lcd, 0x001F); // Blue
        delay_ms(1000);
        LCD_FillScreen(&lcd, 0xFFFF); // White
        delay_ms(1000);
    }
}