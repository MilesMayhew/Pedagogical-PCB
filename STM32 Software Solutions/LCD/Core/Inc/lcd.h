#ifndef LCD_H
#define LCD_H

#include "stm32g0xx_hal.h"

//Defines
#define LCD_WIDTH   240
#define LCD_HEIGHT  320

// Command List
#define DISPLAY_OFF 		0x28
#define DISPLAY_ON 			0x29
#define NORMAL_DISPLAY_ON	0x13
#define EXIT_SLEEP 			0x11
#define PWR_CTRL_A 			0xCB
#define PWR_CTRL_B 			0xCF
#define PWR_CTRL_1 			0xC0
#define PWR_CTRL_2 			0xC1
#define VCOM_CTRL_1 		0xC5
#define VCOM_CTRL_2 		0xC7
#define MEM_ACC_CTRL 		0x36
#define FRAME_RATE_CTRL 	0xB1
#define DISPLAY_FUNC_CTRL 	0xB6
#define PXL_FORMAT 			0x3A
#define GAMMA_3D_CTRL 		0xF2
#define GAMMA_CURVE_3 		0x26
#define COL_ADDR_SET 		0x2A
#define PAGE_ADDR_SET 		0x2B
#define MEM_WRITE			0x2C


//Colors
#define COLOR_RED			0xF800

// Define a structure to hold LCD configuration parameters
typedef struct {
    GPIO_TypeDef* gpioPort;  // GPIO port used for control pins
    uint16_t pinDC;           // Pin for Data/Command (D/CX)
    uint16_t pinCS;           // Pin for Chip Select (CSX)
    uint16_t pinRES;		  // Pin for Chip Reset
    SPI_HandleTypeDef* hspi;  // Pointer to SPI handle
} LCD_HandleTypeDef;

// Function prototypes
void LCD_Init(LCD_HandleTypeDef* lcdHandle, SPI_HandleTypeDef* hspi,
              GPIO_TypeDef* gpioPort,
			  uint16_t pinCS,
			  uint16_t pinDC,
			  uint16_t pinRES);
void LCD_TurnOff(LCD_HandleTypeDef* lcdHandle);
void LCD_SendCommand(LCD_HandleTypeDef* lcdHandle, uint8_t command);
void LCD_SendData(LCD_HandleTypeDef* lcdHandle, uint8_t data);
void SPISendByte(LCD_HandleTypeDef* lcdHandle, uint8_t SendValue);
void LCD_ClearScreen(LCD_HandleTypeDef* lcdHandle);
void LCD_FillScreen(LCD_HandleTypeDef *lcd, uint16_t color);
void LCD_FillScreen_Fast(LCD_HandleTypeDef *lcd, uint16_t color);
void LCD_DrawPixel(LCD_HandleTypeDef* lcd, uint16_t x, uint16_t y, uint16_t color);

#endif // LCD_H


