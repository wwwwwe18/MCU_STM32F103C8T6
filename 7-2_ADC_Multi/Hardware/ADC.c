#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	// **************************************************
	// 1. RCC
	// **************************************************
	// Enable the High Speed APB (APB2) peripheral clocK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
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
	// Initialize the ADCx peripheral according to the specified parameters in the ADC_InitStruct
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;	// Single mode
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE; 			// Single (one channel) mode
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
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
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	// Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	
	// Enable the selected ADC software start conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	// EOC: End of conversion
	// 0: Conversion is not complete
	// 1: Conversion complete
	
	// Check whether the specified ADC flag is set or not
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	// EOC == 0: Conversion is not complete
	
	// EOC = 1: Conversion complete
	
	// 72MHz / 6 = 12 MHz
	// 68 cycles in total (Tconv = 5.67 us)
	// Sample time equal to 55.5 cycles
	// Compare 12.5 cycles
	
	// Return the last ADCx conversion result data for regular channel
	return ADC_GetConversionValue(ADC1);	// uint16_t
	
	// EOC: It is cleared by software or by reading the ADC_DR
}
