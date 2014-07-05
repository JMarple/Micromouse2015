#ifndef __MOUSE_H
#define __MOUSE_H

// Mouse
// Author: Justin Marple (jmarple@umass.edu)
// Date: 6/8/2014
// 
// This file contains all the major configuration entities
//   for the Micromouse robot.  

#include <stdlib.h>
#include "stm32f4xx_conf.h"
#include "SensorDefinitions.h"

// Mouse Structure
// Contains lowerlevel function pointers to things like GPIO pins
typedef struct
{
	// beginSerial(...)
	// Sets up peripherals to use a serial port
	void (*beginSerial)(int baud);

	// send(...)
	// Sends a string message to the serial port
	//   Note: Requires .beginSerial(#) to be called
	void (*sendString)(char *s);
	void (*sendChar)(char num);
	void (*sendNum)(long data);
	void (*sendInteger)(int num);
	void (*forceBuffer)();

	// beginMotors(...)
	// Sets up the PWM modules for the motors
	void (*beginMotors)();

	// get__Encoder(...)
	// Gets the current encoder value 
	int (*getLeftEncoder)();
	int (*getRightEncoder)();

	// reset__Encoder(...)
	// Resets the current encoder
	void (*resetLeftEncoder)();
	void (*resetRightEncoder)();

	// beginSensors(...)
	// Turns on the sensor update loop
	void (*beginSensors)();

	// getSensor(...)
	// Gets the last recorded value of each sensor
	int (*getSensor)(int sensor);

} Robot;

// Our Global Mouse Variable
Robot mouse;

// InitMouse(...)
// This function starts allocating space for our Robot 
void InitRobot();

#endif