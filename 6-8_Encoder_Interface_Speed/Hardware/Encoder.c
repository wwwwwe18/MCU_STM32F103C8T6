#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	// *************************************************
	// 1. RCC clock
	// *************************************************
	// Enable the Low Speed APB (APB1) peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// Enable the High Speed APB (APB2) peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// *************************************************
	// 2. GPIOx
	// *************************************************
	// Initialize the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	// Input Pull Up
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// *************************************************
	// 3. TIMx Time Base Unit
	// *************************************************
	// Initialize the TIMx Time Base Unit peripheral according to the specified parameters in the TIM_TimeBaseInitStruct
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;	// ARR	0 - 65535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1;	// PSC	0 - 65535
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	
	// CK_CNT_OV = CK_PSC/(PSC+1)/(ARR+1)
	// CK_CNT = CK_PSC/(PSC+1)
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	// *************************************************
	// 4. Encoder Interface
	// *************************************************
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	// Fill each TIM_ICInitStruct member with its default value
	TIM_ICStructInit(&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;	// f_samp = f_DTS / 32, N = 8
//	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	
	// Initialize the TIM peripheral according to the specified parameters in the TIM_ICInitStruct
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;	// f_samp = f_DTS / 32, N = 8
//	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	
	// Initialize the TIM peripheral according to the specified parameters in the TIM_ICInitStruct
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	// Configures the TIMx Encoder Interface
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	// *************************************************
	// 5. Enable the specified TIM peripheral
	// *************************************************
	TIM_Cmd(TIM3, ENABLE);
	
}


int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);	// Get the TIMx Counter value
	TIM_SetCounter(TIM3, 0);		// Sets the TIMx Counter Register value - clear
	
	return Temp;
}
