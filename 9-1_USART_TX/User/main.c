#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{

	OLED_Init();
	Serial_Init();
	
	Serial_SendByte(0x41);
	Serial_Printf("\r\n");
	
	uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45};
	Serial_SendArray(MyArray, 4);
	
	Serial_SendString("\r\nNum1 = ");
	
	Serial_SendNumber(111, 5);
	
	printf("\r\nNum2 = %d", 666);
	
	char String[100];
	sprintf(String, "\r\nNum3 = %d", 666);
	Serial_SendString(String);
	
	Serial_Printf("\r\nNum4 = %d", 666);
	Serial_Printf("\r\n你好，世界");
	Serial_Printf("\r\n");
	
	while(1)
	{
		
	}

}
