#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{

	// 1. Enable the Low Speed APB (APB1) peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 2. Configure the TIMx internal Clock
	TIM_InternalClockConfig(TIM2);
	
	// 3. Initialize the TIMx Time Base Unit peripheral according to the specified parameters in the TIM_TimeBaseInitStruct
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;		// 0 - 65535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;	// 0 - 65535
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	
	// CK_CNT_OV = CK_PSC/(PSC+1)/(ARR+1)
	// 1sec <=> 1Hz
	// 1Hz = 72MHz / 7200 / 10000
	// PSC = 7200 - 1
	// ARR = 10000 - 1
	// CK_CNT = CK_PSC/(PSC+1) = 10kHz
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	// Clear the TIMx's pending flags
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	// 4. Enable the specified TIM interrupts
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// 5. NVIC
	// Configure the priority grouping: pre-emption priority and subpriority
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// Initialize the NVIC peripheral according to the specified parameters in the NVIC_InitStruct
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;	// 0 - 3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;			// 0 - 3
	
	NVIC_Init(&NVIC_InitStruct);
	
	// 6. Enable the specified TIM peripheral
	TIM_Cmd(TIM2, ENABLE);
	
}

/*
void TIM2_IRQHandler(void)
{
	// Check whether the TIM interrupt has occurred or not
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		// Clear the TIMx's interrupt pending bits
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/
