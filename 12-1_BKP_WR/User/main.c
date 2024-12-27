#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

uint8_t KeyNum;

uint16_t ArrayWrite[] = {0x1234, 0x5678};
uint16_t ArrayRead[2];

int main(void)
{

	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1, 1, "W:");
	OLED_ShowString(2, 1, "R:");
	
	// **********************************
	// 1. RCC clock
	// **********************************
	// Enable the power and backup interface clocks by setting the PWREN and BKPEN bits in the RCC_APB1ENR register
	
	// Enable the Low Speed APB (APB1) peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	// PWR
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);	// BKP
	
	// **********************************
	// 2. PWR
	// **********************************
	// Set the DBP bit in the Power control register (PWR_CR) to enable access to the Backup registers and RTC
	
	// Enable access to the RTC and backup registers
	PWR_BackupAccessCmd(ENABLE);
	
	/*
	// **********************************
	// 3. BKP
	// **********************************
	// Write user data to the specified Data Backup Register
	BKP_WriteBackupRegister(BKP_DR1, 0x1234);	// BKP_DR: 1 - 10
	
	// Read data from the specified Data Backup Register
	OLED_ShowHexNum(1, 1, BKP_ReadBackupRegister(BKP_DR1), 4);
	*/
	
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if(KeyNum == 1)
		{
			ArrayWrite[0]++;
			ArrayWrite[1]++;
			
			// **********************************
			// 3. BKP
			// **********************************
			// Write user data to the specified Data Backup Register
			BKP_WriteBackupRegister(BKP_DR1, ArrayWrite[0]);	// BKP_DR: 1 - 10
			BKP_WriteBackupRegister(BKP_DR2, ArrayWrite[1]);
			
			OLED_ShowHexNum(1, 3, ArrayWrite[0], 4);
			OLED_ShowHexNum(1, 8, ArrayWrite[1], 4);
		}
		
		// Read data from the specified Data Backup Register
		ArrayRead[0] = BKP_ReadBackupRegister(BKP_DR1);
		ArrayRead[1] = BKP_ReadBackupRegister(BKP_DR2);

		OLED_ShowHexNum(2, 3, ArrayRead[0], 4);
		OLED_ShowHexNum(2, 8, ArrayRead[1], 4);
	}

}
