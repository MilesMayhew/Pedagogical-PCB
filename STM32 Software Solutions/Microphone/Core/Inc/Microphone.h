#ifndef MICROPHONE_H
#define MICROPHONE_H

#include "stm32g0xx_hal.h"

#define SAMPLE_RATE     44100      // 44.1 kHz
#define RECORD_TIME     0.2f      // 200 ms
#define SAMPLE_SIZE     1          // 8-bit = 1 bytes
#define BUFFER_LENGTH   (uint32_t)(SAMPLE_RATE * RECORD_TIME)  // ≈882 samples

typedef struct {
    ADC_HandleTypeDef *hadc;
    uint16_t ADCBuffer[BUFFER_LENGTH];
    uint16_t sampleSize;
} Microphone;

void Microphone_Init(Microphone *mic, ADC_HandleTypeDef *hadc);
void Microphone_Start(Microphone *mic);
void Microphone_Transmit(UART_HandleTypeDef *huart, Microphone *mic, uint32_t length);
#endif
