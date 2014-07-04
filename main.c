
#include "Mouse/Mouse.h"

int main(void)
{
	InitRobot();

	mouse.beginSensors();
	mouse.beginSerial(9600);
	
	mouse.sendChar('a');
	mouse.forceBuffer();

	while(1==1)
	{
		//int timerValue = TIM_GetCounter(TIM2);

	}

	return 0;
}

