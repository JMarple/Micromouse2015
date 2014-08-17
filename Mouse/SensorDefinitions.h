#ifndef __SENSOR_DEFINITIONS_H
#define __SENSOR_DEFINITIONS_H

// ConfigDefinitions.h
// Author: Justin Marple (jmarple@umass.edu)
// Date: 7/4/14
// 
// This file simply includes some global definitions
//   for our sensors

#include "stm32f4xx_conf.h"

#define IR_FRONT_LEFT		0x00
#define IR_FRONT_RIGHT 		0x01
#define IR_SIDE_LEFT		0x02
#define IR_SIDE_RIGHT		0x03
#define GYRO				0x04

#define FRONT_CENTER_LED	0x01
#define FRONT_RIGHT_LED		0x02
#define FRONT_LEFT_LED		0x04
#define BACK_LEFT_LED		0x08
#define BACK_CENTER_LED		0x10
#define BACK_RIGHT_LED		0x20

#endif