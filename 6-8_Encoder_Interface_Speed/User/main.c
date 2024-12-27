#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

int16_t Speed;

int main(void)
{

	OLED_Init();
	Timer_Init();	// CK_CNT_OV = 1Hz
	Encoder_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	
	while(1)
	{
		OLED_ShowSignedNum(1, 7, Speed, 5);
	}

}

void TIM2_IRQHandler(void)
{
	// Check whether the TIM interrupt has occurred or not
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Speed = Encoder_Get();
		
		// Clear the TIMx's interrupt pending bits
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
