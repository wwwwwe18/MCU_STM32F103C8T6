#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{

	// ****************************************************
	// 1. Enable the High Speed APB (APB2) peripheral clock
	// ****************************************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// ****************************************************
	// 2. Initialize the GPIOA peripheral according to the specified parameters in the GPIO_InitStruct
	// ****************************************************
	
	// Initialize GPIO_InitTypeDef structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;	// Out Push Pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;			// Pin0
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// 50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// ****************************************************
	// 3. Clear the selected data port bits
	// ****************************************************
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
//	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	
	while(1)
	{
		
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);	// Turn on LED
		Delay_ms(500);									// Delay 500ms
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);		// Turn off LED
		Delay_ms(500);									// Delay 500ms
		
	}

}
