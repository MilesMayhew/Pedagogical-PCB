#include "button.h"

#define BUTTON_DEBOUNCE_TIME_MS 50
#define MAX_BUTTONS 8

static Button* buttonRegistry[MAX_BUTTONS] = {0};

// ----------------- Actions -----------------
static void Action_None(Button *b) { }
static void Action_Press(Button *b) {
    if(b->onPressed) b->onPressed();
}

// ----------------- FSM Table -----------------
typedef struct {
    ButtonState currentState;
    ButtonEvent event;
    ButtonState nextState;
    void (*action)(Button*);
} ButtonTransition;

static const ButtonTransition fsmTable[] = {
    {BUTTON_IDLE,    EVENT_PRESS,   BUTTON_FALLING, Action_None},
    {BUTTON_IDLE,    EVENT_RELEASE, BUTTON_IDLE,    Action_None},

    {BUTTON_FALLING, EVENT_PRESS,   BUTTON_PRESSED, Action_Press},
    {BUTTON_FALLING, EVENT_RELEASE, BUTTON_IDLE,    Action_None},

    {BUTTON_PRESSED, EVENT_RELEASE, BUTTON_RISING,  Action_None},
    {BUTTON_PRESSED, EVENT_PRESS,   BUTTON_PRESSED, Action_None},

    {BUTTON_RISING,  EVENT_PRESS,   BUTTON_PRESSED, Action_Press},
    {BUTTON_RISING,  EVENT_RELEASE, BUTTON_IDLE,    Action_None}
};

// ----------------- Initialization -----------------
void Button_Init(Button *button, GPIO_TypeDef* port, uint16_t pin, EdgeTrig trigger, ButtonAction action) {
    button->port = port;
    button->pin = pin;
    button->trigger = trigger;
    button->debounce = DEBOUNCE_ENABLED;
    button->state = BUTTON_IDLE;
    button->lastChangeTime = 0;
    button->onPressed = action;

    for (int i = 0; i < MAX_BUTTONS; i++) {
        if (buttonRegistry[i] == NULL) {
            buttonRegistry[i] = button;
            break;
        }
    }
}

// ----------------- Event detection -----------------
static ButtonEvent Button_ReadEvent(Button *b, GPIO_PinState pin) {
    if ((b->trigger == RISING_EDGE) && pin == GPIO_PIN_SET)
        return EVENT_PRESS;
    if ((b->trigger == FALLING_EDGE) && pin == GPIO_PIN_RESET)
        return EVENT_PRESS;

    return EVENT_RELEASE;
}

// ----------------- Polling Update -----------------
void Button_PollUpdate(Button *b) {
    uint32_t now = HAL_GetTick();
    GPIO_PinState pin = HAL_GPIO_ReadPin(b->port, b->pin);
    ButtonEvent event = Button_ReadEvent(b, pin);

    for(int i = 0; i < sizeof(fsmTable)/sizeof(fsmTable[0]); i++){
        if(fsmTable[i].currentState == b->state && fsmTable[i].event == event){
            // Debounce check for FALLING/RISING
            if((b->state == BUTTON_FALLING || b->state == BUTTON_RISING) &&
               (now - b->lastChangeTime) < BUTTON_DEBOUNCE_TIME_MS)
                return; // still debouncing

            b->state = fsmTable[i].nextState;
            b->lastChangeTime = now;

            if(fsmTable[i].action)
                fsmTable[i].action(b);

            break;
        }
    }
}
// ----------------- Rising Edge ISR Handler -----------------
void Button_HandleRisingEdge(GPIO_TypeDef* port, uint16_t pin) {
    // Find the button in the registry
    Button *b = NULL;
    for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonRegistry[i] && buttonRegistry[i]->port == port && buttonRegistry[i]->pin == pin){
            b = buttonRegistry[i];
            break;
        }
    }
    if(!b) return; // No button found

    // Only handle rising edge buttons
    if(b->trigger != RISING_EDGE) return;

    // Read actual pin state to confirm rising edge
    GPIO_PinState state = HAL_GPIO_ReadPin(b->port, b->pin);
    if(state != GPIO_PIN_SET) return; // Not actually HIGH, ignore

    uint32_t now = HAL_GetTick();

    // Debounce check only if enabled
    if(b->debounce == DEBOUNCE_ENABLED && (now - b->lastChangeTime) < BUTTON_DEBOUNCE_TIME_MS)
        return;

    // Update FSM state
    b->state = BUTTON_PRESSED;
    b->lastChangeTime = now;

    // Call user callback
    if(b->onPressed)
        b->onPressed();
}

