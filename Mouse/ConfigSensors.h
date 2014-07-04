#ifndef __CONFIG_SENSORS_H
#define __CONFIG_SENSORS_H

#include "stm32f4xx_conf.h"

#define IR_FRONT_LEFT		0
#define IR_FRONT_RIGHT 		1
#define IR_SIDE_LEFT		2
#define IR_SIDE_RIGHT		3
#define GYRO				4

void SensorInit();
int SensorGetValue(int sensor);

#endif