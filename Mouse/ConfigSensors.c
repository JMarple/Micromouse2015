#include "ConfigSensors.h"

// Functions that shouldn't be used outside of this file
static void SensorTIM();
static void SensorRCC();
static void SensorGPIO();
static void SensorNVIC();
static void SensorADC();

// Stores information about each sensor
typedef struct 
{
	int data;
	uint8_t channel;
	GPIO_TypeDef* port;
	uint16_t pin;
}Sensor;

// Variable for all the sensors
volatile Sensor Sensors[5];

void SensorInit()
{
	int i;
	for(i = 0; i < 5; i++)
		Sensors[i].data = 0;

	// Set all Defaults for the sensor
	Sensors[IR_FRONT_LEFT].channel = ADC_Channel_10;
	Sensors[IR_FRONT_LEFT].port = GPIOB;
	Sensors[IR_FRONT_LEFT].pin = GPIO_Pin_7;

	Sensors[IR_FRONT_RIGHT].channel = ADC_Channel_12;
	Sensors[IR_FRONT_RIGHT].port = GPIOB;
	Sensors[IR_FRONT_RIGHT].pin = GPIO_Pin_6;

	Sensors[IR_SIDE_LEFT].channel = ADC_Channel_11;
	Sensors[IR_SIDE_LEFT].port = GPIOB;
	Sensors[IR_SIDE_LEFT].pin = GPIO_Pin_8;

	Sensors[IR_SIDE_RIGHT].channel = ADC_Channel_13;
	Sensors[IR_SIDE_RIGHT].port = GPIOB;
	Sensors[IR_SIDE_RIGHT].pin = GPIO_Pin_9;

	Sensors[GYRO].channel = ADC_Channel_2;
	Sensors[GYRO].port = GPIOA;
	Sensors[GYRO].pin = GPIO_Pin_2;

	SensorRCC();
	SensorGPIO();
	SensorADC();
	SensorTIM();
	SensorNVIC();
}

// Return the value of each sensor
int SensorGetValue(int sensor)
{
	return Sensors[sensor].data;
}

static void SensorTIM()
{
	// Set the Sensor Timers
	TIM_TimeBaseInitTypeDef TIM_Struct;
	TIM_OCInitTypeDef TIM_OC;

	// Prescalar will determine the speed of the timer counter
	// For instance, 84Mhz / 84 = 1Mhz (or 1us per count)
	TIM_Struct.TIM_Prescaler = 84 - 1;
	
	// Count from zero upwards
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;

	// Auto reload value, this will be when the counter resets back to zero
	// For instance, when the counter reachs 200
	TIM_Struct.TIM_Period = 200 - 1;

	// Initate timer
	TIM_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Struct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_Struct);

	// Have an interrupt thrown when the counter = 0us (the start)
	TIM_OC.TIM_OCMode = TIM_OCMode_Inactive;
	TIM_OC.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OC);	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);

	// Have an interrupt thrown when the counter = 80us
	TIM_OC.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC.TIM_Pulse = 80;
	TIM_OC2Init(TIM4, &TIM_OC);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);

	// Turn on Interrupts
	TIM_ITConfig(TIM4, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

	// Start Timer
	TIM_Cmd(TIM4, ENABLE);
}

static void SensorRCC()
{
	// Turn on perhiperhal clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

}

static void SensorGPIO()
{
	GPIO_InitTypeDef GPIO_Struct;

	// Sensor Emitter Pins
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_Struct);
	GPIO_WriteBit(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9, Bit_RESET);

	// Sensor Reciever pins
	GPIO_Struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_Struct);
}

static void SensorADC()
{
	// Start the ADC
	ADC_InitTypeDef ADC_Struct;
	ADC_DeInit();

	// Set the ADC to do a single conversion
	ADC_Struct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Struct.ADC_Resolution = ADC_Resolution_12b;
	ADC_Struct.ADC_ContinuousConvMode = DISABLE;
	ADC_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_Struct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_Struct.ADC_NbrOfConversion = 1;
	ADC_Struct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_Struct);

	ADC_Cmd(ADC1, ENABLE);	
}

static void SensorNVIC()
{
	// Turn on the TIM4 and ADC Interrupts
	// This allows us to use TIM4_IRQHandler and ADC_IRQHandler
	NVIC_InitTypeDef NVIC_Struct;
	NVIC_Struct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Struct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Struct);

	NVIC_Struct.NVIC_IRQChannel = ADC_IRQn;
	NVIC_Init(&NVIC_Struct);

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
}

volatile int counter = 0;

// TIM4_IRQHandler()
// This is an interrupt that handles all timer events
// This timer is setup to turn on a Sensor pin on CC1, 
//   then turn on an ADC conversion on CC2.  
//   The End-Of-Conversion interrupt will turn off the 
//   sensor pin
void TIM4_IRQHandler()
{
	// If the timer 
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);

		// Write the Sensor pin
		GPIO_WriteBit(Sensors[counter].port, Sensors[counter].pin, Bit_SET);	
	}

	// CC2 : After 80us pass for the sensor to turn on correctly, do a ADC conversion
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);

		// Determine which pin to take the ADC conversion from
		ADC_RegularChannelConfig(ADC1, Sensors[counter].channel, 1, ADC_SampleTime_28Cycles);
		
		// Start the ADC
		ADC_SoftwareStartConv(ADC1);
	}
}

#define ADC_Oversampling 32

volatile int ADC_Counter = 0;
volatile int ADC_PrevValues[ADC_Oversampling+1];

// ADC_IRQHandler
// This is an interrupt that handles all ADC events
// This is setup to have an event called when 
//   there is an End-Of-Conversion (EOC) event occur.
void ADC_IRQHandler()
{
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
	{
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

		if(ADC_Counter < ADC_Oversampling)
		{
			ADC_PrevValues[ADC_Counter] = ADC_GetConversionValue(ADC1);
			ADC_Counter++;
			ADC_SoftwareStartConv(ADC1);
		}
		else
		{
			// Turn off the sensor pin
			GPIO_WriteBit(Sensors[counter].port, Sensors[counter].pin, Bit_RESET);
			ADC_PrevValues[ADC_Oversampling] = ADC_GetConversionValue(ADC1);

			int sum = 0;
			int i;
			for(i = 0; i < ADC_Oversampling+1; i++)
				sum += ADC_PrevValues[i];

			Sensors[counter].data = (sum << 2) / (ADC_Oversampling+1);
			// Save the ADC conversion value (average)
			//Sensors[counter].data = (int)(( sum + ADC_GetConversionValue(ADC1) ) / (ADC_Oversampling + 1) );		

			// Bubble sort, cause simple and for testing purposes
			/*for(i = 0; i < ADC_Oversampling + 1; i++)
			{
				int j;
				for(j = 0; j < ADC_Oversampling; j++)
				{
					if(ADC_PrevValues[j] > ADC_PrevValues[j+1])
					{
						int temp = ADC_PrevValues[j+1];
						ADC_PrevValues[j+1] = ADC_PrevValues[j];
						ADC_PrevValues[j] = temp;
					}
				}
			}

			int median = (int)((ADC_Oversampling+1)/2);
			Sensors[counter].data = ADC_PrevValues[median];*/
			//int ADC_CurValue = ADC_GetConversionValue(ADC1);


			// Change which sensor to use for the next conversion
			counter++;
			if(counter > 4)
				counter = 0;

			ADC_Counter = 0;
		}
	}
}


