#ifndef BUTTON_H
#define BUTTON_H

#include "stm32g0xx_hal.h"

typedef struct Button Button;

// Edge trigger type
typedef enum {
    RISING_EDGE,
    FALLING_EDGE,
} EdgeTrig;

// Debounce mode
typedef enum {
    DEBOUNCE_ENABLED,
    DEBOUNCE_DISABLED
} DebounceMode;

// 4-state button FSM
typedef enum {
    BUTTON_IDLE,      // Not pressed
    BUTTON_FALLING,   // Press detected, debounce in progress
    BUTTON_PRESSED,   // Confirmed pressed
    BUTTON_RISING     // Release detected, debounce in progress
} ButtonState;

// Button events for FSM
typedef enum {
    EVENT_NONE,
    EVENT_PRESS,
    EVENT_RELEASE
} ButtonEvent;

// Callback type
typedef void (*ButtonAction)(void);

struct Button {
    GPIO_TypeDef* port;
    uint16_t pin;
    EdgeTrig trigger;
    DebounceMode debounce;
    ButtonState state;
    uint32_t lastChangeTime;
    ButtonAction onPressed;
};

// Core API
void Button_Init(Button *button, GPIO_TypeDef* port, uint16_t pin, EdgeTrig trigger, ButtonAction action);
void Button_PollUpdate(Button *button);       // Polling-specific update
void Button_HandleRisingEdge(GPIO_TypeDef* port, uint16_t pin);

#endif // BUTTON_H
