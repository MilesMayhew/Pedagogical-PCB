#include <MotorDriver.h>  // Include header for GPIO, TIM configurations, etc.

#define PWM_FREQUENCY     10  // Set your desired PWM frequency in Hz
#define PWM_PERIOD_MS     (1000 / PWM_FREQUENCY)

static uint32_t currentDutyCycle = 0; // Variable to store the current duty cycle
static uint32_t elapsedTime = 0;  // Time elapsed in ms
static uint8_t pwmState = 0;

static MotorDirection currentDirection = MOTOR_STOP; // Variable to store the current direction

// Initialize the motor driver
void MotorDriver_Init(MotorDriver* driver, TIM_HandleTypeDef* timer, uint32_t pwmChannel,
                      GPIO_TypeDef* pin1Port, uint16_t pin1,
                      GPIO_TypeDef* pin2Port, uint16_t pin2) {
    driver->timer = timer;
    driver->pwmChannel = pwmChannel;
    driver->pin1Port = pin1Port;
    driver->pin1 = pin1;
    driver->pin2Port = pin2Port;
    driver->pin2 = pin2;

    // Initialize both pins low
    HAL_GPIO_WritePin(pin1Port, pin1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(pin2Port, pin2, GPIO_PIN_RESET);
}

// Set PWM duty cycle
void MotorDriver_SetPWMDutyCycle(MotorDriver* driver, uint32_t dutyCycle) {
    if (dutyCycle <= 100) {
        currentDutyCycle = dutyCycle;
    }
}

// Set direction (use enum for clarity)
void MotorDriver_SetDirection(MotorDriver* driver, MotorDirection direction) {
    currentDirection = direction;

    // Clear both pins before changing states
    HAL_GPIO_WritePin(driver->pin1Port, driver->pin1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(driver->pin2Port, driver->pin2, GPIO_PIN_RESET);

    switch (direction) {
        case MOTOR_FORWARD:
            HAL_GPIO_WritePin(driver->pin1Port, driver->pin1, GPIO_PIN_SET); // Enable pin1
            break;
        case MOTOR_REVERSE:
            HAL_GPIO_WritePin(driver->pin2Port, driver->pin2, GPIO_PIN_SET); // Enable pin2
            break;
        case MOTOR_STOP:
        default:
            // Both are already set to LOW
            break;
    }
}

// Generate PWM signal in software
void MotorDriver_GeneratePWMOutput(MotorDriver* driver) {
    if (currentDirection == MOTOR_STOP) {
        HAL_GPIO_WritePin(driver->pin1Port, driver->pin1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(driver->pin2Port, driver->pin2, GPIO_PIN_RESET);
        return;
    }

    elapsedTime++;

    uint32_t highTime = (currentDutyCycle * PWM_PERIOD_MS) / 100;
    uint32_t lowTime  = PWM_PERIOD_MS - highTime;

    if (pwmState == 0) {
        // Currently HIGH
        if (elapsedTime >= highTime) {
            HAL_GPIO_WritePin(driver->pin1Port, driver->pin1, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(driver->pin2Port, driver->pin2, GPIO_PIN_RESET);
            pwmState = 1;
            elapsedTime = 0;
        }
    } else {
        // Currently LOW
        if (elapsedTime >= lowTime) {
            if (currentDirection == MOTOR_FORWARD) {
                HAL_GPIO_WritePin(driver->pin1Port, driver->pin1, GPIO_PIN_SET);
                HAL_GPIO_WritePin(driver->pin2Port, driver->pin2, GPIO_PIN_RESET);
            } else if (currentDirection == MOTOR_REVERSE) {
                HAL_GPIO_WritePin(driver->pin1Port, driver->pin1, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(driver->pin2Port, driver->pin2, GPIO_PIN_SET);
            }
            pwmState = 0;
            elapsedTime = 0;
        }
    }
}

