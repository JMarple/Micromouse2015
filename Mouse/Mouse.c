#include "Mouse.h"

#include "ConfigSerial.h"
#include "ConfigMotors.h"

void InitRobot()
{
	mouse.beginSerial 	= &SerialInitiate;
	mouse.sendString	= &SerialSendChar;
	mouse.sendInteger	= &SerialSendRawInt;
	mouse.beginMotors 	= &InitMotors;
}


