#include "ConfigSerial.h"
#include "ConfigSerialPeripherals.h"

typedef struct SerialBuffer
{
	char data[BUFFER_STRING_LENGTH];

	int dataLength;
} SerialBuffer;

// Copy String To Buffer
static int copyStringToBuffer(volatile SerialBuffer* Buffer, char* data);
static void startSerialDMA();

volatile SerialBuffer Buffer[BUFFER_SIZE];
volatile SerialBuffer* BufferList[BUFFER_SIZE];
volatile int BufferPosition;

// The main entry point for the Serial initiation
void SerialInitiate(int Baud)
{
	// Sets all buffers to empty
	int i;
	for(i = 0; i < BUFFER_SIZE; i++)	
	{
		BufferList[i] = 0;
		Buffer[i].dataLength = BUFFER_EMPTY;	
	}

	BufferPosition = -1;

	SerialRCC();
	SerialGPIO();
	SerialUSART(Baud);
	SerialNVIC();
	SerialDMA();

	send("Hello!\n\r");
	send("Second Line!:");
	send("And third!\n\r");

	while(1==1)
	{
		send("TestLineA\n\r");
		send("TestLineB\n\r");
		send("TestLineC\n\r");
		send("TestLineD\n\r");
		int j;
		for(j = 0; j < 168000; j++);
	}
}

void send(char* message)
{
	// Finds a free buffer to use to store our message
	int i;
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		if(Buffer[i].dataLength == BUFFER_EMPTY)
		{
			copyStringToBuffer(&Buffer[i], message);
			break;
		}
	}

	if(i < BUFFER_SIZE)
	{

		// If the list is null (no pointers)
		if(BufferPosition == -1)
		{
			BufferPosition++;

			// Assign first pointer to the target buffer
			BufferList[0] = &Buffer[i];
			startSerialDMA();
		}
		else
		{			
			BufferPosition++;
			BufferList[BufferPosition] = &Buffer[i];
				/*int j;
				for(j = 0; j < BUFFER_SIZE; j++)
				{
					if(BufferList[j] == 0)
					{
						BufferList[j] = &Buffer[i];
						break;
					}
				}*/
			
		}
	}
	

	/*if(Buffer[0].dataLength == BUFFER_EMPTY)
	{
		copyStringToBuffer(&Buffer[0], message);
		startSerialDMA();
	}
	else
	{
		int i;
		for(i = 0; i < BUFFER_SIZE; i++)
		{
			if(Buffer[i].dataLength == BUFFER_EMPTY)
			{
				copyStringToBuffer(&Buffer[i], message);
				break;
			}
		}
	}*/
}

static void startSerialDMA()
{
	DMA_DeInit(DMA2_Stream7);

	DMA_Struct.DMA_BufferSize = BufferList[0]->dataLength;
	DMA_Struct.DMA_Memory0BaseAddr = (uint32_t)BufferList[0];

	DMA_Init(DMA2_Stream7, &DMA_Struct);

	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA2_Stream7, ENABLE); 
}

int count = 0;

static int copyStringToBuffer(volatile SerialBuffer* Buffer, char* data)
{
	int i;

	for(i = 0; i < BUFFER_STRING_LENGTH; i++)
		Buffer->data[i] = 0;

	i = 0;
	while(*data)
	{
		Buffer->data[i] = *data;
		i++;
		data++;
	}

	Buffer->dataLength = i;

	return i;
}

// DMA Completion Interrupt
// This is called when the DMA has completed transmitting it's current message 
void DMA2_Stream7_IRQHandler(void)
{
	// DMA 2 Stream 7 Completion 
	if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
	{		
		if(BufferPosition > 0)
		{
			BufferList[0]->dataLength = BUFFER_EMPTY;
			int i;
			for(i = 1; i < BUFFER_SIZE; i++)
			{
				BufferList[i-1] = BufferList[i];
			}
			BufferList[BUFFER_SIZE-1]->dataLength = BUFFER_EMPTY;
			BufferList[BUFFER_SIZE-1] = 0;

			BufferPosition--;
			startSerialDMA();
		}
		else
		{
			BufferList[0]->dataLength = BUFFER_EMPTY;
			BufferList[0] = 0;
			BufferPosition = -1;
		}
		/*if(BufferList[1] != 0)
		{
			BufferList[0]->dataLength = BUFFER_EMPTY;

			int i;
			for(i = 1; i < BUFFER_SIZE; i++)
			{
				BufferList[i-1] = BufferList[i];
			}
		}
		else
		{
			BufferList[0]->dataLength = 0;
			BufferList[0] = 0;
		}*/


		/*if(Buffer[1].dataLength != BUFFER_EMPTY)
		{
			// Shifts all data down one row
			//   ex. Buffer 1 is now Buffer 0, Buffer 2 is now Buffer 1
			int i;
			for(i = 1; i < BUFFER_SIZE; i++)
			{
				int j;
				for(j = 0; j < BUFFER_STRING_LENGTH; j++)
				{
					Buffer[i-1].data[j] = Buffer[i].data[j];
				}

				Buffer[i-1].dataLength = Buffer[i].dataLength;				
			}
			Buffer[BUFFER_SIZE - 1].dataLength = BUFFER_EMPTY;
			startSerialDMA();
		}
		else
		{
			Buffer[0].dataLength = BUFFER_EMPTY;
		}*/

		/* Clear DMA Stream Transfer Complete interrupt pending bit */
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
	}
}

void USART1_IRQHandler(void)
{
	if( USART_GetITStatus(USART1, USART_IT_RXNE) )
	{
		//t = USART1->DR;
		//SerialSend(&t);

		/*static uint8_t cnt = 0;
		char t = USART1->DR;

		if( (t != '\n') && (cnt < 20) )
		{
			receivedString[cnt] = t;
			cnt++;
		}
		else
		{
			cnt = 0;
			GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
			SerialSend(receivedString);
		}*/
	}
}



