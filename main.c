
#include "Mouse/Mouse.h"

int main(void)
{
	InitRobot();

	// Mouse is our global variable holding lowerlevel calls
	mouse.beginSerial(9600);

	int num = 256;

	mouse.sendInteger(num);
	mouse.sendString("a");
	mouse.sendNum(0xFFFF);
	mouse.sendChar('a');

	mouse.forceBuffer();			

	while(1==1);

	return 0;
}