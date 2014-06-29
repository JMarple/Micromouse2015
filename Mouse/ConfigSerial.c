#include "ConfigSerial.h"
#include "ConfigSerialPeripherals.h"

typedef struct SerialBuffer
{
	char data[BUFFER_STRING_LENGTH];

	int dataLength;
} SerialBuffer;

// Copy String To Buffer
static void startSerialDMA();
static volatile SerialBuffer* findFreeBuffer();

volatile SerialBuffer Buffer[BUFFER_SIZE];
volatile SerialBuffer* BufferList[BUFFER_SIZE];
volatile int BufferPosition;

volatile SerialBuffer* editingBuffer;

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

	BufferPosition = 0;
	editingBuffer = findFreeBuffer();

	SerialRCC();
	SerialGPIO();
	SerialUSART(Baud);
	SerialNVIC();
	SerialDMA();
}

void SerialUpdateEditingBuffer(char* message)
{
	while(*message)
	{
		if(editingBuffer != 0)
		{
			editingBuffer->data[editingBuffer->dataLength] = *message;
			editingBuffer->dataLength++;

			if(editingBuffer->dataLength >= BUFFER_STRING_LENGTH)
			{
				SerialSendEditingBuffer();
			}			
		}
		// We're losing data at this point because of an overflow
		else
		{
			editingBuffer = findFreeBuffer();
		}

		message++;
	}
}

void SerialSendEditingBuffer()
{	
	BufferList[BufferPosition] = editingBuffer;
	BufferPosition++;

	if(BufferPosition == 1)
	{
		startSerialDMA();
	}

	// Get new editing buffer
	editingBuffer = findFreeBuffer();
}

void SerialSendRawInteger(int data)
{
	if(editingBuffer != 0)
	{
		int sentFlag = 0;

		int i;
		for(i = sizeof(data)-1; i >= 0; i--)
		{
			char num = (data >> (i*8) ) & 0xFF;

			if( (num != 0 || sentFlag) 
				|| (i == 0))
			{
				sentFlag = 1;
				editingBuffer->data[editingBuffer->dataLength] = num;
				editingBuffer->dataLength++;

				if(editingBuffer->dataLength >= BUFFER_STRING_LENGTH)
				{
					SerialSendEditingBuffer();
				}
			}
		}
	}
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

static volatile SerialBuffer* findFreeBuffer()
{
	int i;
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		if(Buffer[i].dataLength == BUFFER_EMPTY)
		{
			// Mark as empty but in use
			Buffer[i].dataLength = 0;
			return &Buffer[i];
		}
	}

	return 0;
}

// DMA Completion Interrupt
// This is called when the DMA has completed transmitting it's current message 
void DMA2_Stream7_IRQHandler(void)
{
	// DMA 2 Stream 7 Completion 
	if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
	{		
		if(BufferPosition > 1)
		{
			// Open up this buffer for reuse
			BufferList[0]->dataLength = BUFFER_EMPTY;
			
			// Shift buffer pointers
			int i;
			for(i = 1; i < BUFFER_SIZE; i++)
			{
				BufferList[i-1] = BufferList[i];
			}

			BufferList[BUFFER_SIZE-1] = 0;

			BufferPosition--;
			startSerialDMA();
		}
		else
		{
			BufferList[0]->dataLength = BUFFER_EMPTY;
			BufferList[0] = 0;
			BufferPosition = 0;
		}

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



