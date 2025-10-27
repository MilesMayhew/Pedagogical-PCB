#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32g0xx_hal.h" // Adjust based on your STM32 series
#include <stdint.h>        // Include standard integer types

// Define the structure to hold motor driver properties
typedef struct {
    TIM_HandleTypeDef* timer;      // Pointer to the timer
    uint32_t pwmChannel;            // PWM channel to be used
    GPIO_TypeDef* pin1Port;        // Port for pin1
    uint16_t pin1;                  // Pin number for pin1
    GPIO_TypeDef* pin2Port;        // Port for pin2
    uint16_t pin2;                  // Pin number for pin2
} MotorDriver;

// Enum to define motor directions
typedef enum {
    MOTOR_STOP,
    MOTOR_FORWARD,
    MOTOR_REVERSE
} MotorDirection;

// Function prototypes
void MotorDriver_Init(MotorDriver* driver, TIM_HandleTypeDef* timer, uint32_t pwmChannel,
                      GPIO_TypeDef* pin1Port, uint16_t pin1,
                      GPIO_TypeDef* pin2Port, uint16_t pin2);
void MotorDriver_SetPWMDutyCycle(MotorDriver* driver, uint32_t dutyCycle);
void MotorDriver_SetDirection(MotorDriver* driver, MotorDirection direction);
void MotorDriver_GeneratePWMOutput(MotorDriver* driver);

#endif // MOTOR_DRIVER_H
