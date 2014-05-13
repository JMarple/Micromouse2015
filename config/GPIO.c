#include "GPIO.h"
#include "config.h"

void GPIO_SetupPin(GPIO_TypeDef* port, int pin, int mode)
{
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = pin;
	gpio.GPIO_Mode = mode;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(port, &gpio);
} 

void GPIO_SetupPinDetail(GPIO_TypeDef* port, int pin, int mode, int pupd, int otype)
{
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = pin;
	gpio.GPIO_Mode = mode;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(port, &gpio);
} 