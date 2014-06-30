#include "Mouse.h"

#include "ConfigSerial.h"
#include "ConfigMotors.h"

void InitRobot()
{
	// Initialization
	mouse.beginSerial 	= &SerialInitiate;
	mouse.beginMotors 	= &InitMotors;

	// Serial Communication Controls
	mouse.sendNum		= &SerialSaveNumber;
	mouse.sendChar		= &SerialSaveRawChar;
	mouse.sendString	= &SerialSaveString;
	mouse.sendInteger	= &SerialSaveRawInteger;
	mouse.forceBuffer 	= &SerialForceBuffer;

	
}


