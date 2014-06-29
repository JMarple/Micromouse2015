#include "ConfigSerial.h"
#include "ConfigSerialPeripherals.h"

typedef struct SerialBuffer
{
	char data[BUFFER_STRING_LENGTH];

	int dataLength;
} SerialBuffer;

// Copy String To Buffer
static int copyStringToBuffer(volatile SerialBuffer* Buffer, char* data);
static int copyIntegerToBuffer(volatile SerialBuffer* Buffer, int data);
static void startSerialDMA();
static void addBufferToSendingMessages(SerialBuffer* buffer);
static volatile SerialBuffer* findFreeBuffer();

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
}

void SerialSendChar(char* message)
{
	SerialBuffer* buffer = findFreeBuffer();

	if(buffer != 0)
	{
		copyStringToBuffer(buffer, message);
		addBufferToSendingMessages(buffer);	
	}
}

void SerialSendRawInt(int number)
{
	SerialBuffer* buffer = findFreeBuffer();

	if(buffer != 0)
	{
		copyIntegerToBuffer(buffer, number);
		addBufferToSendingMessages(buffer);
	}
}

void SerialUpdateBuffer()
{
	
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
	// Defualt all characters to zero
	int i;
	for(i = 0; i < BUFFER_STRING_LENGTH; i++)
		Buffer->data[i] = 0;

	// Until we get a null pointer, assign our characters
	i = 0;
	while(*data)
	{
		Buffer->data[i] = *data;
		i++;
		data++;
	}

	// Assign the length of the string so 
	//   the DMA knows how many bytes to go through
	Buffer->dataLength = i;

	return i;
}

static int copyIntegerToBuffer(volatile SerialBuffer* Buffer, int data)
{
	int highbyte2 = data >> 24 & 0xFF;
	int highbyte1 = data >> 16 & 0xFF;
	int lowbyte1 = data >> 8 & 0xFF;
	int lowbyte2 = data >> 0 & 0xFF;

	Buffer->data[0] = data >> 24 & 0xFF;
	Buffer->data[1] = data >> 16 & 0xFF;
	Buffer->data[2] = data >> 8 & 0xFF;
	Buffer->data[3] = data >> 0 & 0xFF;

	Buffer->dataLength = 4;

	return 4;
}

static volatile SerialBuffer* findFreeBuffer()
{
	int i;
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		if(Buffer[i].dataLength == BUFFER_EMPTY)
		{
			return &Buffer[i];
		}
	}

	return 0;
}

static void addBufferToSendingMessages(SerialBuffer* buffer)
{
	// If that buffer exists
	if(buffer != 0)
	{
		// If the list is null (no pointers)
		if(BufferPosition == -1)
		{
			// Increment our positioner to 0
			BufferPosition++;

			// Assign first pointer to the target buffer
			BufferList[0] = buffer;

			// Start Transmission
			startSerialDMA();
		}
		else
		{		
			// Since we have multiple messages in the queue, 
			//   increment our counter and link the buffer
			BufferPosition++;
			BufferList[BufferPosition] = buffer;			
		}
	}
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



