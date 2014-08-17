#include "Mouse.h"

#include "ConfigSerial.h"
#include "ConfigMotors.h"
#include "ConfigSensors.h"
#include "ConfigMisc.h"

void InitRobot()
{
	// Initialization
	mouse.beginSerial 	= &SerialInitiate;
	mouse.beginMotors 	= &InitMotors;
	mouse.beginSensors 	= &SensorInit;
	mouse.beginMisc		= &MiscInit;

	// Serial Communication Controls
	mouse.sendNum		= &SerialSaveNumber;
	mouse.sendChar		= &SerialSaveRawChar;
	mouse.sendString	= &SerialSaveString;
	mouse.sendInteger	= &SerialSaveRawInteger;
	mouse.forceBuffer 	= &SerialForceBuffer;

	// Encoder Controls
	mouse.getLeftEncoder 	= &MotorEncoder1;
	mouse.getRightEncoder	= &MotorEncoder2;
	mouse.resetLeftEncoder	= &MotorEncoder1Reset;
	mouse.resetRightEncoder = &MotorEncoder2Reset;

	// Motor Controls
	mouse.setLeftMotor		= &SetMotor1;
	mouse.setRightMotor		= &SetMotor2;

	// Sensors Control
	mouse.getSensor		= &SensorGetValue;	

	// LED Control
	mouse.ledOn			= &MiscTurnOnLED;
	mouse.ledOff		= &MiscTurnOffLED;

	// Button
	mouse.isButtonPushed = &GetButtonPushed;
}


