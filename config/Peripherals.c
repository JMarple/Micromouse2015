#include "config.h"
#include "Peripherals.h"

/*

    A less efficient way of setting up GPIO, Interrupts and timers 
	for the purpose of making code a bit more cleaner.  This code
	should only be used for initialization when performance is 
	not critical.
	
*/


void GPIO_SetupPin(GPIO_TypeDef* port,
					uint32_t GPIO_Pin,
					GPIOSpeed_TypeDef GPIO_Mode,
					GPIOSpeed_TypeDef GPIO_Speed,
					GPIOSpeed_TypeDef GPIO_OType,
					GPIOSpeed_TypeDef GPIO_PuPd)
{

	GPIO_InitTypeDef GPIO_MyStruct;
	
	GPIO_MyStruct.GPIO_Pin = GPIO_Pin;
	GPIO_MyStruct.GPIO_Mode = GPIO_Mode;
	GPIO_MyStruct.GPIO_Speed = GPIO_Speed;
	GPIO_MyStruct.GPIO_OType = GPIO_OType;
	GPIO_MyStruct.GPIO_PuPd = GPIO_PuPd;
	
	GPIO_Init(port, &GPIO_MyStruct);
}

void NVIC_SetupInterrupt(uint8_t NVIC_IRQChannel, 
						uint8_t NVIC_IRQChannelPreemptionPriority,
						uint8_t NVIC_IRQChannelSubPriority,
						uint8_t NVIC_IRQChannelCmd)
{
	NVIC_InitTypeDef NVIC_MyStruct;
	
	NVIC_MyStruct.NVIC_IRQChannel = NVIC_IRQChannel;
	NVIC_MyStruct.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;
	NVIC_MyStruct.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;
	NVIC_MyStruct.NVIC_IRQChannelCmd = NVIC_IRQChannelCmd;
	
	NVIC_Init(&NVIC_MyStruct);
}

void TIM_SetupTimer(TIM_TypeDef* TIMx, 
					uint16_t TIM_Prescaler,
					uint16_t TIM_CounterMode,
					uint16_t TIM_Period,
					uint16_t TIM_ClockDivision,
					uint16_t TIM_RepetitionCounter)
{

	TIM_TimeBaseInitTypeDef TIM_MyStruct;
	
	TIM_MyStruct.TIM_Prescaler = TIM_Prescaler;
	TIM_MyStruct.TIM_CounterMode = TIM_CounterMode;
	TIM_MyStruct.TIM_Period = TIM_Period;
	TIM_MyStruct.TIM_ClockDivision = TIM_ClockDivision;
	TIM_MyStruct.TIM_RepetitionCounter = TIM_RepetitionCounter;
	
	TIM_TimeBaseInit(TIMx, &TIM_MyStruct);
}

void TIM_SetupOutputCompare(TIM_TypeDef* TIMx,
						void (*init) (TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct),
						uint16_t TIM_OCMode,
						uint16_t TIM_OutputState,
						uint16_t TIM_OutputNState,
						uint32_t TIM_Pulse,
						uint16_t TIM_OCPolarity,    
						uint16_t TIM_OCNPolarity,  
						uint16_t TIM_OCIdleState,  
						uint16_t TIM_OCNIdleState)  
{
	TIM_OCInitTypeDef TIM_MyStruct;
	
	TIM_MyStruct.TIM_OCMode = TIM_OCMode;
	TIM_MyStruct.TIM_OutputState = TIM_OutputState;
	TIM_MyStruct.TIM_OutputNState = TIM_OutputNState;
	TIM_MyStruct.TIM_Pulse = TIM_Pulse;
	TIM_MyStruct.TIM_OCPolarity = TIM_OCPolarity;
	TIM_MyStruct.TIM_OCNPolarity = TIM_OCNPolarity;
	TIM_MyStruct.TIM_OCIdleState =TIM_OCIdleState;
	TIM_MyStruct.TIM_OCNIdleState = TIM_OCNIdleState;
	
	init(TIMx, &TIM_MyStruct);
}

void TIM_SetupOutputCompare1(TIM_TypeDef* TIMx, uint16_t TIM_OCMode, uint16_t TIM_OutputState, uint16_t TIM_OutputNState,uint32_t TIM_Pulse,uint16_t TIM_OCPolarity,    uint16_t TIM_OCNPolarity,  uint16_t TIM_OCIdleState,  uint16_t TIM_OCNIdleState)  
{
	TIM_SetupOutputCompare(TIMx, TIM_OC1Init, TIM_OCMode, TIM_OutputState, TIM_OutputNState, 
						TIM_Pulse, 
						TIM_OCPolarity, TIM_OCNPolarity, TIM_OCIdleState, TIM_OCNIdleState);
}

void TIM_SetupOutputCompare2(TIM_TypeDef* TIMx, uint16_t TIM_OCMode, uint16_t TIM_OutputState, uint16_t TIM_OutputNState,uint32_t TIM_Pulse,uint16_t TIM_OCPolarity,    uint16_t TIM_OCNPolarity,  uint16_t TIM_OCIdleState,  uint16_t TIM_OCNIdleState)  
{
	TIM_SetupOutputCompare(TIMx, TIM_OC2Init, TIM_OCMode, TIM_OutputState, TIM_OutputNState, 
						TIM_Pulse, 
						TIM_OCPolarity, TIM_OCNPolarity, TIM_OCIdleState, TIM_OCNIdleState);
}


void TIM_SetupOutputCompare3(TIM_TypeDef* TIMx, uint16_t TIM_OCMode, uint16_t TIM_OutputState, uint16_t TIM_OutputNState,uint32_t TIM_Pulse,uint16_t TIM_OCPolarity,    uint16_t TIM_OCNPolarity,  uint16_t TIM_OCIdleState,  uint16_t TIM_OCNIdleState)  
{
	TIM_SetupOutputCompare(TIMx, TIM_OC3Init, TIM_OCMode, TIM_OutputState, TIM_OutputNState, 
						TIM_Pulse, 
						TIM_OCPolarity, TIM_OCNPolarity, TIM_OCIdleState, TIM_OCNIdleState);
}


void TIM_SetupOutputCompare4(TIM_TypeDef* TIMx, uint16_t TIM_OCMode, uint16_t TIM_OutputState, uint16_t TIM_OutputNState,uint32_t TIM_Pulse,uint16_t TIM_OCPolarity,    uint16_t TIM_OCNPolarity,  uint16_t TIM_OCIdleState,  uint16_t TIM_OCNIdleState)  
{
	TIM_SetupOutputCompare(TIMx, TIM_OC4Init, TIM_OCMode, TIM_OutputState, TIM_OutputNState, 
						TIM_Pulse, 
						TIM_OCPolarity, TIM_OCNPolarity, TIM_OCIdleState, TIM_OCNIdleState);
}
