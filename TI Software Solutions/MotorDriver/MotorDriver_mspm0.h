#ifndef MOTOR_DRIVER_MSPM0_H
#define MOTOR_DRIVER_MSPM0_H

#include <stdint.h>
#include <ti/drivers/GPIO.h>

typedef enum {
    MOTOR_STOP = 0,
    MOTOR_FORWARD,
    MOTOR_REVERSE
} MotorDirection;

typedef struct {
    uint_least8_t pin_index; /* single control pin (pin_0) */
    MotorDirection direction;
    uint32_t pwmDuty; /* 0..100 */
} MotorDriver;

void MotorDriver_Init(MotorDriver *drv, uint_least8_t pin_index);
void MotorDriver_SetPWMDutyCycle(MotorDriver *drv, uint32_t dutyPercent); /* 0..100 */
void MotorDriver_SetDirection(MotorDriver *drv, MotorDirection dir);
void MotorDriver_GeneratePWMOutput(MotorDriver *drv); /* call from 1 ms tick */

#endif /* MOTOR_DRIVER_MSPM0_H */
