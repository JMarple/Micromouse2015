
#include "Mouse/Mouse.h"

int main(void)
{
	InitRobot();

	// Mouse is our global variable holding lowerlevel calls
	mouse.beginSerial(921600);

	//mouse.send("AT+UART=115200,1,0");

	//mouse.send("Heartbeat");

	//uint16_t num = 500;

	//USART_SendData(USART1, num);
	//mouse.send(num);

	while(1==1)
	{
		//mouse.send("1234567");
		//mouse.send("8910111213");
	}

	return 0;
}