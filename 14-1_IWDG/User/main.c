#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{

	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1, 1, "IWDG TEST");
	
	// Check whether the specified RCC flag is set or not
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)	// Independent Watchdog reset
	{
		OLED_ShowString(2, 1, "IWDGRST");
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
	
	// If the Independent watchdog (IWDG) is started by either hardware option or software
	// access, the LSI oscillator is forced ON and cannot be disabled. After the LSI oscillator
	// temporization, the clock is provided to the IWDG.

	// Enable write access to IWDG_PR and IWDG_RLR registers
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	// Enable write access to IWDG_PR and IWDG_RLR registers (0x5555)
	
	// Timeout period = 1000ms
	// T_IWDG = T_LSI * PR * (RL + 1)
	
	// Set IWDG Prescaler value
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	
	// Set IWDG Reload value
	IWDG_SetReload(2500 - 1);
	
	// Reload IWDG counter with value defined in the reload register
	// (write access to IWDG_PR and IWDG_RLR registers disabled)
	IWDG_ReloadCounter();
	
	// Enable IWDG (write access to IWDG_PR and IWDG_RLR registers disabled)
	IWDG_Enable();
	
	while(1)
	{
		Key_GetNum();
		
		IWDG_ReloadCounter();
		
		// 800ms
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(200);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600);
	}

}
