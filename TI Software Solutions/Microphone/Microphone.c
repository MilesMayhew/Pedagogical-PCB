// Microphone.c


#include "Microphone.h"
#include "ti_msp_dl_config.h"
#include "dl_adc12.h"
#include "dl_dma.h"
#include "dl_uart.h"
#include "dl_timer.h"
#include <string.h>

/* Global instance & buffer */
Microphone gMicrophone;
uint16_t gMicADCBuffer[BUFFER_LENGTH];

/* Local bookkeeping */
static uint8_t sUartTxDmaChannel = 1; /* pick a free DMA channel for UART TX (adjust per board) */

void Microphone_Init(Microphone *mic, void *adcInst, void *uartInst, uint8_t dmaChannel)
{
    mic->adcInst = adcInst;
    mic->uartInst = uartInst;
    mic->dmaChannel = dmaChannel;

    /* Initialize driver DMA system (if not already). */
    DL_DMA_init(); /* safe to call multiple times in many SDKs */
}

/* Start ADC in DMA circular mode and enable DMA callbacks.
 * Returns 0 on success, negative on failure.
 */
int Microphone_Start(Microphone *mic, uint16_t *buffer, size_t length)
{
    DL_Status stat;

    /* Configure DMA: peripheral = ADC result register, memory = buffer */
    DL_DMA_ChannelConfig chCfg;
    DL_DMA_ChannelConfig_initDefault(&chCfg);

    /* NOTE: adapt these register addresses to the SDK macros if different */
    chCfg.channel = mic->dmaChannel;
    chCfg.periphAddr = (uint32_t)&(ADC12_0_INST->RESULT); /* adjust if your ADC register name differs */
    chCfg.memAddr = (uint32_t)buffer;
    chCfg.dataSize = DL_DMA_DATA_SIZE_HALF_WORD; /* 16-bit */
    chCfg.blockSize = length;
    chCfg.circular = true;

    /* Setup callbacks */
    chCfg.halfCompleteCallback = Microphone_DMA_HalfCompleteCallback;
    chCfg.fullCompleteCallback = Microphone_DMA_FullCompleteCallback;

    stat = DL_DMA_configChannel(&chCfg);
    if (stat != DL_SUCCESS) {
        return -1;
    }

    /* Enable and start DMA channel */
    DL_DMA_enableChannel(chCfg.channel);
    DL_DMA_startChannel(chCfg.channel);

    /* Ensure ADC is configured for external trigger (timer) — SysConfig should have done this */
    /* Start ADC conversions enable (already done in ti_msp_dl_config.c), we ensure it's enabled */
    DL_ADC12_enableConversions(ADC12_0_INST);

    return 0;
}

/* Helper to send half-buffer over UART using DMA for TX (non-blocking).
 * This uses a separate DMA channel (sUartTxDmaChannel). If your platform doesn't
 * support UART TX via DMA wrapper used here, fall back to non-blocking UART send.
 */
static void transmit_half_buffer_dma(uint16_t *buf, size_t halfLen)
{
    /* Start UART TX DMA - pseudo call; replace with your SDK's exact API */
    DL_UART_startTxDMA(UART_0_INST, (uint8_t*)buf, halfLen * sizeof(uint16_t), sUartTxDmaChannel);
}

/* DMA callbacks invoked when half/full parts are filled by ADC DMA */
void Microphone_DMA_HalfCompleteCallback(void)
{
    /* Transmit first half (0 .. length/2 -1) */
    transmit_half_buffer_dma(&gMicADCBuffer[0], BUFFER_LENGTH/2);
}

void Microphone_DMA_FullCompleteCallback(void)
{
    /* Transmit second half (length/2 .. length-1) */
    transmit_half_buffer_dma(&gMicADCBuffer[BUFFER_LENGTH/2], BUFFER_LENGTH/2);
}
