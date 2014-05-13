#include "stm32f4xx_conf.h"
#include "misc.h"

#ifndef mUSART_H
#define mUSART_H

/* mUSART - Structure for driving a Serial port */
typedef struct
{
	//Function that will look for a char 
	char (*getChar)(void);
	
	//Init USART
	void (*start)(void);
	
	//Send Data
	void (*send)(volatile char *s);
	
}mUSART;

void initSerial(mUSART * Serial);
char pushRecordedCharsOut();
char USART_getchar();
void init_USART();
void send_USART(volatile char *s);

#endif