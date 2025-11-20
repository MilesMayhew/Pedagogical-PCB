#ifndef MAIN_H_
#define MAIN_H_

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>

// --- LCD Definitions ---
#ifndef LCD_H_
#define LCD_H_

#include "main.h"

// --- LCD Control Struct (HAL replacement) ---
typedef struct {
    // SPI peripheral base address (acts as the handle)
    uint32_t spiHandle;
    // GPIO Port and Pins for control
    uint32_t gpioPort;
    uint32_t pinCS;
    uint32_t pinDC;
    uint32_t pinRES;
} LCD_HandleTypeDef;


// --- LCD Commands (ST7735/ST7789 compatible) ---
#define EXIT_SLEEP              0x11
#define DISPLAY_ON              0x29
#define DISPLAY_OFF             0x28
#define COL_ADDR_SET            0x2A
#define PAGE_ADDR_SET           0x2B
#define MEM_WRITE               0x2C
#define PWR_CTRL_A              0xCB
#define PWR_CTRL_B              0xCF
#define PWR_CTRL_1              0xC0
#define PWR_CTRL_2              0xC1
#define VCOM_CTRL_1             0xC5
#define MEM_ACC_CTRL            0x36
#define PXL_FORMAT              0x3A
#define FRAME_RATE_CTRL         0xB1
#define DISPLAY_FUNC_CTRL       0xB6
#define VCOM_CTRL_2             0xC7
#define GAMMA_3D_CTRL           0x26
#define GAMMA_CURVE_3           0xE0

// --- Function Prototypes ---

// Low-level SPI functions (implemented in lcd.c, using MSPM0 SDK)
void SPISendByte(LCD_HandleTypeDef* lcdHandle, uint8_t SendValue);
void SPI_SendData(LCD_HandleTypeDef *lcd, uint8_t *data, uint16_t size);

// LCD Control
void LCD_SendCommand(LCD_HandleTypeDef* lcdHandle, uint8_t command);
void LCD_SendData(LCD_HandleTypeDef* lcdHandle, uint8_t data);
void LCD_TurnOff(LCD_HandleTypeDef* lcdHandle);
void LCD_SetAddressWindow(LCD_HandleTypeDef *lcd, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

// LCD Drawing/Init
void LCD_Init(LCD_HandleTypeDef* lcdHandle, uint32_t hspiBase,
              uint32_t gpioPort, uint32_t pinCS, uint32_t pinDC, uint32_t pinRES);
void LCD_FillScreen(LCD_HandleTypeDef *lcd, uint16_t color);
void LCD_DrawPixel(LCD_HandleTypeDef* lcd, uint16_t x, uint16_t y, uint16_t color);
void LCD_FillScreen_Fast(LCD_HandleTypeDef *lcd, uint16_t color);

#endif /* LCD_H_ */