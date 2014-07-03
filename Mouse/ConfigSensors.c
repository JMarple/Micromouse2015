#include "ConfigSensors.h"

static void SensorTIM();
static void SensorRCC();
static void SensorGPIO();
static void SensorNVIC();
static void SensorADC();

void SensorInit()
{
	SensorRCC();
	SensorGPIO();
	SensorADC();
	SensorTIM();
	SensorNVIC();
}

TIM_TimeBaseInitTypeDef TIM_Struct;
TIM_OCInitTypeDef TIM_OC;

static void SensorTIM()
{

	TIM_Struct.TIM_Prescaler = 84 - 1;
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Struct.TIM_Period = 200 - 1; // Auto reload value
	TIM_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Struct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_Struct);

	TIM_OC.TIM_OCMode = TIM_OCMode_Inactive;
	TIM_OC.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC.TIM_Pulse = 0;
	TIM_OC1Init(TIM2, &TIM_OC);	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	TIM_OC.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC.TIM_Pulse = 80;
	TIM_OC2Init(TIM2, &TIM_OC);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

	TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
}

static void SensorRCC()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
}

static void SensorGPIO()
{
	GPIO_InitTypeDef GPIO_Struct;

	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_Struct);

	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_Struct);
}

static void SensorADC()
{
	ADC_InitTypeDef ADC_Struct;
	//ADC_CommonInitTypeDef ADC_Common;

	ADC_DeInit();

	ADC_Struct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Struct.ADC_Resolution = ADC_Resolution_12b;
	ADC_Struct.ADC_ContinuousConvMode = ENABLE;
	ADC_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_Struct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_Struct.ADC_NbrOfConversion = 1;
	ADC_Struct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_Struct);

	ADC_Cmd(ADC1, ENABLE);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_144Cycles);
}

static void SensorNVIC()
{
	NVIC_InitTypeDef NVIC_Struct;
	NVIC_Struct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Struct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Struct);
}

volatile int counter = 100;

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		/*TIM_PrescalerConfig(TIM2, counter, TIM_PSCReloadMode_Immediate);

		counter++;
		if(counter > 500)
			counter = 100;*/

		//GPIO_ToggleBits(GPIOA, GPIO_Pin_8);
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);	
	}
	else if(TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
	}
}