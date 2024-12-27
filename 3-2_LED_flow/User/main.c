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
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;		// Pin all
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// 50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// ****************************************************
	// 3. Clear the selected data port bits
	// ****************************************************
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
//	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	
	while(1)
	{
		
		GPIO_Write(GPIOA, ~0x0001);						// Turn on LED0 0000_0000_0000_0001
		Delay_ms(100);									// Delay 100ms
		GPIO_Write(GPIOA, ~0x0002);						// Turn on LED1
		Delay_ms(100);
		GPIO_Write(GPIOA, ~0x0004);						// Turn on LED2
		Delay_ms(100);
		GPIO_Write(GPIOA, ~0x0008);						// Turn on LED3
		Delay_ms(100);
		GPIO_Write(GPIOA, ~0x0010);						// Turn on LED4
		Delay_ms(100);
		GPIO_Write(GPIOA, ~0x0020);						// Turn on LED5
		Delay_ms(100);
		GPIO_Write(GPIOA, ~0x0040);						// Turn on LED6
		Delay_ms(100);
		GPIO_Write(GPIOA, ~0x0080);						// Turn on LED7
		Delay_ms(100);
		
	}

}
