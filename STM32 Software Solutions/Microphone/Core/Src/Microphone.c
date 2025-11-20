#include "Microphone.h"

void Microphone_Init(Microphone *mic, ADC_HandleTypeDef *hadc)
{
    mic->hadc = hadc;
}

/* Start ADC with DMA (8-bit samples) */
void Microphone_Start(Microphone *mic)
{
    HAL_ADC_Start_DMA(mic->hadc, (uint32_t*)mic->ADCBuffer, BUFFER_LENGTH);
}

/* Transmit 8-bit samples directly over UART */
void Microphone_Transmit(UART_HandleTypeDef *huart, Microphone *mic, uint32_t length)
{
    HAL_UART_Transmit(huart, mic->ADCBuffer, length, HAL_MAX_DELAY);
}
