
#include "Mouse/Mouse.h"

int main(void)
{
	InitRobot();

	mouse.beginSensors();
	mouse.beginMotors();
	mouse.beginSerial(921600);
	mouse.beginMisc();

	mouse.setLeftMotor(0);
	mouse.setRightMotor(0);

	mouse.ledOn(FRONT_LEFT_LED | FRONT_CENTER_LED | FRONT_RIGHT_LED);
	mouse.ledOn(BACK_LEFT_LED | BACK_CENTER_LED | BACK_RIGHT_LED);

	while(!mouse.isButtonPushed() == 1);

	while(1==1)
	{

		mouse.setLeftMotor(-1000);
		mouse.setRightMotor(1000);
		//mouse.sendInteger(num);
		//mouse.sendInteger(0);
		//mouse.sendInteger();
		//mouse.sendInteger(mouse.getRightEncoder());
		//mouse.sendInteger(mouse.getSensor(IR_FRONT_RIGHT));
		//mouse.sendInteger(mouse.getSensor(IR_FRONT_LEFT));
		//mouse.sendInteger(mouse.getSensor(IR_FRONT_RIGHT)); // IR SIDE LEFT
		//mouse.sendInteger(mouse.getSensor(IR_SIDE_LEFT)); // IR FRONT RIGHT
		//mouse.sendInteger(mouse.getSensor(IR_SIDE_RIGHT)); // IR SIDE RIGHT

		int i;
		for(i = 0; i < 160000; i++);
	}

	return 0;
}