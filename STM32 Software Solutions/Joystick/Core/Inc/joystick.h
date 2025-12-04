#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "stm32g0xx_hal.h"     // Change to match your MCU family
#include <stdint.h>

typedef struct
{
    uint16_t rawX;     // Raw ADC X
    uint16_t rawY;     // Raw ADC Y
    float    normX;    // Normalized -1.0f .. +1.0f
    float    normY;    // Normalized -1.0f .. +1.0f
} Joystick_t;

/**
 * @brief  Initialize joystick module.
 * @param  hadc   Pointer to ADC handle (must use 2-channel scan + DMA).
 */
void Joystick_Init(ADC_HandleTypeDef *hadc);

/**
 * @brief  Start continuous DMA sampling.
 */
void Joystick_Start(void);

/**
 * @brief  Process latest DMA samples.
 *         Call this inside HAL_ADC_ConvCpltCallback().
 */
void Joystick_Process(void);

/**
 * @brief  Get latest joystick state.
 */
Joystick_t Joystick_Get(void);

#endif // JOYSTICK_H
