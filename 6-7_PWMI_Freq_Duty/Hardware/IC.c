#include "stm32f10x.h"                  // Device header

void IC_Init(void)
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
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// *************************************************
	// 3. TIMx Time Base Unit
	// *************************************************
	// Configure the TIMx internal Clock
	TIM_InternalClockConfig(TIM3);
	
	// Initialize the TIMx Time Base Unit peripheral according to the specified parameters in the TIM_TimeBaseInitStruct
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;	// ARR	0 - 65535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;	// PSC	0 - 65535, fc = 1MHz
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	
	// CK_CNT_OV = CK_PSC/(PSC+1)/(ARR+1)
	// CK_CNT = CK_PSC/(PSC+1)
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	// *************************************************
	// 4. Input Capture (IC)
	// *************************************************
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;	// f_samp = f_DTS / 32, N = 8
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	
	// Configures the TIM peripheral according to the specified parameters in the TIM_ICInitStruct to measure an external PWM signal
	TIM_PWMIConfig(TIM3, &TIM_ICInitStruct);
	
	// *************************************************
	// 5. Select the Input Trigger source
	// *************************************************
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	
	// *************************************************
	// 6. Select the TIMx Slave Mode
	// *************************************************
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);	// Rising edge of the selected trigger signal (TRGI) re-initializes the counter and triggers an update of the registers.
	
	// *************************************************
	// 7. Enables the specified TIM peripheral
	// *************************************************
	TIM_Cmd(TIM3, ENABLE);
	
}

uint32_t IC_GetFreq(void)
{
	// fc / N = CK_PSC/(PSC+1) / CCR
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

uint32_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
}
