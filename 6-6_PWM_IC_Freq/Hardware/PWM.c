#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{

	// 1. RCC clock
	// Enable the Low Speed APB (APB1) peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// Enable the High Speed APB (APB2) peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/*
	// Use PA15 as GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// Changes the mapping of the specified pin
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	// JTAG-DP Disabled and SW-DP Enabled
	*/
	
	// 2. Initializes the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	// Alternate Function Push Pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 3. Configure the TIMx internal Clock
	TIM_InternalClockConfig(TIM2);
	
	// 4. Initialize the TIMx Time Base Unit peripheral according to the specified parameters in the TIM_TimeBaseInitStruct
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;	// ARR	0 - 65535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;	// PSC	0 - 65535
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	
	// CK_CNT_OV = CK_PSC/(PSC+1)/(ARR+1)
	// 1sec <=> 1Hz
	// 1kHz = 72MHz / 720 / 100
	// PSC = 720 - 1
	// ARR = 100 - 1
	// CK_CNT = CK_PSC/(PSC+1)
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	// Clear the TIMx's pending flags
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	// 5. Initialize the TIMx Channel1 according to the specified parameters in the TIM_OCInitStruct
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	// Fill each TIM_OCInitStruct member with its default value
	TIM_OCStructInit(&TIM_OCInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;	// CCR
	
	// PWM
	// Freq = CK_CNT_OV = CK_PSC/(PSC+1)/(ARR+1) = 1kHz
	// Duty = CRR/(ARR+1) = 50% => CRR = 50
	// Reso = 1/(ARR+1) = 1%
	
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	
	// 6. Enable the specified TIM peripheral
	TIM_Cmd(TIM2, ENABLE);
	
}

void PWM_SetCompare1(uint16_t Compare)
{
	// Set the TIMx Capture Compare1 Register value
	TIM_SetCompare1(TIM2, Compare);
}

void PWM_SetPrescaler(uint16_t Prescaler)
{
	// Configures the TIMx Prescaler
	TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate);
}
