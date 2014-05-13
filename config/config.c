#include "config.h"

void configInit(void)
{
	//Setup Serial Communication
	initSerial(&Config.serial);
	
	initPWM();
	
	TIM3->CCR1 = 50;
	TIM3->CCR2 = 50;
	TIM3->CCR3 = 50;
	TIM3->CCR4 = 50;
}

