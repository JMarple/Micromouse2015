
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
	
	mouse.sendString("Hello World");

	while(1==1)
	{
		
	}

	return 0;
}