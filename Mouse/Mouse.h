// Mouse
// Author: Justin Marple (jmarple@umass.edu)
// Date: 6/8/2014
// 
// This file contains all the major configuration entities
// for the Micromouse robot.  

#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdlib.h>
#include "stm32f4xx_conf.h"

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

	void (*beginMotors)();

	void (*beginSensors)();
} Robot;

// Our Global Mouse Variable
Robot mouse;

// InitMouse(...)
// This function starts allocating space for our Robot 
void InitRobot();

#endif