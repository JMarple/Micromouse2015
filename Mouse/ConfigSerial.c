#include "ConfigSerial.h"
#include "ConfigSerialPeripherals.h"

// Important functions:
//  SerialSaveRawChar -> Updates the buffer pointed by editingBuffer
//                         and updates DMA on overflows
//  startSerialDMA    -> Starts the DMA with the info at BufferList[0]->data
//  DMA2_Stream7_IRQHandler -> is called when DMA has finished a buffer and 
//                                needs a new one!
// Notes:
//   Buffer is all the information in no particular order
//     (findFreeBuffer will find a new buffer at random)
//
//   BufferList points to a certain buffer location
//     this will be used by startSerialDMA, [0] is the next 
//     position to be sent by USART, next beeing 1, then 2, etc.
//
//   editingBuffer points to a buffer that SerialSaveRawChar
//     will update.  When the buffer from editingBuffer is full,
//     it will save that pointer to BufferList, then find a new
//     buffer with findFreeBuffer

// Stores each buffer of bytes that will be
//   references to by the DMA (see startSerialDMA())
typedef struct SerialBuffer
{
	// The bytes that 
	char data[BUFFER_STRING_LENGTH];

	// The number of bytes written to data[]
	int dataLength;

} SerialBuffer;

// Starts the DMA
static void startSerialDMA();

// Returns a pointer to a new buffer
static SerialBuffer* findFreeBuffer();

// Holds the data to be sent in no particular order
SerialBuffer Buffer[BUFFER_SIZE];

// Tells us the order to send the data, 0 being first
SerialBuffer* BufferList[BUFFER_SIZE];

// The buffer currently being added to
SerialBuffer* editingBuffer;

// The position in the editingBuffer;
int BufferPosition;

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

	// Basic Initiation
	SerialRCC();
	SerialGPIO();
	SerialUSART(Baud);
	SerialNVIC();
	SerialDMA();
}

// Save a string to editingBuffer
void SerialSaveString(char* message)
{
	while(*message)
	{
		SerialSaveRawChar(*message);
		message++;
	}
}

// Save a char to editingBuffer
void SerialSaveRawChar(char num)
{
	if(editingBuffer != 0)
	{
		editingBuffer->data[editingBuffer->dataLength] = num;		
		editingBuffer->dataLength++;

		// Checks if the buffer is full
		if(editingBuffer->dataLength >= BUFFER_STRING_LENGTH)
		{
			SerialForceBuffer();
		}
	}
	else
	{
		editingBuffer = findFreeBuffer();
	}
}

// Save an arbitrary number to editingBuffer
// This will only show important bytes! 
// ie, 5 (0101), will be sent as 00000101
// instead of 00000000 00000000 00000000 00000101
void SerialSaveNumber(long data)
{
	int sentFlag = 0;

	int i;
	for(i = sizeof(data)-1; i >= 0; i--)
	{
		// Get the byte i of the data
		char num = (data >> (i*8) ) & 0xFF;

		// If number isn't 0, 
		//  or we've saved some data before, 
		//  or we're on the last byte
		if( (num != 0 || sentFlag) 
			|| (i == 0))
		{
			sentFlag = 1;
			SerialSaveRawChar(num);
		}
	}	
}

// Saves 4 bytes to editingBuffer
void SerialSaveRawInteger(int data)
{	
	int i;
	for(i = 3; i >= 0; i--)
	{
		SerialSaveRawChar((data >> (i*8) ) & 0xFF);			
	}
}

// Forces the buffer to send anything in the buffer
//  even if the buffer isn't full!
void SerialForceBuffer()
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

// Starts the DMA process on our processor
static void startSerialDMA()
{
	DMA_DeInit(DMA2_Stream7);

	// Only transwer the number of bytes listed by dataLength
	DMA_Struct.DMA_BufferSize = BufferList[0]->dataLength;

	// Set the pointer to the buffer to BufferList[0]
	DMA_Struct.DMA_Memory0BaseAddr = (uint32_t)BufferList[0];

	DMA_Init(DMA2_Stream7, &DMA_Struct);

	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA2_Stream7, ENABLE); 
}

// Finds the next buffer not being used
//   it looks for dataLength to equal BUFFER_EMPTY
static SerialBuffer* findFreeBuffer()
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
			
			// Shift buffer pointers so that [1] is now [0]
			int i;
			for(i = 1; i < BUFFER_SIZE; i++)
			{
				BufferList[i-1] = BufferList[i];
			}

			// Set the last buffer to be null since it's new
			BufferList[BUFFER_SIZE-1] = 0;

			// Decrement the position as one position was just erased
			BufferPosition--;

			// Start a new DMA stream with the new buffer (BufferList[0])
			startSerialDMA();
		}
		else
		{
			// Since there are no more buffers to send, set BufferList[0] to null
			BufferList[0]->dataLength = BUFFER_EMPTY;
			BufferList[0] = 0;
			BufferPosition = 0;
		}

		// Clear DMA Stream Transfer Complete interrupt pending bit 
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
	}
}

// Receving function (not used atm)
void USART1_IRQHandler(void)
{
	if( USART_GetITStatus(USART1, USART_IT_RXNE) )
	{
		//t = USART1->DR;
	}
}