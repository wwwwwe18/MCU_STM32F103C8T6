#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

void AD_Init(void)
{
	// **************************************************
	// 1. RCC Clock
	// **************************************************
	// Enable the High Speed APB (APB2) peripheral clocK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// Enable the AHB peripheral clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// Configure the ADC clock (ADCCLK)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	// ADC clock = PCLK2/6 = 72MHz / 6 = 12MHz
	
	// **************************************************
	// 2. GPIO
	// **************************************************
	// Initialize the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// **************************************************
	// 3. ADC
	// **************************************************
	// Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	// Initialize the ADCx peripheral according to the specified parameters in the ADC_InitStruct
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;		// Continuous mode
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 4;				// Number of ADC channels that will be converted using the sequencer for regular channel group
	ADC_InitStruct.ADC_ScanConvMode = ENABLE; 			// Scan (multichannels) mode
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
	// ***********************************************
	// 4. DMA
	// ***********************************************
	// DMA Init structure
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;			// Peripheral base address for DMAy Channelx
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// Peripheral data width
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// Peripheral address register is not incremented
	
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)AD_Value;					// Memory base address for DMAy Channelx
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		// Memory data width
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;					// Memory address register is incremented
	
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;		// Peripheral is the source
	DMA_InitStruct.DMA_BufferSize = 4;					// Buffer size, in data unit, of the specified Channel (1)
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;			// Disable; choose hardware trigger (2*)
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;	// Software priority for the DMAy Channelx
	
	// Initialize the DMAy Channelx according to the specified parameters in the DMA_InitStruct
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	
	// Enable the specified DMAy Channelx (3)
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// Enable the specified ADC DMA request
	ADC_DMACmd(ADC1, ENABLE);
	
	// Enable the specified ADC peripheral
	ADC_Cmd(ADC1, ENABLE);
	
	// Reset the selected ADC calibration registers
	ADC_ResetCalibration(ADC1);
	
	// RSTCAL: Reset calibration
	// 1: Initialize calibration register
	// 0: Calibration register initialized
	
	// Get the selected ADC reset calibration registers status
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);	// RSTCAL == 1: Initialize calibration register
	
	// RSTCAL = 0: Calibration register initialized
	
	// CAL: A/D Calibration
	// 1: Enable calibration
	// 0: Calibration completed
	
	// Start the selected ADC calibration process
	ADC_StartCalibration(ADC1);
	
	// Get the selected ADC calibration status
	while(ADC_GetCalibrationStatus(ADC1) == SET);	// CAL == 1: Enable calibration
	
	// CAL = 0: Calibration completed
	
	// Enable the selected ADC software start conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
void AD_GetValue(void)
{
	// Disable the specified DMAy Channelx
	DMA_Cmd(DMA1_Channel1, DISABLE);
	
	// Set the number of data units in the current DMAy Channelx transfer
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
	
	// Enable the specified DMAy Channelx (3)
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// Check whether the specified DMAy Channelx flag is set or not
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);	// DMA1_FLAG_TC1: DMA1 Channel1 transfer complete flag
	
	// Clear the DMAy Channelx's pending flags
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
*/
