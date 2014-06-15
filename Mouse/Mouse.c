#include "Mouse.h"

#include "ConfigSerial.h"

void SerialSend(char *s);

void InitRobot()
{
	mouse.beginSerial 	= &SerialInitiate;
	mouse.send 			= &SerialSend;
}




void SerialSend(char *s)
{
	while(*s)
	{
		while( !(USART1->SR & 0x00000040) );

		USART_SendData(USART1, *s);
		s++;
	}
}

