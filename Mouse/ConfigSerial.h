#ifndef __CONFIG_SERIAL_H
#define __CONFIG_SERIAL_H

// ConfigSerial.h
// Author: Justin Marple (jmarple@umass.edu)
// Date: 6/30/14
//
// This file implements several functions for serial
//   communication with the STM32F4 processor
// ConfigSerialPeripherals.c implements functions that
//   configure the hardware on the chip. This file 
//   implements a way to interract with the DMA 
//   and efficiently send messages without using 
//   valueable processing time
//
// Basic Usage:
//   SerialInitiate();       -> Initializes DMA Usart on PA9 and PA10
//   SerialSaveString();     -> Inputs a character message into the current buffers
//   SerialSaveRawInteger(); -> Inputs 4 bytes into the current buffer
//   SerialForceBuffer();    -> Forces whatever is in the current buffer to be sent,
//                                even if it isn't full 
//
// Note: 
//   The buffer only sends if it reaches BUFFER_STRING_LENGTH, if you stop calling
//   SerialSave___(...), you should call SerialForceBuffer() to send the remaining
//   bytes in the buffer

#include "stm32f4xx_conf.h"

// Holds the information that needs to be sent
#define BUFFER_STRING_LENGTH 	50
#define BUFFER_SIZE				100
#define BUFFER_EMPTY 			-1

// SerialInitiate
// This will setup a USART stream on our device
void SerialInitiate(int Baud);

// SerialSaveRawChar(...)
// Adds a char to the current buffer for sending data
void SerialSaveRawChar(char data);

// SerialSaveString(...)
// Adds a string to the buffer to be sent
void SerialSaveString(char* message);

// SerialSaveNumber(...)
// Sends only the minimum number of bytes necessary for data to be sent
void SerialSaveNumber(long data);

// SerialSaveRawInteger(...)
// Adds 4 bytes to the buffer to be sent
void SerialSaveRawInteger(int data);

// SerialForceBuffer(...)
// Forces the buffer to send it's content
void SerialForceBuffer();


#endif