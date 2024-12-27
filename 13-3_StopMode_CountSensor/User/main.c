#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main(void)
{

	OLED_Init();
	CountSensor_Init();
	
	// Enable the Low Speed APB (APB1) peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	OLED_ShowString(1, 1, "Count:");
	
	while(1)
	{
		OLED_ShowNum(1, 7, CountSensor_Get(), 5);
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		// Enter STOP mode
		// PWR_Regulator_ON: STOP mode with regulator ON
		// PWR_STOPEntry_WFI: enter STOP mode with WFI instruction
		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
		
		// When exiting Stop mode by issuing an interrupt or a wakeup event, the HSI RC oscillator is
		// selected as system clock.
		
		SystemInit();
	}

}
