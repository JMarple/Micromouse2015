#include "USART.h"
#include "config.h"

//USART (definitions in config.h)
// #define USART_GPIO		GPIOB
// #define USART_PORT		USART1
// #define USART_PIN_TX		GPIO_PinSource6
// #define USART_PIN_RX		GPIO_PinSource7
// #define USART_BAUD		9600
// #define USART_HANDLER	USART1_IRQHandler

//Example
// mUSART test;
// initSerial(&test);
// test.send("Hello World\r\n"); 


//USART buffer for recent recieved bytes
static char recordedChars[50];

/* 

   initSerial (USART_TypeDef *num, int baud)

Store basic information about what kind of serial port to open up
then update our system to start using USART via the init_USART()
function

*/
void initSerial(mUSART * Serial)
{
	Serial->send = &send_USART;	 
	Serial->getChar = &USART_getchar;
	init_USART();
}

/* 

   send_USART (USART_TypeDef* USARTx, volatile char *s) 

Send a String of information over a specificed port

*/
void send_USART(volatile char *s)
{
	while(*s)
	{
		while(!(USART_PORT->SR & 0x00000040) );
		USART_SendData(USART_PORT, *s);
		*s++;
	}	
}

/* 

   init_USART (mUSARTx *port) 

Given port data, this function will 
initiate USART on selected pins

*/
void init_USART()
{
	//Structures to aid with settings
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* GPIO SETUP */

		//Enable APB1/2 Peripheral clock for USARTx
		if(USART_PORT == USART1)	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		else if(USART_PORT == USART2)
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		else if(USART_PORT == USART3)
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
		else if(USART_PORT == USART6)
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
			
		//Enable Peripheral Clock for GPIO Pins used for usart
		if(USART_GPIO == GPIOA)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
		else if(USART_GPIO == GPIOB)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
		else if(USART_GPIO == GPIOC)
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);			
		
		//Set Pin numbers
		//Since pinTX is numbered 1-15, and GPIO_Pin represents each pin per bit, bit shift using pinTX to get the GPIO_Pin Representation
		//Example: port->pinTX = GPIO_PinSource6 = 0x06.  Use this to bit shift 1 << 0x06 = 0x040 = GPIO_Pin_6
		GPIO_InitStructure.GPIO_Pin = 1 << USART_PIN_TX | 1 << USART_PIN_RX;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//Set to alternate function mode		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//Set IO Speed	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Set pin to push/pull (rather then open drain)		
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//Set pullup resistor		
		GPIO_Init(USART_GPIO, &GPIO_InitStructure);//Update GPIO registers		
					
		//The pins have been set to their alternative functions (AF)
		// so we need to tell USART to take over the pins
		int gpio_af;
		
		//If USART1, 2 or 3, use Alternate function 7
		if(USART_PORT == USART1 || USART_PORT == USART2 || USART_PORT == USART3)
			gpio_af = 0x07;
		//If USART4, 5 or 6, use Alternate function 8
		else
			gpio_af = 0x08;
		 
		//Tell USART to take over the pins
		GPIO_PinAFConfig(USART_GPIO, USART_PIN_TX, gpio_af);
		GPIO_PinAFConfig(USART_GPIO, USART_PIN_RX, gpio_af);	
	
	/* USART SETUP*/
	
		//Set baudrate
		USART_InitStruct.USART_BaudRate = USART_BAUD;//Set 8 bits per word
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;//Set 1 stop bit
		USART_InitStruct.USART_StopBits = USART_StopBits_1;//Set no parity
		USART_InitStruct.USART_Parity = USART_Parity_No;//Set no flow control
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Enable TX and RX only	
		USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//Update USART registers
		USART_Init(USART_PORT, &USART_InitStruct);
		
		//Set USART RECIEVE function on
		USART_ITConfig(USART_PORT, USART_IT_RXNE, ENABLE);
		
		//Enables USART
		USART_Cmd(USART_PORT, ENABLE);
		
	/* INTERRUPTS SETUP */		
				
		//Configure which USART
		if(USART_PORT==USART1)
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		else if(USART_PORT==USART2)
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		else if(USART_PORT==USART3)
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		else if(USART_PORT==USART6)
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

/* 

	This is an interrupt defined in config.h that handles
	incoming bytes of information and stores them into
	recordedChars

*/
void USART_HANDLER(void)
{
	if( USART_GetITStatus(USART_PORT, USART_IT_RXNE))
	{
		//Shift everything in the array by one
		pushRecordedCharsOut();
		
		//Update Array
		recordedChars[0] = USART1->DR; 
	}
}

/* 

	Shift all contents of recordedChars[] over one spot and 
	return the char that was pushed out 
   
*/
char pushRecordedCharsOut()
{
	int i;
	//Keep track of the char that is removed
	char endChar = recordedChars[sizeof(recordedChars) / sizeof(char)-1];
	
	//Shift all contents
	for(i = sizeof(recordedChars) / sizeof(char)-1; i > 0; i--)		
		recordedChars[i] = recordedChars[i-1];
	
	return endChar;
}

/* 

Look through the contents and look for the first char that isn't NULL 

*/
char USART_getchar()
{
	int i;
	//Search through all contents
	for(i = sizeof(recordedChars) / sizeof(char)-1; i >= 0; i--)
	{
		//Is the char not null?
		if(recordedChars[i] != 0x00)
		{
			//Save char info, delete it from array and return then info
			char endChar = recordedChars[i];
			recordedChars[i] = 0x00;			
			return endChar;
		}
	}
	
	return 0x00;
}


