#ifndef PERIPHERALS_H
#define PERIPHERALS_H

void GPIO_SetupPin(GPIO_TypeDef* port,
					uint32_t GPIO_Pin,
					GPIOSpeed_TypeDef GPIO_Mode,
					GPIOSpeed_TypeDef GPIO_Speed,
					GPIOSpeed_TypeDef GPIO_OType,
					GPIOSpeed_TypeDef GPIO_PuPd);
					
void NVIC_SetupInterrupt(uint8_t NVIC_IRQChannel, 
					uint8_t NVIC_IRQCHannelPreemptionPriority,
					uint8_t NVIC_IRQChannelSubPriority,
					uint8_t NVIC_IRQChannelCmd);
						
void TIM_SetupTimer(TIM_TypeDef* TIMx, 
					uint16_t TIM_Prescaler,
					uint16_t TIM_CounterMode,
					uint16_t TIM_Period,
					uint16_t TIM_ClockDivision,
					uint16_t TIM_RepetitionCounter);
					
void TIM_SetupOutputCompare(TIM_TypeDef* TIMx,
						void (*init) (TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct),
						uint16_t TIM_OCMode,
						uint16_t TIM_OutputState,
						uint16_t TIM_OutputNState,
						uint32_t TIM_Pulse,
						uint16_t TIM_OCPolarity,    
						uint16_t TIM_OCNPolarity,  
						uint16_t TIM_OCIdleState,  
						uint16_t TIM_OCNIdleState);
						
void TIM_SetupOutputCompare1(TIM_TypeDef* TIMx,						
						uint16_t TIM_OCMode,
						uint16_t TIM_OutputState,
						uint16_t TIM_OutputNState,
						uint32_t TIM_Pulse,
						uint16_t TIM_OCPolarity,    
						uint16_t TIM_OCNPolarity,  
						uint16_t TIM_OCIdleState,  
						uint16_t TIM_OCNIdleState);

void TIM_SetupOutputCompare2(TIM_TypeDef* TIMx,						
						uint16_t TIM_OCMode,
						uint16_t TIM_OutputState,
						uint16_t TIM_OutputNState,
						uint32_t TIM_Pulse,
						uint16_t TIM_OCPolarity,    
						uint16_t TIM_OCNPolarity,  
						uint16_t TIM_OCIdleState,  
						uint16_t TIM_OCNIdleState);

void TIM_SetupOutputCompare3(TIM_TypeDef* TIMx,						
						uint16_t TIM_OCMode,
						uint16_t TIM_OutputState,
						uint16_t TIM_OutputNState,
						uint32_t TIM_Pulse,
						uint16_t TIM_OCPolarity,    
						uint16_t TIM_OCNPolarity,  
						uint16_t TIM_OCIdleState,  
						uint16_t TIM_OCNIdleState);

void TIM_SetupOutputCompare4(TIM_TypeDef* TIMx,						
						uint16_t TIM_OCMode,
						uint16_t TIM_OutputState,
						uint16_t TIM_OutputNState,
						uint32_t TIM_Pulse,
						uint16_t TIM_OCPolarity,    
						uint16_t TIM_OCNPolarity,  
						uint16_t TIM_OCIdleState,  
						uint16_t TIM_OCNIdleState);

#endif