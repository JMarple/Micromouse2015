#include "NVIC.h"
#include "config.h"

void NVIC_SetupInterrupt(int channel, int prePrio, int subprio, int cmd)
{
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = channel;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = prePrio;
	nvicStructure.NVIC_IRQChannelSubPriority = subprio;
	nvicStructure.NVIC_IRQChannelCmd = cmd;
	NVIC_Init(&nvicStructure);	
}