#include "stm32f10x.h"                  // Device header

void Buzzer_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	
}

void Buzzer_ON(void)
{
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);

}

void Buzzer_OFF(void)
{
	
	GPIO_SetBits(GPIOB, GPIO_Pin_12);

}

void Buzzer_Turn(void)
{
	
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12) == 0)	// If buzzer is ON
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);				// Turn off buzzer
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);				// Else turn on buzzer
	}

}
