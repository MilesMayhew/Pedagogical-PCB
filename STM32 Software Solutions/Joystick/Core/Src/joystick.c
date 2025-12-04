#include "joystick.h"

// ---------------- Internal State ----------------
static ADC_HandleTypeDef *s_adc = NULL;

static uint16_t s_adcBuf[2];    // DMA buffer: [0] = X, [1] = Y
static Joystick_t s_joy;

// Center and scale depend on ADC resolution
static float s_halfScale   = 2048.0f;
static float s_divisor     = 2048.0f;

// ------------------------------------------------
static float map_to_norm(uint16_t value)
{
    return ((float)value - s_halfScale) / s_halfScale;
}

// ------------------------------------------------
void Joystick_Init(ADC_HandleTypeDef *hadc)
{
    s_adc = hadc;

    // Set scale based on ADC resolution dynamically
    uint32_t res = LL_ADC_GetResolution(hadc->Instance);

    switch (res)
    {
        case LL_ADC_RESOLUTION_12B: s_halfScale = 2048.0f; break;
        case LL_ADC_RESOLUTION_10B: s_halfScale =  512.0f; break;
        case LL_ADC_RESOLUTION_8B:  s_halfScale =  128.0f; break;
        default:                    s_halfScale = 2048.0f; break;
    }

    s_divisor = s_halfScale;

    // Initialize output struct
    s_joy.rawX = 0;
    s_joy.rawY = 0;
    s_joy.normX = 0;
    s_joy.normY = 0;
}

// ------------------------------------------------
void Joystick_Start(void)
{
    if (s_adc == NULL) return;

    // Expect scan mode, 2 channels, continuous DMA
    HAL_ADC_Start_DMA(s_adc, (uint32_t *)s_adcBuf, 2);
}

// ------------------------------------------------
void Joystick_Process(void)
{
    s_joy.rawX = s_adcBuf[0];
    s_joy.rawY = s_adcBuf[1];

    s_joy.normX = map_to_norm(s_joy.rawX);
    s_joy.normY = map_to_norm(s_joy.rawY);
}

// ------------------------------------------------
Joystick_t Joystick_Get(void)
{
    return s_joy;
}
