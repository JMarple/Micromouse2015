
#include "Mouse/Mouse.h"

int main(void)
{
	InitRobot();

	// Mouse is our global variable holding lowerlevel calls
	mouse.beginSerial(230400);
	//mouse.beginMotors();

	//TIM3->CCR1 = 50;
	//TIM3->CCR2 = 50;
	//TIM3->CCR3 = 50;
	//TIM3->CCR4 = 50;
	
	mouse.sendString("Hello World");
	mouse.sendString("Testing This");
	mouse.sendString("Still Sending");
	mouse.sendString("Why Hello There!");
	mouse.sendString("lol");
	mouse.sendBufferRemains();
	mouse.sendString("Hi!!!");
	mouse.sendString("\n\r Break LIne 8)");
	mouse.sendString("wasabiiii");
	mouse.sendBufferRemains();

	while(1==1)
	{
		mouse.sendString("1234");
	}

	return 0;
}