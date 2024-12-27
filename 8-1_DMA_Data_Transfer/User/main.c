#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"

/*
uint8_t aa = 0x66;
const uint8_t bb = 0x88;	// Flash

#define ADC1_DR (uint32_t *)0x4001244C
*/

uint8_t DataA[] = {0x01, 0x02, 0x03, 0x04};
uint8_t DataB[] = {0, 0, 0, 0};

int main(void)
{

	OLED_Init();
	
	OLED_ShowString(1, 1, "DataA");
	OLED_ShowString(3, 1, "DataB");
	
	// Address
	OLED_ShowHexNum(1, 8, (uint32_t)DataA, 8);
	OLED_ShowHexNum(3, 8, (uint32_t)DataB, 8);
	
	MyDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4);
	
	/*
	OLED_ShowHexNum(1, 1, aa, 2);
	OLED_ShowHexNum(2, 1, (uint32_t)&aa, 8);	// 32-bit address (in SRAM)
	
	OLED_ShowHexNum(3, 1, bb, 2);
	OLED_ShowHexNum(4, 1, (uint32_t)&bb, 8);	// const (in Flash)
	
	OLED_ShowHexNum(2, 1, (uint32_t)&ADC1->DR, 8);	// APB2 ADC1 ADC_DR, 0x4001 2400 + 0x4C = 0x4001 244C
	
//	*ADC1_DR
	*/
	
	while(1)
	{
		DataA[0]++;
		DataA[1]++;
		DataA[2]++;
		DataA[3]++;
		
		// DataA
		OLED_ShowHexNum(2, 1,  DataA[0], 2);	// Tips: Alt + Click to select all
		OLED_ShowHexNum(2, 4,  DataA[1], 2);
		OLED_ShowHexNum(2, 7,  DataA[2], 2);
		OLED_ShowHexNum(2, 10, DataA[3], 2);
		
		// DataB
		OLED_ShowHexNum(4, 1,  DataB[0], 2);
		OLED_ShowHexNum(4, 4,  DataB[1], 2);
		OLED_ShowHexNum(4, 7,  DataB[2], 2);
		OLED_ShowHexNum(4, 10, DataB[3], 2);
		
		Delay_ms(1000);
		
		MyDMA_Transfer();
		
		// DataA
		OLED_ShowHexNum(2, 1,  DataA[0], 2);
		OLED_ShowHexNum(2, 4,  DataA[1], 2);
		OLED_ShowHexNum(2, 7,  DataA[2], 2);
		OLED_ShowHexNum(2, 10, DataA[3], 2);
		
		// DataB
		OLED_ShowHexNum(4, 1,  DataB[0], 2);
		OLED_ShowHexNum(4, 4,  DataB[1], 2);
		OLED_ShowHexNum(4, 7,  DataB[2], 2);
		OLED_ShowHexNum(4, 10, DataB[3], 2);
		
		Delay_ms(1000);
	}

}
