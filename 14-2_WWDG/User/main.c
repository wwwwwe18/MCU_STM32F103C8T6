#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{

	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1, 1, "WWDG TEST");
	
	// Check whether the specified RCC flag is set or not
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)	// Window Watchdog reset
	{
		OLED_ShowString(2, 1, "WWDGRST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		RCC_ClearFlag();	// Clear the RCC reset flags
	}
	else
	{
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	// Enable the Low Speed APB (APB1) peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	// T_WWDG = T_PCLK1 * 4096 * 2^WDGTB * (T[5:0] + 1)
	// T_WIN  = T_PCLK1 * 4096 * 2^WDGTB * (T[5:0] - W[5:0])
	
	// T_WWDG = 50ms
	// T_WIN  = 30ms
	
	// T_PCLK1 = 1 / (30MHz)
	// WDGTB   = 3 (2^3 = 8)
	// T[5:0]  = 54
	// W[5:0]  = 21
	
	// Set the WWDG Prescaler
	WWDG_SetPrescaler(WWDG_Prescaler_8);	// WWDG counter clock = (PCLK1/4096)/8
	
	// Set the WWDG window value
	WWDG_SetWindowValue(0x40 | 21);			// W[6] = 1
	
	// Enable WWDG and load the counter value
	WWDG_Enable(0x40 | 54);					// T[6] = 1
	
	while(1)
	{
		Key_GetNum();
		
		// 40ms
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(20);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(20);
		
		// Set the WWDG counter value
		WWDG_SetCounter(0x40 | 54);
	}

}
