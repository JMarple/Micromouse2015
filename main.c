#include "stm32f4xx_conf.h"
#include "misc.h"
#include "RobotManager/mUSART.h"

int main(void)
{
     //Setup Serial communication
	initSerial(USART1, 9600);
	Serial.send(USART1, "Init Complete! HelloWorld\r\n");
	
	while (1)
	{
	 	char getChar = Serial.getChar();
		if(getChar != 0x00)
		{
			Serial.send(USART1, &getChar);
		} 
	} 
}

