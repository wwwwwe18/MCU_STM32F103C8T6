#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"

int main(void)
{

	OLED_Init();
	MyRTC_Init();
	
	// Enable the Low Speed APB (APB1) peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	// PWR
	
	OLED_ShowString(1, 1, "CNT :");
	OLED_ShowString(2, 1, "ALR :");
	OLED_ShowString(3, 1, "ALRF:");
	
	// Enable the WakeUp Pin functionality
	PWR_WakeUpPinCmd(ENABLE);
	
	uint32_t Alarm = RTC_GetCounter() + 5;
	
	// Set the RTC alarm value
	RTC_SetAlarm(Alarm);
	
	OLED_ShowNum(2, 6,  Alarm, 10);
	
	while(1)
	{
		MyRTC_ReadTime();
		
		OLED_ShowNum(1, 6,  RTC_GetCounter(), 10);	// Get the RTC counter value
		
		// Check whether the specified RTC flag is set or not
		// RTC_FLAG_ALR: Alarm flag
		OLED_ShowNum(3, 6,  RTC_GetFlagStatus(RTC_FLAG_ALR), 1);
		
		OLED_ShowString(4, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(4, 1, "       ");
		Delay_ms(100);
		
		OLED_ShowString(4, 9, "STANDBY");
		Delay_ms(1000);
		OLED_ShowString(4, 9, "       ");
		Delay_ms(100);
		
		OLED_Clear();
		
		// Enter STANDBY mode
		PWR_EnterSTANDBYMode();
	}

}
