#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32g0xx_hal.h"

#define ROWS 4
#define COLS 3

// Keypad layout
static const char KEYPAD_MAP[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

// Keypad handle structure
typedef struct {
    GPIO_TypeDef *rowPorts[ROWS];
    uint16_t      rowPins[ROWS];
    GPIO_TypeDef *colPorts[COLS];
    uint16_t      colPins[COLS];
} Keypad;

// Initialize the keypad GPIO directions only
void Keypad_Init(
    Keypad *keypad,
    GPIO_TypeDef *rowPorts[ROWS],
    uint16_t rowPins[ROWS],
    GPIO_TypeDef *colPorts[COLS],
    uint16_t colPins[COLS]
);

char Keypad_Scan(Keypad *keypad);

#endif // KEYPAD_H
