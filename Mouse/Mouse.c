#include "Mouse.h"

#include "ConfigSerial.h"
#include "ConfigMotors.h"

void InitRobot()
{
	mouse.beginSerial 	= &SerialInitiate;
	mouse.sendString	= &SerialUpdateEditingBuffer;
	mouse.sendInteger	= &SerialSendRawInt;
	mouse.sendBufferRemains = &SerialSendEditingBuffer;

	mouse.beginMotors 	= &InitMotors;
}


