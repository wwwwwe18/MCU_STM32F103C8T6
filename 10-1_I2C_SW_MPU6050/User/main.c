#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"

uint8_t ID;
int16_t AX, AY, AZ, GX, GY, GZ;

int main(void)
{

	OLED_Init();
	MPU6050_Init();
	
	/*
	MPU6050_WriteReg(0x6B, 0x00);			// PWR_MGMT_1 (Power Management 1), turn off sleep mode (bit6 = 0)
	MPU6050_WriteReg(0x19, 0xAA);			// SMPLRT_DIV (Sample Rate Divider)
	
	uint8_t ID = MPU6050_ReadReg(0x75);		// WHO_AM_I
	uint8_t Temp = MPU6050_ReadReg(0x19);	// SMPLRT_DIV
	
	OLED_ShowHexNum(1, 1, Temp, 2);
	*/
	
	OLED_ShowString(1, 1, "ID:");
	
	ID = MPU6050_GetID();
	OLED_ShowHexNum(1, 4, ID, 2);
	
	while(1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		
		OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
	}
	
	// AZ = 1872 <=> 1g
	// (1872 / 32768) * 16g = 0.914g

}
