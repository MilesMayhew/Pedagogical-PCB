#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/dl_gpio.h>
#include "uart_mspm0.h"
#include "Keypad_mspm0.h"

int main(void)
{
    SYSCFG_DL_init();
    uart_init(115200);
    Keypad_Init();

    uart_print("Keypad ready\r\n");

    while (1) {
        char k = Keypad_Scan();
        if (k != '\0') {
            char s[16];
            snprintf(s, sizeof(s), "%c\r\n", k);
            uart_print(s);

            for (volatile int i = 0; i < 60000; i++) { __asm__("nop"); }
        }
    }
}
