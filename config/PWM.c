#include "PWM.h"
#include "config.h"

void initPWM(void)
{
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* GPIOC and GPIOB clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* GPIOC Configuration: TIM3 CH1 (PC6) and TIM3 CH2 (PC7) */
	GPIO_SetupPin(GPIOC, 
				GPIO_Pin_6 | GPIO_Pin_7, 
				GPIO_Mode_AF, 
				GPIO_Speed_100MHz, 
				GPIO_OType_PP, 
				GPIO_PuPd_UP);
			
	/* GPIOB Configuration:  TIM3 CH3 (PB0) and TIM3 CH4 (PB1) */
	GPIO_SetupPin(GPIOB,
				GPIO_Pin_0 | GPIO_Pin_1,
				GPIO_Mode_OUT,
				GPIO_Speed_100MHz,
				GPIO_OType_PP,
				GPIO_PuPd_DOWN);
				
	GPIO_WriteBit(GPIOB, GPIO_Pin_0 | GPIO_Pin_1, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
	
	/* Connect TIM3 pins to AF2 */ 
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); 
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); 
	
	/* Time base configuration */
	TIM_SetupTimer(TIM3, 
				(uint16_t) ((SystemCoreClock /2) / 28000000) - 1,//Prescalar
				TIM_CounterMode_Up,
				800, //Period
				TIM_CKD_DIV1,
				0);
				
	/* PWM1 Mode configuration: Channel1 */
	TIM_SetupOutputCompare1(TIM3,
						TIM_OCMode_PWM1,//TIM mode
						TIM_OutputState_Enable,//Output compare state
						0x00,//Not Valid for TIM3
						0,//Pulse Value
						TIM_OCPolarity_High,//Output Polarity
						0x00,0x00,0x00);//Not Valid for TIM3
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	/* PWM1 Mode configuration: Channel2 */
	TIM_SetupOutputCompare2(TIM3,
						TIM_OCMode_PWM1,//TIM mode
						TIM_OutputState_Enable,//Output compare state
						0x00,//Not Valid for TIM3
						0,//Pulse Value
						TIM_OCPolarity_High,//Output Polarity
						0x00,0x00,0x00);//Not Valid for TIM3
						
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	/* PWM1 Mode configuration: Channel3 */
	/* TIM_SetupOutputCompare3(TIM3,
						TIM_OCMode_PWM1,//TIM mode
						TIM_OutputState_Enable,//Output compare state
						0x00,//Not Valid for TIM3
						0,//Pulse Value
						TIM_OCPolarity_High,//Output Polarity
						0x00,0x00,0x00);//Not Valid for TIM3
						
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); */
	
	
	/* PWM1 Mode configuration: Channel4 */
/* 	TIM_SetupOutputCompare4(TIM3,
						TIM_OCMode_PWM1,//TIM mode
						TIM_OutputState_Enable,//Output compare state
						0x00,//Not Valid for TIM3
						0,//Pulse Value
						TIM_OCPolarity_High,//Output Polarity
						0x00,0x00,0x00);//Not Valid for TIM3
						
	TIM_ARRPreloadConfig(TIM3, ENABLE); */
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}


void init_PWM()
{
	
}
