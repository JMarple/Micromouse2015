
#include "Mouse/Mouse.h"

int main(void)
{
	InitRobot();

	mouse.beginSensors();
	mouse.beginSerial(9600);

	//int num = 256;

	//mouse.sendInteger(num);
	//mouse.sendString("a");
	//mouse.sendNum(0xFFFF);
	//mouse.sendChar('a');

	

  	ADC_SoftwareStartConv(ADC1);
  	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
  	
  	int num = ADC_GetConversionValue(ADC1);
  	mouse.sendInteger(num);
	mouse.forceBuffer();	

	while(1==1)
	{
		//int timerValue = TIM_GetCounter(TIM2);

	}

	return 0;
}