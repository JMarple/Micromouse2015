
#include "Mouse/Mouse.h"

int main(void)
{
	InitRobot();

	mouse.beginSensors();
	mouse.beginSerial(921600);

	mouse.sendChar('a');
	mouse.forceBuffer();

	while(1==1)
	{
		mouse.sendInteger(mouse.getSensor(IR_FRONT_LEFT));
		mouse.sendInteger(mouse.getSensor(IR_FRONT_RIGHT));
		mouse.sendInteger(mouse.getSensor(IR_SIDE_LEFT));
		mouse.sendInteger(mouse.getSensor(IR_SIDE_RIGHT));

		int i;
		for(i = 0; i < 160000; i++);
		//int timerValue = TIM_GetCounter(TIM2);

	}

	return 0;
}

