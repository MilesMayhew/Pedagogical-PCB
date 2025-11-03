#include "MotorDriver_mspm0.h"
#include <ti/drivers/GPIO.h>

/* Low-frequency software PWM matching previous example */
#define PWM_FREQUENCY     10U
#define PWM_PERIOD_MS     (1000U / PWM_FREQUENCY)  /* 100 ms */

static volatile uint32_t elapsedMs = 0;
static volatile uint8_t pwmState = 0; /* 0 = HIGH, 1 = LOW */

void MotorDriver_Init(MotorDriver *drv, uint_least8_t pin_index)
{
    drv->pin_index = pin_index;
    drv->direction = MOTOR_STOP;
    drv->pwmDuty = 0;

    GPIO_write(drv->pin_index, GPIO_PIN_LOW);
}

void MotorDriver_SetPWMDutyCycle(MotorDriver *drv, uint32_t dutyPercent)
{
    if (dutyPercent > 100U) dutyPercent = 100U;
    drv->pwmDuty = dutyPercent;
}

void MotorDriver_SetDirection(MotorDriver *drv, MotorDirection dir)
{
    drv->direction = dir;
    elapsedMs = 0;
    pwmState = 0;

    if (dir == MOTOR_STOP) {
        GPIO_write(drv->pin_index, GPIO_PIN_LOW);
    } else if (dir == MOTOR_FORWARD) {
        GPIO_write(drv->pin_index, GPIO_PIN_HIGH);
    } else { /* MOTOR_REVERSE */
        GPIO_write(drv->pin_index, GPIO_PIN_LOW);
    }
}

void MotorDriver_GeneratePWMOutput(MotorDriver *drv)
{
    if (drv->direction == MOTOR_STOP) {
        GPIO_write(drv->pin_index, GPIO_PIN_LOW);
        return;
    }

    if (drv->pwmDuty == 0U) {
        GPIO_write(drv->pin_index, GPIO_PIN_LOW);
        return;
    } else if (drv->pwmDuty == 100U) {
        if (drv->direction == MOTOR_FORWARD) GPIO_write(drv->pin_index, GPIO_PIN_HIGH);
        else GPIO_write(drv->pin_index, GPIO_PIN_LOW);
        return;
    }

    uint32_t highTime = (drv->pwmDuty * PWM_PERIOD_MS) / 100U;
    uint32_t lowTime  = PWM_PERIOD_MS - highTime;

    elapsedMs++;

    if (pwmState == 0) {
        if (elapsedMs >= highTime) {
            GPIO_write(drv->pin_index, GPIO_PIN_LOW);
            pwmState = 1;
            elapsedMs = 0;
        }
    } else {
        if (elapsedMs >= lowTime) {
            if (drv->direction == MOTOR_FORWARD) {
                GPIO_write(drv->pin_index, GPIO_PIN_HIGH);
            } else {
                GPIO_write(drv->pin_index, GPIO_PIN_LOW);
            }
            pwmState = 0;
            elapsedMs = 0;
        }
    }
}
