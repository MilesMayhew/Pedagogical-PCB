#include "LightSensor.h"

// -----------------------------------------------------------------------------
// Local helper to write a one-byte command
// -----------------------------------------------------------------------------
static HAL_StatusTypeDef BH1750_WriteCmd(BH1750 *dev, uint8_t cmd)
{
    return HAL_I2C_Master_Transmit(dev->i2c, dev->addr, &cmd, 1, HAL_MAX_DELAY);
}

// -----------------------------------------------------------------------------
// Initialize the BH1750
// -----------------------------------------------------------------------------
HAL_StatusTypeDef BH1750_Init(BH1750 *dev, I2C_HandleTypeDef *i2c,
                              uint8_t highAddress)
{
    dev->i2c  = i2c;
    dev->addr = highAddress ? BH1750_ADDR_HIGH : BH1750_ADDR_LOW;
    dev->mtreg = 69;  // Default per datasheet (typical 120ms)

    // Power on
    if (BH1750_WriteCmd(dev, BH1750_CMD_POWER_ON) != HAL_OK)
        return HAL_ERROR;

    // Reset data register (requires power-on)
    if (BH1750_WriteCmd(dev, BH1750_CMD_RESET) != HAL_OK)
        return HAL_ERROR;

    // Default measurement mode
    dev->mode = BH1750_CONT_H_RES;
    return BH1750_SetMode(dev, dev->mode);
}

// -----------------------------------------------------------------------------
// Set measurement mode
// -----------------------------------------------------------------------------
HAL_StatusTypeDef BH1750_SetMode(BH1750 *dev, BH1750_Mode mode)
{
    dev->mode = mode;
    return BH1750_WriteCmd(dev, (uint8_t)mode);
}

// -----------------------------------------------------------------------------
// Set measurement time (MTreg) — valid range 31…254
// -----------------------------------------------------------------------------
HAL_StatusTypeDef BH1750_SetMTreg(BH1750 *dev, uint8_t mtreg)
{
    if (mtreg < 31 || mtreg > 254)
        return HAL_ERROR;

    dev->mtreg = mtreg;

    uint8_t high = 0x40 | (mtreg >> 5);
    uint8_t low  = 0x60 | (mtreg & 0x1F);

    if (BH1750_WriteCmd(dev, high) != HAL_OK)
        return HAL_ERROR;

    if (BH1750_WriteCmd(dev, low) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}

// -----------------------------------------------------------------------------
// Read lux from sensor
// -----------------------------------------------------------------------------
HAL_StatusTypeDef BH1750_ReadLux(BH1750 *dev, float *lux)
{
    uint8_t raw[2];
    uint16_t value;

    // Determine required measurement time
    uint32_t wait_ms = (dev->mtreg * 120) / 69; // Scale per datasheet

    // Continuous mode: delay only for first read
    // One-time modes require start→delay→read
    if ((dev->mode & 0x20) == 0x20) {
        // One-time mode → issue mode command
        if (BH1750_WriteCmd(dev, dev->mode) != HAL_OK)
            return HAL_ERROR;
    }

    HAL_Delay(wait_ms);

    if (HAL_I2C_Master_Receive(dev->i2c, dev->addr, raw, 2, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    value = (raw[0] << 8) | raw[1];

    // Convert to lux with MTreg scaling
    // 1 lx = (value / 1.2) * (69 / MTreg)
    float ratio = 69.0f / (float)dev->mtreg;
    *lux = (value / 1.2f) * ratio;

    return HAL_OK;
}
