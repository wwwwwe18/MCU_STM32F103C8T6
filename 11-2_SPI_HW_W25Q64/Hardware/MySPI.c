#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue)
{
	// Sets or clears the selected data port bit
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

/*
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
*/

void MySPI_Init(void)
{
	// **************************************************
	// 1. RCC Clock
	// **************************************************
	// Enable the High Speed APB (APB2) peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);	// SPI1

	// **************************************************
	// 2. GPIO
	// **************************************************
	// GPIO Init structure
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;			// Master outputs - output push-pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;					// SS
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	// Initialize the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;			// Master outputs - alternate function push-pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;		// SCK, MOSI
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	// Initialize the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;				// Master input - input pull-up
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					// MISO
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	// Initialize the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// **************************************************
	// 3. SPI
	// **************************************************
	// SPI Init structure
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;							// SPI operating mode - master
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		// SPI bidirectional data mode
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;						// Data transfers start from MSB bit
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	// 72MHz / 128
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;								// SPI mode 0, Serial clock steady state
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;							// SPI mode 0, Clock active edge for the bit capture
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;								// NSS signal is managed by hardware (NSS pin) or by software using the SSI bit
	SPI_InitStruct.SPI_CRCPolynomial = 7;								// Polynomial used for the CRC calculation
	
	// Initialize the SPIx peripheral according to the specified parameters in the SPI_InitStruct
	SPI_Init(SPI1, &SPI_InitStruct);
	
	// Enable the specified SPI peripheral
	SPI_Cmd(SPI1, ENABLE);
	
	// Default: select no slave
	MySPI_W_SS(1);	// Select no slave
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
	
	/*
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
	*/
	
	// SPI_I2S_FLAG_TXE: Transmit buffer empty flag
	// TXE: Transmit buffer empty
	// 0: Tx buffer not empty
	// 1: Tx buffer empty
	
	// When it is set, this flag indicates that the Tx buffer is empty and the next data to be
	// transmitted can be loaded into the buffer. The TXE flag is cleared when writing to the
	// SPI_DR register.
	
	// Wait until TXE = 1 (Tx buffer empty)
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);	// Check whether the specified SPI/I2S flag is set or not
	
	// Software writes data into SPI_DR (TDR)
	SPI_I2S_SendData(SPI1, ByteSend);								// Transmit a Data through the SPIx/I2Sx peripheral
	
	// SPI_I2S_FLAG_RXNE: Receive buffer not empty flag
	// RXNE: Receive buffer not empty
	// 0: Rx buffer empty
	// 1: Rx buffer not empty
	
	// When set, this flag indicates that there are valid received data in the Rx buffer. It is cleared
	// when SPI_DR is read.
	
	// Wait until RXNE = 1 (Rx buffer not empty) <=> Byte received, ByteSend completed
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
	
	// Software reads data into SPI_DR (RDR)
	return SPI_I2S_ReceiveData(SPI1);
}
