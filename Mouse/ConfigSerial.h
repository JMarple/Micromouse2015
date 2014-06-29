#ifndef __CONFIG_SERIAL_H
#define __CONFIG_SERIAL_H

#include "stm32f4xx_conf.h"

// Holds the information that needs to be sent
#define BUFFER_STRING_LENGTH 	50
#define BUFFER_SIZE				100
#define BUFFER_EMPTY 			-1

// SerialInitiate
// This will setup a USART stream on our device
void SerialInitiate(int Baud);

void SerialSendChar(char* message);
void SerialSendRawInt(int number);

#endif