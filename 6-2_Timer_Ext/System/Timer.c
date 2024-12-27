#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{

	// 1. Enable the Low Speed APB (APB1) peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 2. Initialize the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	// 3. Configure the External clock Mode2
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);
	
	// 4. Initialize the TIMx Time Base Unit peripheral according to the specified parameters in the TIM_TimeBaseInitStruct
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10 - 1;		// 0 - 65535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1;	// 0 - 65535
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	
	// CK_CNT_OV = CK_PSC/(PSC+1)/(ARR+1)
	// 7.2MHz = 72MHz / 1 / 10
	// PSC = 1 - 1
	// ARR = 10 - 1
	// CK_CNT = CK_PSC/(PSC+1)
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	// Clear the TIMx's pending flags
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	// 5. Enable the specified TIM interrupts
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// 6. NVIC
	// Configure the priority grouping: pre-emption priority and subpriority
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// Initialize the NVIC peripheral according to the specified parameters in the NVIC_InitStruct
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;	// 0 - 3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;			// 0 - 3
	
	NVIC_Init(&NVIC_InitStruct);
	
	// 7. Enable the specified TIM peripheral
	TIM_Cmd(TIM2, ENABLE);
	
}

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
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
