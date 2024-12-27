#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint16_t Num;

int main(void)
{

	OLED_Init();
	Timer_Init();
	
	OLED_ShowString(1, 1, "Num:");
	
	while(1)
	{
		OLED_ShowNum(1, 5, Num, 5);
//		OLED_ShowNum(2, 5, TIM_GetCounter(TIM2), 5);	// ARR
	}

}

void TIM2_IRQHandler(void)
{
	// Check whether the TIM interrupt has occurred or not
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Num++;
		
		// Clear the TIMx's interrupt pending bits
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
