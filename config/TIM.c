#include "TIM.h"
#include "config.h"

void TIM_SetupTimer(TIM_TypeDef* TIMx, int prescaler, int period)
{
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = prescaler-1;//Clock speed is 42 MHz, so to get the clock down to 1Mhz, set prescalar to 42-1
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = period-1;//Clock speed i s 1Mhz, set period to 1000-1 to get to 1Khz
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit(TIMx, &timerInitStructure);
	

}