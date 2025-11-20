#include "lcd.h"
#include "system_mspm0_init.c" // Include for delay_ms() function

// Function to send a block of data over SPI
void SPI_SendData(LCD_HandleTypeDef *lcd, uint8_t *data, uint16_t size) {
    // MSPM0 equivalent of HAL_SPI_Transmit with a timeout/blocking
    DL_SPI_transmitDataBlocking(lcd->spiHandle, data, size);
}

// Function to send a single byte over SPI (wrapper for general convenience)
void SPISendByte(LCD_HandleTypeDef* lcdHandle, uint8_t SendValue) {
    DL_SPI_transmitDataBlocking(lcdHandle->spiHandle, &SendValue, 1);
}


// Send a command to the LCD
void LCD_SendCommand(LCD_HandleTypeDef* lcdHandle, uint8_t command) {
    // Select the LCD by driving CS low
    DL_GPIO_clearPins(lcdHandle->gpioPort, lcdHandle->pinCS);
    // Set the DC pin low for command mode
    DL_GPIO_clearPins(lcdHandle->gpioPort, lcdHandle->pinDC);

    // Send the command
    SPISendByte(lcdHandle, command);

    // Deselect the LCD by driving CS high
    DL_GPIO_setPins(lcdHandle->gpioPort, lcdHandle->pinCS);
}

// Send data to the LCD
void LCD_SendData(LCD_HandleTypeDef* lcdHandle, uint8_t data) {
    // Select the LCD by driving CS low
    DL_GPIO_clearPins(lcdHandle->gpioPort, lcdHandle->pinCS);
    // Set the DC pin high for data mode
    DL_GPIO_setPins(lcdHandle->gpioPort, lcdHandle->pinDC);

    // Send the data
    SPISendByte(lcdHandle, data);

    // Deselect the LCD by driving CS high
    DL_GPIO_setPins(lcdHandle->gpioPort, lcdHandle->pinCS);
}
void LCD_TurnOff(LCD_HandleTypeDef* lcdHandle){
	LCD_SendCommand(lcdHandle, DISPLAY_OFF);
}

// Internal helper to set the address window
static void LCD_SetAddressWindow(LCD_HandleTypeDef *lcd, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t data[4];

    // Column address
    LCD_SendCommand(lcd, COL_ADDR_SET);
    data[0] = (uint8_t)(x0 >> 8); data[1] = (uint8_t)(x0 & 0xFF);
    data[2] = (uint8_t)(x1 >> 8); data[3] = (uint8_t)(x1 & 0xFF);
    SPI_SendData(lcd, data, 4);

    // Page address
    LCD_SendCommand(lcd, PAGE_ADDR_SET);
    data[0] = (uint8_t)(y0 >> 8); data[1] = (uint8_t)(y0 & 0xFF);
    data[2] = (uint8_t)(y1 >> 8); data[3] = (uint8_t)(y1 & 0xFF);
    SPI_SendData(lcd, data, 4);

    // Write memory
    LCD_SendCommand(lcd, MEM_WRITE);
}

void LCD_Init(LCD_HandleTypeDef* lcdHandle, uint32_t hspiBase,
              uint32_t gpioPort, uint32_t pinCS, uint32_t pinDC, uint32_t pinRES)
{
    // Store hardware configuration in the handle
    lcdHandle->spiHandle = hspiBase;
    lcdHandle->gpioPort = gpioPort;
    lcdHandle->pinCS = pinCS;
    lcdHandle->pinDC = pinDC;
    lcdHandle->pinRES = pinRES;

    // Reset pulse (uses MSPM0 DL_GPIO functions and the delay_ms from system_mspm0_init.c)
    DL_GPIO_clearPins(gpioPort, pinRES);
    delay_ms(100);
    DL_GPIO_setPins(gpioPort, pinRES);
    delay_ms(100);

    // --- LCD Initialization Sequence (Commands from original lcd.c) ---

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
    LCD_SendData(lcdHandle, 0x55); // 16-bit/pixel

    LCD_SendCommand(lcdHandle, GAMMA_3D_CTRL);
    LCD_SendData(lcdHandle, 0x02);

    LCD_SendCommand(lcdHandle, GAMMA_CURVE_3);
    LCD_SendData(lcdHandle, 0x01);

    // Set full screen address window (0,0 to LCD_WIDTH-1, LCD_HEIGHT-1)
    LCD_SendCommand (lcdHandle, COL_ADDR_SET);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, (uint8_t)((LCD_WIDTH - 1) >> 8));
    LCD_SendData(lcdHandle, (uint8_t)((LCD_WIDTH - 1) & 0xFF));

    LCD_SendCommand (lcdHandle, PAGE_ADDR_SET);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, 0x00);
    LCD_SendData(lcdHandle, (uint8_t)((LCD_HEIGHT - 1) >> 8));
    LCD_SendData(lcdHandle, (uint8_t)((LCD_HEIGHT - 1) & 0xFF));

    LCD_SendCommand (lcdHandle, DISPLAY_ON);
}

void LCD_FillScreen(LCD_HandleTypeDef *lcd, uint16_t color)
{
    uint8_t colorData[512];  // transmit buffer (must be even number of bytes)
    uint16_t chunkPixels = sizeof(colorData) / 2;
    uint32_t totalPixels = LCD_WIDTH * LCD_HEIGHT;

    // Fill color buffer
    for (uint16_t i = 0; i < chunkPixels; i++) {
        colorData[i * 2]     = (uint8_t)(color >> 8);   // High byte
        colorData[i * 2 + 1] = (uint8_t)(color & 0xFF); // Low byte
    }

    // Set the full window
    LCD_SetAddressWindow(lcd, 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    // Start data mode
    DL_GPIO_setPins(lcd->gpioPort, lcd->pinDC);
    DL_GPIO_clearPins(lcd->gpioPort, lcd->pinCS);

    // Send the same chunk repeatedly
    while (totalPixels > 0) {
        uint16_t pixelsToSend = (totalPixels > chunkPixels) ? chunkPixels : (uint16_t)totalPixels;
        SPI_SendData(lcd, colorData, pixelsToSend * 2);
        totalPixels -= pixelsToSend;
    }

    DL_GPIO_setPins(lcd->gpioPort, lcd->pinCS);
}

void LCD_DrawPixel(LCD_HandleTypeDef* lcd, uint16_t x, uint16_t y, uint16_t color)
{
    // Bound check
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;

    LCD_SetAddressWindow(lcd, x, y, x, y);

    // data mode and select chip
    DL_GPIO_setPins(lcd->gpioPort, lcd->pinDC);
    DL_GPIO_clearPins(lcd->gpioPort, lcd->pinCS);

    uint8_t data[2] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
    SPI_SendData(lcd, data, 2);

    // Deselect chip
    DL_GPIO_setPins(lcd->gpioPort, lcd->pinCS);
}

void LCD_FillScreen_Fast(LCD_HandleTypeDef *lcd, uint16_t color)
{
    uint32_t totalPixels = LCD_WIDTH * LCD_HEIGHT;
    uint8_t colorData[2] = { (uint8_t)(color >> 8), (uint8_t)(color & 0xFF) }; // 2 bytes per pixel

    // Step 1: Set the full screen address window (uses internal helper)
    LCD_SetAddressWindow(lcd, 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    // Step 2: Send all pixel data
    DL_GPIO_setPins(lcd->gpioPort, lcd->pinDC);   // Data mode
    DL_GPIO_clearPins(lcd->gpioPort, lcd->pinCS); // Select LCD

    // Efficiently repeat the same 2-byte color for the whole screen
    for (uint32_t i = 0; i < totalPixels; i++) {
        SPISendByte(lcd, colorData[0]); // High byte
        SPISendByte(lcd, colorData[1]); // Low byte
    }

    DL_GPIO_setPins(lcd->gpioPort, lcd->pinCS); // Deselect LCD
}