
#include "Mouse/Mouse.h"

int main(void)
{
	InitRobot();

	// Mouse is our global variable holding lowerlevel calls
	mouse.beginSerial(9600);
	//mouse.beginMotors();

	//TIM3->CCR1 = 50;
	//TIM3->CCR2 = 50;
	//TIM3->CCR3 = 50;
	//TIM3->CCR4 = 50;
	
	/*mouse.sendString("Hello World");
	mouse.sendString("Testing This");
	mouse.sendString("Still Sending");
	mouse.sendString("Why Hello There!");
	mouse.sendString("lol");
	mouse.sendString("Hi!!!");
	mouse.sendString("\n\r Break Line 8)");
	mouse.sendString("wasabiiii");
	mouse.sendString("1234");

	int i;
	for(i = 0; i < 50; i++)
	{
		mouse.sendString("1234");
	}*/

	mouse.sendInteger(0);

	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);

	mouse.sendBufferRemains();			

	while(1==1);

	return 0;
}