#ifndef __CONFIG_MOTORS_H
#define __CONFIG_MOTORS_H

// ConfigSensors.h
// Author: Justin Marple (jmarple@umass.edu)
// Date: 7/5/14
//
// This file implements the PWM motor timers and 
// the quadrature encoders timers.  
// TIM2 and TIM5 are used for the encoders, as they are
//   32 bit resolution.  They are setup using the 
//   "TIM_EncoderInterfaceConfig(...)" function, so no
//   interrupts or software is needed for counting the
//   encoders.  
// The Motors used PWM which is a form of output compare

#include "stm32f4xx_conf.h"

#define ENC_DEFAULT_VALUE 0x100

// InitMotors(...)
// Start Motor PWM and Encoders
void InitMotors();

// MotorEncoder_(...)
// Returns the current motor count
int MotorEncoder1();
int MotorEncoder2();

// SetMotor_(...)
// Sets the PWM of the motor, ranges from 0 to 1000
void SetMotor1(int value);
void SetMotor2(int value);

// MotorEncoder_Reset(...)
// Reset the encoder timer to a default value
void MotorEncoder1Reset();
void MotorEncoder2Reset();
#endif