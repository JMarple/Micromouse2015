#include "ConfigSerialPeripherals.h"

// Initiates GPIO USART pins
void SerialGPIO()
{
	GPIO_InitTypeDef GPIO_Struct;

	// Configures GPIOA pins 9 and 10 for Serial AF
	GPIO_Struct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_Struct);

	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);

	// Setups Alternative Function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
}

void SerialUSART(int Baud)
{
	USART_InitTypeDef USART_Struct;

	// Configure USART 1 settings
	USART_Struct.USART_BaudRate = Baud;
	USART_Struct.USART_WordLength = USART_WordLength_8b;
	USART_Struct.USART_StopBits = USART_StopBits_1;
	USART_Struct.USART_Parity = USART_Parity_No;
	USART_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_Struct);

	// Configure the Recieving Interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// Enables USART1
	USART_Cmd(USART1, ENABLE);
}

void SerialNVIC()
{
	NVIC_InitTypeDef NVIC_Struct;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_Struct.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Struct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_Struct);
}

void SerialDMA(volatile char* bufferAddr)
{
	DMA_DeInit(DMA2_Stream7);

	DMA_Struct.DMA_Channel = DMA_Channel_4;
	DMA_Struct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_Struct.DMA_Memory0BaseAddr = (uint32_t)bufferAddr;
	DMA_Struct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	//DMA_Struct.DMA_BufferSize = BUFFER_STRING_LENGTH - 1;	
	DMA_Struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_Struct.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	DMA_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_Struct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_Struct.DMA_Mode = DMA_Mode_Normal;
	DMA_Struct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_Struct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Struct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Struct.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_Struct.DMA_Priority = DMA_Priority_Low;

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

void SerialRCC()
{
	// Turns on Peripheral Clocks for GPIO and USART functions
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
}