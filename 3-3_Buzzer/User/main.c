#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{

	// ****************************************************
	// 1. Enable the High Speed APB (APB2) peripheral clock
	// ****************************************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// ****************************************************
	// 2. Initialize the GPIOB peripheral according to the specified parameters in the GPIO_InitStruct
	// ****************************************************
	
	// Initialize GPIO_InitTypeDef structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;	// Out Push Pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;			// Pin12
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// 50MHz
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	while(1)
	{
		
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		Delay_ms(100);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		Delay_ms(500);
		
	}

}
