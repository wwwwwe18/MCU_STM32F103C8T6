#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size = Size;
	
	// ***********************************************
	// 1. RCC Clock
	// ***********************************************
	// Enable the AHB peripheral clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// ***********************************************
	// 2. DMA
	// ***********************************************
	// DMA Init structure
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = AddrA;							// Peripheral base address for DMAy Channelx
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	// Peripheral data width
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;			// Peripheral address register is incremented
	
	DMA_InitStruct.DMA_MemoryBaseAddr = AddrB;								// Memory base address for DMAy Channelx
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			// Memory data width
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;					// Memory address register is incremented
	
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;		// Peripheral is the source
	DMA_InitStruct.DMA_BufferSize = Size;				// Buffer size, in data unit, of the specified Channel (1)
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;			// DMAy Channelx will be used in memory-to-memory transfer (2)
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;	// Software priority for the DMAy Channelx
	
	// Initialize the DMAy Channelx according to the specified parameters in the DMA_InitStruct
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	
	// Disable the specified DMAy Channelx
	DMA_Cmd(DMA1_Channel1, DISABLE);

}

void MyDMA_Transfer(void)
{
	// Disable the specified DMAy Channelx
	DMA_Cmd(DMA1_Channel1, DISABLE);
	
	// Set the number of data units in the current DMAy Channelx transfer
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
	
	// Enable the specified DMAy Channelx (3)
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// Check whether the specified DMAy Channelx flag is set or not
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);	// DMA1_FLAG_TC1: DMA1 Channel1 transfer complete flag
	
	// Clear the DMAy Channelx's pending flags
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
