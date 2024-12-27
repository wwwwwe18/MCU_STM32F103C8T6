#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

void Serial_Init(void)
{
	
	// **************************************************
	// 1. RCC clock
	// **************************************************
	// Enable the High Speed APB (APB2) peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	// USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOA
	
	// **************************************************
	// 2. GPIO
	// **************************************************
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	// TX - Alternate Function Pull Push
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// **************************************************
	// 3. USART
	// **************************************************
	// USART Init Structure  
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;						// USART communication baud rate
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// Hardware flow control mode is disabled
	USART_InitStruct.USART_Mode = USART_Mode_Tx;				// Transmit mode is enabled
	USART_InitStruct.USART_Parity = USART_Parity_No;			// Parity mode - No
	USART_InitStruct.USART_StopBits = USART_StopBits_1;			// Number of stop bits transmitted
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	// Number of data bits transmitted in a frame
	
	// Initialize the USARTx peripheral according to the specified parameters in the USART_InitStruct
	USART_Init(USART1, &USART_InitStruct);
	
	// Enable the specified USART peripheral
	USART_Cmd(USART1, ENABLE);
	
}

void Serial_SendByte(uint8_t Byte)
{
	
	// Transmit single data through the USARTx peripheral (Data writes to TDR)
	USART_SendData(USART1, Byte);
	
	// Transmit data register empty flag USART_FLAG_TXE
	// TXE: Transmit data register empty
	// 0: Data is not transferred to the shift register
	// 1: Data is transferred to the shift register)
	// It is cleared by a write to the USART_DR register (no reset needed)
	
	// Wait
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// Check whether the specified USART flag is set or not
	
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	
	for(i = 0; i < Length; i++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint16_t i;
	
	for(i = 0; String[i] != '\0'; i++)
	{
		Serial_SendByte(String[i]);
	}
}

// Result = X^Y
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	
	while(Y--)
	{
		Result *= X;
	}	
	
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint16_t i;
	
	for(i = 0; i < Length; i++)
	{
		Serial_SendByte((Number / Serial_Pow(10, Length - i - 1)) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}
