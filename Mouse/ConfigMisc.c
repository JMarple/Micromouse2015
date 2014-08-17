#include "ConfigMisc.h"

static void MiscRCC();
static void MiscGPIO();

void MiscInit()
{
	MiscRCC();
	MiscGPIO();
}

void MiscTurnOnLED(int led)
{
	if(led & FRONT_CENTER_LED)
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	if(led & FRONT_LEFT_LED)
		GPIO_WriteBit(GPIOH, GPIO_Pin_1, Bit_SET);
	if(led & FRONT_RIGHT_LED)
		GPIO_WriteBit(GPIOH, GPIO_Pin_0, Bit_SET);
	if(led & BACK_LEFT_LED)
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);	
	if(led & BACK_CENTER_LED)
		GPIO_WriteBit(GPIOC, GPIO_Pin_11, Bit_SET);
	if(led & BACK_RIGHT_LED)
		GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_SET);	
}


void MiscTurnOffLED(int led)
{
	if(led & FRONT_CENTER_LED)
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
	if(led & FRONT_LEFT_LED)
		GPIO_WriteBit(GPIOH, GPIO_Pin_1, Bit_RESET);
	if(led & FRONT_RIGHT_LED)
		GPIO_WriteBit(GPIOH, GPIO_Pin_0, Bit_RESET);
	if(led & BACK_LEFT_LED)
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);	
	if(led & BACK_CENTER_LED)
		GPIO_WriteBit(GPIOC, GPIO_Pin_11, Bit_RESET);
	if(led & BACK_RIGHT_LED)
		GPIO_WriteBit(GPIOA, GPIO_Pin_12, Bit_RESET);	
}

// Button is connected to PC10
int GetButtonPushed()
{
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);
}

static void MiscRCC()
{
	 /* GPIOC and GPIOB clock enable */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
}

static void MiscGPIO()
{
	GPIO_InitTypeDef GPIO_Struct;

	// All Led's
	GPIO_Struct.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOH, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_Struct);

	// PC10 is button
	GPIO_Struct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_Struct);

	// Set all default to on
	//GPIO_WriteBit(GPIOH, GPIO_Pin_0 | GPIO_Pin_1, Bit_SET);
	//GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_12 | GPIO_Pin_8, Bit_SET);
	//GPIO_WriteBit(GPIOC, GPIO_Pin_11, Bit_SET);

}