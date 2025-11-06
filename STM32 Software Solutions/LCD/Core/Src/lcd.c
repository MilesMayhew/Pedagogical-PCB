#include "lcd.h"



void LCD_Init(LCD_HandleTypeDef* lcdHandle, SPI_HandleTypeDef* hspi,
              GPIO_TypeDef* gpioPort, uint16_t pinCS, uint16_t pinDC, uint16_t pinRES)
{
    lcdHandle->hspi = hspi;
    lcdHandle->gpioPort = gpioPort;
    lcdHandle->pinCS = pinCS;
    lcdHandle->pinDC = pinDC;
    lcdHandle->pinRES = pinRES;

    // Reset pulse
    HAL_GPIO_WritePin(gpioPort, pinRES, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(gpioPort, pinRES, GPIO_PIN_SET);
    HAL_Delay(100);

    // Exit sleep and turn display on
    LCD_SendCommand(lcdHandle, DISPLAY_OFF); // Sleep out
    LCD_SendCommand(lcdHandle, EXIT_SLEEP);

    LCD_SendCommand(lcdHandle, PWR_CTRL_A);
    LCD_SendData(lcdHandle, 0x39);
    LCD_SendData(lcdHandle, 0x2C);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, 0x34);
    LCD_SendData(lcdHandle, 0x02);

    LCD_SendCommand(lcdHandle, PWR_CTRL_B);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, 0x81);
    LCD_SendData(lcdHandle, 0x30);

    LCD_SendCommand(lcdHandle, PWR_CTRL_1);
    LCD_SendData(lcdHandle, 0x26);
    LCD_SendData(lcdHandle, 0x04);

    LCD_SendCommand(lcdHandle, PWR_CTRL_2);
    LCD_SendData(lcdHandle, 0x11);

    LCD_SendCommand(lcdHandle, VCOM_CTRL_1);
    LCD_SendData(lcdHandle, 0x35);
    LCD_SendData(lcdHandle, 0x3E);

    LCD_SendCommand(lcdHandle, MEM_ACC_CTRL);
    LCD_SendData(lcdHandle, 0x88);

    LCD_SendCommand(lcdHandle, FRAME_RATE_CTRL);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, 0x18);

    LCD_SendCommand(lcdHandle, DISPLAY_FUNC_CTRL);
    LCD_SendData(lcdHandle, 0x0A);
    LCD_SendData(lcdHandle, 0xA2);

    LCD_SendCommand(lcdHandle, VCOM_CTRL_2);
    LCD_SendData(lcdHandle, 0xBE);

    LCD_SendCommand(lcdHandle, PXL_FORMAT);
    LCD_SendData(lcdHandle, 0x55);

    LCD_SendCommand(lcdHandle, GAMMA_3D_CTRL);
    LCD_SendData(lcdHandle, 0x02);

    LCD_SendCommand(lcdHandle, GAMMA_CURVE_3);
    LCD_SendData(lcdHandle, 0x01);

    LCD_SendCommand (lcdHandle, COL_ADDR_SET);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, 0xEF);

    LCD_SendCommand (lcdHandle, PAGE_ADDR_SET);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, 0x01);
    LCD_SendData(lcdHandle, 0x3F);

    LCD_SendCommand (lcdHandle, DISPLAY_ON);

}


// Send a command to the LCD
void LCD_SendCommand(LCD_HandleTypeDef* lcdHandle, uint8_t command) {
    // Select the LCD by driving CS low
    HAL_GPIO_WritePin(lcdHandle->gpioPort, lcdHandle->pinCS, GPIO_PIN_RESET);
    // Set the DC pin low for command mode
    HAL_GPIO_WritePin(lcdHandle->gpioPort, lcdHandle->pinDC, GPIO_PIN_RESET);

    // Send the command
    SPISendByte(lcdHandle, command);

    // Deselect the LCD by driving CS high
    HAL_GPIO_WritePin(lcdHandle->gpioPort, lcdHandle->pinCS, GPIO_PIN_SET);
}

// Send data to the LCD
void LCD_SendData(LCD_HandleTypeDef* lcdHandle, uint8_t data) {
    // Select the LCD by driving CS low
    HAL_GPIO_WritePin(lcdHandle->gpioPort, lcdHandle->pinCS, GPIO_PIN_RESET);
    // Set the DC pin high for data mode
    HAL_GPIO_WritePin(lcdHandle->gpioPort, lcdHandle->pinDC, GPIO_PIN_SET);

    // Send the data
    SPISendByte(lcdHandle, data);

    // Deselect the LCD by driving CS high
    HAL_GPIO_WritePin(lcdHandle->gpioPort, lcdHandle->pinCS, GPIO_PIN_SET);
}
void LCD_TurnOff(LCD_HandleTypeDef* lcdHandle){
	LCD_SendCommand(lcdHandle, DISPLAY_OFF);
}
// === Internal helper ===
void SPI_SendData(LCD_HandleTypeDef *lcd, uint8_t *data, uint16_t size) {
    HAL_SPI_Transmit(lcd->hspi, data, size, HAL_MAX_DELAY);
}
static void LCD_SetAddressWindow(LCD_HandleTypeDef *lcd, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t data[4];

    // Column address
    LCD_SendCommand(lcd, 0x2A);
    data[0] = x0 >> 8; data[1] = x0 & 0xFF;
    data[2] = x1 >> 8; data[3] = x1 & 0xFF;
    SPI_SendData(lcd, data, 4);

    // Page address
    LCD_SendCommand(lcd, 0x2B);
    data[0] = y0 >> 8; data[1] = y0 & 0xFF;
    data[2] = y1 >> 8; data[3] = y1 & 0xFF;
    SPI_SendData(lcd, data, 4);

    // Write memory
    LCD_SendCommand(lcd, 0x2C);
}
// Function to send a byte over SPI
void SPISendByte(LCD_HandleTypeDef* lcdHandle, uint8_t SendValue) {
    uint8_t LocalSendValue = SendValue;
    HAL_SPI_Transmit(lcdHandle->hspi, &LocalSendValue, 1, HAL_MAX_DELAY);
}

// Function to receive a byte over SPI
uint8_t SPIReceiveByte(LCD_HandleTypeDef* lcdHandle) {
    uint8_t ReceiveValue;
    HAL_SPI_Receive(lcdHandle->hspi, &ReceiveValue, 1, HAL_MAX_DELAY);
    return ReceiveValue;
}
void LCD_FillScreen(LCD_HandleTypeDef *lcd, uint16_t color)
{
    uint8_t colorData[512];  // transmit buffer (must be even number of bytes)
    uint16_t chunkPixels = sizeof(colorData) / 2;
    uint32_t totalPixels = LCD_WIDTH * LCD_HEIGHT;

    // Fill color buffer
    for (uint16_t i = 0; i < chunkPixels; i++) {
        colorData[i * 2]     = color >> 8;   // High byte
        colorData[i * 2 + 1] = color & 0xFF; // Low byte
    }

    // Set the full window
    LCD_SetAddressWindow(lcd, 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    // Start data mode
    HAL_GPIO_WritePin(lcd->gpioPort, lcd->pinDC, GPIO_PIN_SET);
    HAL_GPIO_WritePin(lcd->gpioPort, lcd->pinCS, GPIO_PIN_RESET);

    // Send the same chunk repeatedly
    while (totalPixels > 0) {
        uint16_t pixelsToSend = (totalPixels > chunkPixels) ? chunkPixels : totalPixels;
        SPI_SendData(lcd, colorData, pixelsToSend * 2);
        totalPixels -= pixelsToSend;
    }

    HAL_GPIO_WritePin(lcd->gpioPort, lcd->pinCS, GPIO_PIN_SET);
}
void LCD_DrawPixel(LCD_HandleTypeDef* lcd, uint16_t x, uint16_t y, uint16_t color)
{
    LCD_SetAddressWindow(lcd, x, y, x, y);
    HAL_GPIO_WritePin(lcd->gpioPort, lcd->pinDC, GPIO_PIN_SET); // data mode
    HAL_GPIO_WritePin(lcd->gpioPort, lcd->pinCS, GPIO_PIN_RESET);

    uint8_t data[2] = {color >> 8, color & 0xFF};
    SPI_SendData(lcd, data, 2);

    HAL_GPIO_WritePin(lcd->gpioPort, lcd->pinCS, GPIO_PIN_SET);
}
void LCD_FillScreen_Fast(LCD_HandleTypeDef *lcd, uint16_t color)
{
    uint32_t totalPixels = LCD_WIDTH * LCD_HEIGHT;
    uint8_t colorData[2] = { color >> 8, color & 0xFF }; // 2 bytes per pixel

    // Step 1: Set the full screen address window
    LCD_SendCommand(lcd, 0x2A); // Column address set
    uint8_t data_col[4] = { 0, 0, (LCD_WIDTH - 1) >> 8, (LCD_WIDTH - 1) & 0xFF };
    SPI_SendData(lcd, data_col, 4);

    LCD_SendCommand(lcd, 0x2B); // Row address set
    uint8_t data_row[4] = { 0, 0, (LCD_HEIGHT - 1) >> 8, (LCD_HEIGHT - 1) & 0xFF };
    SPI_SendData(lcd, data_row, 4);

    LCD_SendCommand(lcd, 0x2C); // Memory write

    // Step 2: Send all pixel data
    HAL_GPIO_WritePin(lcd->gpioPort, lcd->pinDC, GPIO_PIN_SET);   // Data mode
    HAL_GPIO_WritePin(lcd->gpioPort, lcd->pinCS, GPIO_PIN_RESET); // Select LCD

    // Efficiently repeat the same 2-byte color for the whole screen
    for (uint32_t i = 0; i < totalPixels; i++) {
        SPI_SendData(lcd, colorData, 2);
    }

    HAL_GPIO_WritePin(lcd->gpioPort, lcd->pinCS, GPIO_PIN_SET); // Deselect LCD
}


