#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "stm32g0xx_hal.h"
#include <stdint.h>

// -----------------------------------------------------------------------------
// BH1750 I2C Addresses
// -----------------------------------------------------------------------------
#define BH1750_ADDR_LOW   (0x23 << 1)
#define BH1750_ADDR_HIGH  (0x5C << 1)

// -----------------------------------------------------------------------------
// BH1750 Commands (from datasheet)
// -----------------------------------------------------------------------------
#define BH1750_CMD_POWER_DOWN     0x00
#define BH1750_CMD_POWER_ON       0x01
#define BH1750_CMD_RESET          0x07

// Measurement modes
typedef enum {
    BH1750_CONT_H_RES      = 0x10,   // 1 lx, 120ms
    BH1750_CONT_H_RES2     = 0x11,   // 0.5 lx, 120ms
    BH1750_CONT_L_RES      = 0x13,   // 4 lx, 16ms

    BH1750_ONE_H_RES       = 0x20,   // 1 lx, 120ms
    BH1750_ONE_H_RES2      = 0x21,   // 0.5 lx, 120ms
    BH1750_ONE_L_RES       = 0x23    // 4 lx, 16ms
} BH1750_Mode;

// -----------------------------------------------------------------------------
// Device structure
// -----------------------------------------------------------------------------
typedef struct {
    I2C_HandleTypeDef *i2c;
    uint16_t addr;
    BH1750_Mode mode;
    uint8_t mtreg;       // Measurement time register (31–254)
} BH1750;

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------
HAL_StatusTypeDef BH1750_Init(BH1750 *dev, I2C_HandleTypeDef *i2c,
                              uint8_t highAddress);

HAL_StatusTypeDef BH1750_SetMode(BH1750 *dev, BH1750_Mode mode);

HAL_StatusTypeDef BH1750_SetMTreg(BH1750 *dev, uint8_t mtreg);

HAL_StatusTypeDef BH1750_ReadLux(BH1750 *dev, float *lux);

#endif
