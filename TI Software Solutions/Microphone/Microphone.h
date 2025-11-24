// Microphone.h - small driver header


#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <stdint.h>
#include <stddef.h>
#include "ti_msp_dl_config.h" /* for ADC12_0_INST, UART_0_INST, etc. */

/* adjust types: using 16-bit containers for up to 12-bit ADC */
#ifndef BUFFER_LENGTH
#define BUFFER_LENGTH 1024
#endif

typedef struct {
    void *adcInst;    /* ADC instance (ADC12_0_INST) */
    void *uartInst;   /* UART instance (UART_0_INST) */
    uint8_t dmaChannel;/* DMA channel used for ADC->Mem */
} Microphone;

/* Public driver instance (one microphone) */
extern Microphone gMicrophone;

/* ADC buffer (ping-pong) - store as 16-bit to hold up to 12-bit ADC results */
extern uint16_t gMicADCBuffer[BUFFER_LENGTH];

/* API */
void Microphone_Init(Microphone *mic, void *adcInst, void *uartInst, uint8_t dmaChannel);
int  Microphone_Start(Microphone *mic, uint16_t *buffer, size_t length);

/* Called from IRQ/DM A callbacks (implemented here) */
void Microphone_DMA_HalfCompleteCallback(void);
void Microphone_DMA_FullCompleteCallback(void);

#endif /* MICROPHONE_H */
