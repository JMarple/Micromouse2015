#include "ConfigMotors.h"

static void MotorGPIO();
static void MotorTimer();
static void MotorRCC();

void InitMotors()
{
	MotorRCC();
	MotorGPIO();
	MotorTimer();
}

static void MotorRCC()
{
	/* TIM3 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 
 	/* GPIOC and GPIOB clock enable */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);
}
static void MotorGPIO()
{
	GPIO_InitTypeDef GPIO_Struct;

	GPIO_Struct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_Struct);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
}

static void MotorTimer()
{
	TIM_TimeBaseInitTypeDef TIM_Struct;
	TIM_OCInitTypeDef TIMOC_Struct;

	uint16_t period = 800;
	uint16_t prescalerValue = 0;
	prescalerValue = (uint16_t) ((SystemCoreClock /2) / 28000000) - 1;

	// Timer Init
	TIM_Struct.TIM_Period = period;
	TIM_Struct.TIM_Prescaler = prescalerValue;
	TIM_Struct.TIM_ClockDivision = 0;
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_Struct);

	// PWM Mode Configuration
	TIMOC_Struct.TIM_OCMode = TIM_OCMode_PWM1;
	TIMOC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	TIMOC_Struct.TIM_Pulse = 0;
	TIMOC_Struct.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &TIMOC_Struct);
	TIM_OC2Init(TIM3, &TIMOC_Struct);
	TIM_OC3Init(TIM3, &TIMOC_Struct);
	TIM_OC4Init(TIM3, &TIMOC_Struct);

	TIM_Cmd(TIM3, ENABLE);
}