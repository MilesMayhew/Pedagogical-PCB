#include <Keypad.h>

void Keypad_Init(
    Keypad *keypad,
    GPIO_TypeDef *rowPorts[ROWS],
    uint16_t rowPins[ROWS],
    GPIO_TypeDef *colPorts[COLS],
    uint16_t colPins[COLS]
	) {

    // Store configuration in handle
    for (int i = 0; i < ROWS; i++) {
        keypad->rowPorts[i] = rowPorts[i];
        keypad->rowPins[i]  = rowPins[i];
    }
    for (int j = 0; j < COLS; j++) {
        keypad->colPorts[j] = colPorts[j];
        keypad->colPins[j]  = colPins[j];
    }
}

char Keypad_Scan(Keypad *keypad)
{
    for (int col = 0; col < COLS; col++) {
        // Set all columns HIGH first
        for (int c = 0; c < COLS; c++) {
            HAL_GPIO_WritePin(keypad->colPorts[c], keypad->colPins[c], GPIO_PIN_SET);
        }

        // Set the current column LOW
        HAL_GPIO_WritePin(keypad->colPorts[col], keypad->colPins[col], GPIO_PIN_RESET);

        // Delay to allow signals to settle (debounce)
        HAL_Delay(5);

        // Read each row
        for (int row = 0; row < ROWS; row++) {
            GPIO_PinState state = HAL_GPIO_ReadPin(keypad->rowPorts[row], keypad->rowPins[row]);

            // Check if the key is pressed
            if (state == GPIO_PIN_RESET) {
                // Wait until the key is released (simple debounce)
                //while (HAL_GPIO_ReadPin(keypad->rowPorts[row], keypad->rowPins[row]) == GPIO_PIN_SET);

                // Return corresponding character
                return KEYPAD_MAP[row][col];
            }
        }
    }

    // No key is pressed
    return '\0';
}


