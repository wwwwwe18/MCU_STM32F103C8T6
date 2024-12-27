#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)
{
	// Sets or clears the selected data port bit
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	// Sets or clears the selected data port bit
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	// Sets or clears the selected data port bit
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

uint8_t MySPI_R_MISO(void)
{
	// Read the specified input port pin
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

void MySPI_Init(void)
{
	// **************************************************
	// 1. RCC Clock
	// **************************************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// **************************************************
	// 2. GPIO
	// **************************************************
	// GPIO Init structure
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;						// Master outputs - output push-pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;	// CS, CLK, DI (MOSI)
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	// Initialize the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;							// Master input - input pull-up
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;								// CS, CLK, DO (MISO)
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	// Initialize the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// **************************************************
	// 3. SPI
	// **************************************************
	MySPI_W_SS(1);	// Select no slave
	MySPI_W_SCK(0);	// SPI mode 0
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);	// Select slave
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);	// Select no slave
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;
	
	// SPI mode 0
	for(i = 0; i < 8; i++)
	{
																// Default: SCK = L	
		MySPI_W_MOSI(ByteSend & (0x80 >> i));					// Master sends bit (MSB to LSB)
																// Slave sends bit (MSB to LSB)
		MySPI_W_SCK(1);											// SCK = H
		if(MySPI_R_MISO() == 1){ByteReceive |= (0x80 >> i);}	// Master receives bit from Slave (MSB to LSB)
																// Slave receives bit from Master
		MySPI_W_SCK(0);											// SCK = L
	}
	
	return ByteReceive;
}

/*
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i;
	
	// SPI mode 0
	for(i = 0; i < 8; i++)
	{
																// Default: SCK = L	
		MySPI_W_MOSI(ByteSend & 0x80);							// Master sends bit (MSB to LSB)
		ByteSend <<= 1;											// Right shift
																// Slave sends bit (MSB to LSB)
		MySPI_W_SCK(1);											// SCK = H
		if(MySPI_R_MISO() == 1){ByteSend |= 0x01;}				// Master receives bit from Slave (MSB to LSB)
																// Slave receives bit from Master
		MySPI_W_SCK(0);											// SCK = L
	}
	
	return ByteSend;
}
*/
