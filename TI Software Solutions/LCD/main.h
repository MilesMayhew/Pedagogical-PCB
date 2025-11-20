#ifndef MAIN_H_
#define MAIN_H_

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>

// --- LCD Definitions ---
// Define your LCD width and height here
#define LCD_WIDTH   160
#define LCD_HEIGHT  128

// --- MSPM0 Peripheral and Pin Definitions ---
// SPI Module
#define LCD_SPI_PERIPHERAL      SPI0_BASE

// ** FINALIZED SPI PIN ASSIGNMENTS (GPIOA) **
#define LCD_SCK_PORT        GPIOA
#define LCD_SCK_PIN         DL_GPIO_PIN_11  // SCK (PA11) - Controller Clock
#define LCD_MOSI_PIN        DL_GPIO_PIN_14  // MOSI (PA14) - PICO (Controller Output)
#define LCD_MISO_PIN        DL_GPIO_PIN_10  // MISO (PA10) - POCI (Controller Input)

// ** FINALIZED GPIO Control Pins (GPIOB) **
#define LCD_CTRL_PORT       GPIOB
#define LCD_CS_PIN          DL_GPIO_PIN_1   // Chip Select (CS) - PB1
#define LCD_DC_PIN          DL_GPIO_PIN_2   // Data/Command (DC) - PB2
#define LCD_RES_PIN         DL_GPIO_PIN_3   // Reset (RES) - PB3


// Function Prototypes
void delay_ms(uint32_t ms);
void SystemClock_Config(void);
void Error_Handler(void);
void MX_SPI_Init(void);
void MX_GPIO_Init(void);
void MX_MSPM0_System_Init(void);

#endif /* MAIN_H_ */