#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
	// GPIO -> AFIO -> EXTI -> NVIC -> CPU

	// 1. Enable the High Speed APB (APB2) peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	// 2. Initialize the GPIOB peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	// Input Pull Up
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// 3. Select the GPIO pin used as EXTI Line
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	// 4. Initialize the EXTI peripheral according to the specified parameters in the EXTI_InitStruct
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line14;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	
	EXTI_Init(&EXTI_InitStruct);
	
	// 5. NVIC
	// Configures the priority grouping: pre-emption priority and subpriority
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// Initialize the NVIC peripheral according to the specified parameters in the NVIC_InitStruct
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	// 0 - 3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;			// 0 - 3
	
	NVIC_Init(&NVIC_InitStruct);
	
}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

void EXTI15_10_IRQHandler(void)
{

	// Checks whether the specified EXTI line is asserted or not
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
	
		CountSensor_Count++;
		
		// Clears the EXTI's line pending bits
		EXTI_ClearITPendingBit(EXTI_Line14);
		
	}
	
}
