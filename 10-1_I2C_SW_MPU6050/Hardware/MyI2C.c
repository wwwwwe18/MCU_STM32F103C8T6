#include "stm32f10x.h"                  // Device header
#include "Delay.h"

// SCL write
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);	// Set or clear the selected data port bit
	Delay_us(10);											// Delay 10us
}

// SDA write
void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);	// Set or clear the selected data port bit
	Delay_us(10);											// Delay 10us
}

// SDA read
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);	// Read the specified input port pin
	Delay_us(10);											// Delay 10us
	
	return BitValue;
}

void MyI2C_Init(void)
{
	// **************************************************
	// 1. RCC clock
	// **************************************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// **************************************************
	// 2. GPIO
	// **************************************************
	// GPIO Init structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;	// Output Open Drain (can only be driven L, not H)
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	// Initialize the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// Set the selected data port bits
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
	
	// SCL 1
	// SDA 1
}

// Start
void MyI2C_Start(void)
{
	// SCL x110
	// SDA 1100
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

// Stop
void MyI2C_Stop(void)
{
	// SCL x11
	// SDA 001
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

// SendByte
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	
	for(i = 0; i < 8; i++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));	// Master sends data on SDA (MSB to LSB)
		
		// SCL = L already set before
		MyI2C_W_SCL(1);						// Slave reads data
		MyI2C_W_SCL(0);						// Next
	}
}

// ReceiveByte
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	
	// SCL = L already set before
	
	MyI2C_W_SDA(1);	// Master releases SDA; Slave sends data on SDA
	
	for(i = 0; i < 8; i++)
	{
		MyI2C_W_SCL(1);
		
		if(MyI2C_R_SDA() == 1)
		{
			Byte |= (0x80 >> i);	// Master reads data on SDA (MSB to LSB)
		}
		
		MyI2C_W_SCL(0);				// Next
	}
	
	return Byte;
}

// Send ACK
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);	// Master sends AckBit on SDA
	
	// SCL = L already set before
	MyI2C_W_SCL(1);			// Slave reads AckBit
	MyI2C_W_SCL(0);
}

// Receive ACK
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	
	// SCL = L already set before
	
	MyI2C_W_SDA(1);			// Master releases SDA; Slave sends AckBit on SDA
	MyI2C_W_SCL(1);			// Master reads AckBit
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	
	return AckBit;
}
