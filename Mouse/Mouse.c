#include "Mouse.h"

#include "ConfigSerial.h"
#include "ConfigMotors.h"
#include "ConfigSensors.h"

void InitRobot()
{
	// Initialization
	mouse.beginSerial 	= &SerialInitiate;
	mouse.beginMotors 	= &InitMotors;
	mouse.beginSensors 	= &SensorInit;

	// Serial Communication Controls
	mouse.sendNum		= &SerialSaveNumber;
	mouse.sendChar		= &SerialSaveRawChar;
	mouse.sendString	= &SerialSaveString;
	mouse.sendInteger	= &SerialSaveRawInteger;
	mouse.forceBuffer 	= &SerialForceBuffer;

	mouse.getSensor		= &SensorGetValue;	
}


