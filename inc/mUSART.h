#include "stm32f4xx_usart.h"
#include "stm32f4xx_conf.h"
#include "misc.h"

#ifndef mUSART_H
#define mUSART_H

typedef struct
{
	volatile char recordedChars[50];
	
	//Pin assignments
	int pinTX;
	int pinRX;
	
	//How fast to transmit data
	int baudrate;

	//GPIO Port (A B or C?)
	GPIO_TypeDef *gpio;
	
	//USART number
	USART_TypeDef *num;
	
} mUSARTx;

typedef struct
{
	//All the seperate usart ports
	mUSARTx usart;
	
	//Function that will look for a char 
	char (*getChar)(void);
	
	//Init USART
	void (*start)(mUSARTx *port);
	
	//Send Data
	void (*send)(USART_TypeDef* USARTx, volatile char *s);
	
}mUSART;

mUSART Serial;

char pushRecordedCharsOut(void);
char USART_getchar(void);
void initSerial(USART_TypeDef *num, int baud);
void init_USART(mUSARTx *port);
void send_USART(USART_TypeDef* USARTx, volatile char *s);

#endif