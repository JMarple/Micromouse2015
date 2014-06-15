#ifndef __CONFIG_SERIAL_PERIPH_H
#define __CONFIG_SERIAL_PERIPH_H

#include "stm32f4xx_conf.h"

// Serial___(...)
// Initiates the Serial peripherals
void SerialGPIO();
void SerialUSART(int Baud);
void SerialNVIC();
void SerialDMA(volatile char* bufferAddr);
void SerialRCC();

// Holds the DMA information when wanting to send a message
DMA_InitTypeDef DMA_Struct;

#endif