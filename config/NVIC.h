#include "stm32f4xx_conf.h"

#ifndef NVIC_H
#define NVIC_H

void NVIC_SetupInterrupt(int channel, int prePrio, int subprio, int cmd);

#endif