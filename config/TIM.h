#include "stm32f4xx_conf.h"

#ifndef TIM_H
#define TIM_H

void TIM_SetupTimer(TIM_TypeDef* TIMx, int prescaler, int period);

#endif