#include "USART.h"
#include "GPIO.h"
#include "NVIC.h"
#include "TIM.h"
#include "PWM.h"

#ifndef config_h
#define config_h

//USART
#define	USART_GPIO		GPIOB
#define	USART_PORT		USART1
#define USART_PIN_TX	GPIO_PinSource6
#define USART_PIN_RX	GPIO_PinSource7
#define USART_BAUD		9600
#define USART_HANDLER	USART1_IRQHandler

//Update Loop
#define UPDATE_HANDLER	TIM2_IRQHandler	

void configInit(void);


typedef struct
{
	mUSART serial;
	void (*init)(void);
	
} config;

//Global variable holding our system information
config Config;

void SetupMainLoop(void);

#endif