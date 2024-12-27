#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"

int main(void)
{

	OLED_Init();
	MyRTC_Init();
	
	OLED_ShowString(1, 1, "Date:XXXX-XX-XX");
	OLED_ShowString(2, 1, "Time:XX:XX:XX");
	OLED_ShowString(3, 1, "CNT :");
	OLED_ShowString(4, 1, "DIV :");
	
	while(1)
	{
		MyRTC_ReadTime();
		
		OLED_ShowNum(1, 6,  MyRTC_Time[0], 4);		// Y
		OLED_ShowNum(1, 11, MyRTC_Time[1], 2);		// M
		OLED_ShowNum(1, 14, MyRTC_Time[2], 2);		// D
													
		OLED_ShowNum(2, 6,  MyRTC_Time[3], 2);		// Hr
		OLED_ShowNum(2, 9,  MyRTC_Time[4], 2);		// Min
		OLED_ShowNum(2, 12, MyRTC_Time[5], 2);		// Sec
		
		OLED_ShowNum(3, 6,  RTC_GetCounter(), 10);	// Get the RTC counter value
		
		OLED_ShowNum(4, 6,  RTC_GetDivider(), 10);	// Get the RTC divider value
		
//		OLED_ShowNum(4, 6,  (32767 - RTC_GetDivider()) / 32767.0 * 999, 10);	// msec
	}

}
