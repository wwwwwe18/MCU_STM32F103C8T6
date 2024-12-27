#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[] = {2024, 12, 25, 16, 53, 14};

void MyRTC_SetTime(void);

void MyRTC_Init(void)
{
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
	
	// Init
	// Read data from the specified Data Backup Register
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		// **********************************
		// 3. LSE
		// **********************************
		// Configure the External Low Speed oscillator (LSE)
		RCC_LSEConfig(RCC_LSE_ON);							// LSE oscillator ON
		
		// Check whether the specified RCC flag is set or not
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);	// Wait until LSE oscillator clock ready
		
		// **********************************
		// 4. RTC
		// **********************************
		// Configure the RTC clock (RTCCLK)
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);				// LSE selected as RTC clock
		
		// Enable the RTC clock
		RCC_RTCCLKCmd(ENABLE);
		
		// Wait until the RTC registers (RTC_CNT, RTC_ALR and RTC_PRL) are synchronized with RTC APB clock
		RTC_WaitForSynchro();
		
		// Wait until last write operation on RTC registers has finished
		RTC_WaitForLastTask();
		
		// **********************************
		
		// To write in the RTC_PRL, RTC_CNT, RTC_ALR registers, the peripheral must enter
		// Configuration mode. This is done by setting the CNF bit in the RTC_CRL register
		// RTC_EnterConfigMode() & RTC_ExitConfigMode() are included in RTC_SetPrescaler
		
		// Set the RTC prescaler value
		RTC_SetPrescaler(32768 - 1);
		
		// Wait until last write operation on RTC registers has finished
		RTC_WaitForLastTask();
		
		// **********************************
		
		/*
		// 1735145594
		// Wed Dec 25 2024 16:53:14 GMT+0000
		
		// Set the RTC counter value
		RTC_SetCounter(1735145594);
		
		// Wait until last write operation on RTC registers has finished
		RTC_WaitForLastTask();
		*/
		
		MyRTC_SetTime();
		
		// Write user data to the specified Data Backup Register
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	// Init: BKP_DR1 = 0xA5A5
	}
	// If BKP_DR1 = 0xA5A5, init already
	else
	{
		// Wait until the RTC registers (RTC_CNT, RTC_ALR and RTC_PRL) are synchronized with RTC APB clock
		RTC_WaitForSynchro();
		
		// Wait until last write operation on RTC registers has finished
		RTC_WaitForLastTask();
	}
}

void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year	= MyRTC_Time[0] - 1900;	// Years since 1900
	time_date.tm_mon	= MyRTC_Time[1] - 1;	// Months since January, 0 to 11
	time_date.tm_mday	= MyRTC_Time[2];
	time_date.tm_hour	= MyRTC_Time[3];
	time_date.tm_min	= MyRTC_Time[4];
	time_date.tm_sec	= MyRTC_Time[5];
	
	time_cnt = mktime(&time_date) -  0 * 60 * 60;	// GMT+0
	
	// Set the RTC counter value
	RTC_SetCounter(time_cnt);
	
	// Wait until last write operation on RTC registers has finished
	RTC_WaitForLastTask();
}

void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	// Get the RTC counter value
	time_cnt = RTC_GetCounter() + 0 * 60 * 60;		// GMT+0
	
	time_date = *localtime(&time_cnt);
	
	MyRTC_Time[0] = time_date.tm_year + 1900;	// Years since 1900
	MyRTC_Time[1] = time_date.tm_mon + 1;		// Months since January, 0 to 11
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}
