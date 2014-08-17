#ifndef __CONFIG_MISC_H
#define __CONFIG_MISC_H

#include "stm32f4xx_conf.h"
#include "SensorDefinitions.h"

void MiscInit();
void MiscTurnOnLED(int led);
void MiscTurnOffLED(int led);
int GetButtonPushed();

#endif
