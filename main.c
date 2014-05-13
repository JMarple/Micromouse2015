#include "stm32f4xx_conf.h"
#include "misc.h"
#include "config.h"

int main(void)
{
	//Setup Configuration Files
	configInit();

	//Turn on continous 1Khz Loop
	SetupMainLoop();
	
	//Turn On LED's on DiscoveryBoard
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	
	GPIO_SetupPin(GPIOD, GPIO_Pin_12 | GPIO_Pin_13, GPIO_Mode_OUT);
	GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13, Bit_RESET);	
	
	while (1)
	{
	  	char getChar = Config.serial.getChar();
		if(getChar != 0x00)
		{
			Config.serial.send(&getChar);
		}   
	} 
}

/*
TIM2 Interrupt
*/
void UPDATE_HANDLER()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);		
	}
}

/*

This will create a 1Khz Update Loop for our robot

*/
void SetupMainLoop(void)
{
	//Enable TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	
	//Set Timer to 1KHz -> 42Mhz/42 = 1Mhz/1000 = 1Khz
	TIM_SetupTimer(TIM2, 42, 1000);	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	
	TIM_Cmd(TIM2, ENABLE);	

	//Create an Interrupt
	NVIC_SetupInterrupt(TIM2_IRQn, 0, 1, ENABLE);	
}

