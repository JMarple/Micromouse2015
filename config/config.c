#include "config.h"

void configInit(void)
{
	//Setup Serial Communication
	initSerial(&Config.serial);
	
	initPWM();
	
	TIM3->CCR1 = 700;
	TIM3->CCR2 = 700;
	TIM3->CCR3 = 700;
	TIM3->CCR4 = 100;
}

