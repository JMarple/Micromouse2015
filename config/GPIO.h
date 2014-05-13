#include "stm32f4xx_conf.h"

#ifndef GPIO_H
#define GPIO_H

void GPIO_SetupPin(GPIO_TypeDef* port, int pin, int mode);
void GPIO_SetupPinDetail(GPIO_TypeDef* port, int pin, int mode, int pupd, int otype);

#endif