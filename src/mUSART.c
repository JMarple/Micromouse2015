#include "mUSART.h"
#include <stdlib.h>

void initSerial(USART_TypeDef *num, int baud)
{
	if(num == USART1)
	{
		Serial.usart.gpio = GPIOB;
		Serial.usart.num = USART1;
		Serial.usart.pinTX = GPIO_PinSource6;
		Serial.usart.pinRX = GPIO_PinSource7;
		Serial.usart.baudrate = baud;		
		init_USART(&Serial.usart);
	}
	
	Serial.getChar = &USART_getchar;
	Serial.send = &send_USART;	
}

void USART1_IRQHandler(void)
{	
	if( USART_GetITStatus(USART1, USART_IT_RXNE) )
	{
		char nowChar = USART1->DR;
		//Serial.send(USART1, &nowChar);
		
		pushRecordedCharsOut();
		Serial.usart.recordedChars[0] = nowChar;		
	}
} 
 
char pushRecordedCharsOut(void)
{
	int i;
	//Keep track of the char that is removed
	char endChar = Serial.usart.recordedChars[sizeof(Serial.usart.recordedChars) / sizeof(char)-1];
	for(i = sizeof(Serial.usart.recordedChars) / sizeof(char)-1; i > 0; i--)
	{	
		Serial.usart.recordedChars[i] = Serial.usart.recordedChars[i-1];
	} 
	
	return endChar;
}

char USART_getchar(void)
{
	int i;
	for(i = sizeof(Serial.usart.recordedChars) / sizeof(char)-1; i >= 0; i--)
	{
		if(Serial.usart.recordedChars[i] != 0x00)
		{
			char endChar = Serial.usart.recordedChars[i];
			Serial.usart.recordedChars[i] = 0x00;			
			return endChar;
		}
	}
	
	return 0x00;
}

void DMA2_Stream7_IRQHandler(void)
{
  /* Test on DMA Stream Transfer Complete interrupt */
  if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6))
  {
    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
  }
}

/* 

   send_USART (USART_TypeDef* USARTx, volatile char *s) 

Send a String of information over a specificed port

*/
void send_USART(USART_TypeDef* USARTx, volatile char *s)
{
	/* while(*s)
	{
		while(!(USARTx->SR & 0x00000040) );
		USART_SendData(USARTx, *s);
		*s++;
	}	 */
	
	DMA_InitTypeDef  DMA_InitStructure;

	
    DMA_DeInit(DMA2_Stream7);
  
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Transmit
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)s;
    
    uint16_t size = 0;
   
    while(*s)
    {
		size++;
		*s++;
    }
    
    DMA_InitStructure.DMA_BufferSize = (uint16_t)size;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);
   
    /* Enable the USART Tx DMA request */
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
   
    /* Enable DMA Stream Transfer Complete interrupt */
    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
   
    /* Enable the DMA RX Stream */
    DMA_Cmd(DMA2_Stream7, ENABLE);
}

/* 

   init_USART (mUSARTx *port) 

Given port data, this function will 
initiate USART on selected pins

*/
void init_USART(mUSARTx *port)
{
	//Structures to aid with settings
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* GPIO SETUP */

		//Enable APB1/2 Peripheral clock for USARTx
		if(port->num == USART1)	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		else if(port->num == USART2)
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		else if(port->num == USART3)
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
		else if(port->num == USART6)
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
			
		//Enable Peripheral Clock for GPIO Pins used for usart
		if(port->gpio == GPIOA)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
		else if(port->gpio == GPIOB)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
		else if(port->gpio == GPIOC)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
		
		//Set Pin numbers
		//Since pinTX is numbered 1-15, and GPIO_Pin represents each pin per bit, bit shift using pinTX to get the GPIO_Pin Representation
		//Example: port->pinTX = GPIO_PinSource6 = 0x06.  Use this to bit shift 1 << 0x06 = 0x040 = GPIO_Pin_6
		GPIO_InitStructure.GPIO_Pin = 1 << port->pinTX | 1 << port->pinRX;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//Set to alternate function mode		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//Set IO Speed	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Set pin to push/pull (rather then open drain)		
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//Set pullup resistor		
		GPIO_Init(port->gpio, &GPIO_InitStructure);//Update GPIO registers		
					
		//The pins have been set to their alternative functions (AF)
		// so we need to tell USART to take over the pins
		int gpio_af;
		
		//If USART1, 2 or 3, use Alternate function 7
		if(port->num == USART1 || port->num == USART2 || port->num == USART3)
			gpio_af = 0x07;
		//If USART4, 5 or 6, use Alternate function 8
		else
			gpio_af = 0x08;
		 
		//Tell USART to take over the pins
		GPIO_PinAFConfig(port->gpio, port->pinTX, gpio_af);
		GPIO_PinAFConfig(port->gpio, port->pinRX, gpio_af);
		
		
	
	
	
	/* USART SETUP*/
	
		//Set baudrate
		USART_InitStruct.USART_BaudRate = port->baudrate;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;//Set 8 bits per word
		USART_InitStruct.USART_StopBits = USART_StopBits_1;//Set 1 stop bit
		USART_InitStruct.USART_Parity = USART_Parity_No;//Set no parity
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Set no flow control
		USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//Enable TX and RX only		
		USART_Init(port->num, &USART_InitStruct);//Update USART registers
		
		//Set USART RECIEVE function on
		USART_ITConfig(port->num, USART_IT_RXNE, ENABLE);
		
		//Enables USART
		USART_Cmd(port->num, ENABLE);
		
	/* INTERRUPTS SETUP */		
				
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		//Configure which USART
		if(port->num==USART1)
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		else if(port->num==USART2)
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		else if(port->num==USART3)
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		else if(port->num==USART6)
			NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
			
		//Set the priority of this USART
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		
		//Sets subpriority within group
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		
		//USART interrupts are globally enabled
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		
		//Update NVIC registers
		NVIC_Init(&NVIC_InitStructure); 
	
	
}



