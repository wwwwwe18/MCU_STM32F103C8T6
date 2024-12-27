#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;		// Input Pull Up
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// No effect
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}

// Get number of key
uint8_t Key_GetNum(void)
{

	uint8_t KeyNum = 0;
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)	// Key1 (PB1) is pressed
	{
	
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
		Delay_ms(20);
		KeyNum = 1;
		
	}
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)	// Key2 (PB11) is pressed
	{
	
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
		Delay_ms(20);
		KeyNum = 2;
		
	}
	
	return KeyNum;
	
}
