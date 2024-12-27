#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[4];
uint8_t Serial_RxFlag;

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
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;		// RX - Input Pull Up
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// **************************************************
	// 3. USART
	// **************************************************
	// USART Init Structure  
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;						// USART communication baud rate
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// Hardware flow control mode is disabled
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;// Transmit mode is enabled; Receive mode is enabled
	USART_InitStruct.USART_Parity = USART_Parity_No;			// Parity mode - No
	USART_InitStruct.USART_StopBits = USART_StopBits_1;			// Number of stop bits transmitted
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	// Number of data bits transmitted in a frame
	
	// Initialize the USARTx peripheral according to the specified parameters in the USART_InitStruct
	USART_Init(USART1, &USART_InitStruct);
	
	// **************************************************
	// 4. Interrupt
	// **************************************************
	// Enable the specified USART interrupts
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	// USART_IT_RXNE: Receive Data register not empty interrupt
	
	// **************************************************
	// 5. NVIC
	// **************************************************
	// Configure the priority grouping: pre-emption priority and subpriority
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// NVIC Init Structure
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;			// IRQ channel to be enabled
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			// IRQ channel defined in NVIC_IRQChannel will be enabled
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	// Pre-emption priority for the IRQ channel specified in NVIC_IRQChannel (0 - 3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;			// Subpriority level for the IRQ channel specified in NVIC_IRQChannel (0 - 3)
	
	// Initialize the NVIC peripheral according to the specified parameters in the NVIC_InitStruct
	NVIC_Init(&NVIC_InitStruct);
	
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

void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendArray(Serial_TxPacket, 4);
	Serial_SendByte(0xFE);
}

uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		
		return 1;
	}
	
	return 0;
}

void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	
	// Check whether the specified USART interrupt has occurred or not
	// USART_IT_RXNE: Receive Data register not empty interrupt
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		// Return the most recent received data by the USARTx peripheral
		uint8_t RxData = USART_ReceiveData(USART1);
		
		// FSM
		
		// State 0 - wait for header
		if(RxState == 0)
		{
			if(RxData == 0xFF)	// Header received
			{
				RxState = 1;
				
				pRxPacket = 0;
			}
			/*
			else
			{
				RxState = 0;
			}
			*/
		}
		// State 1 - receive data
		else if(RxState == 1)
		{
			Serial_RxPacket[pRxPacket] = RxData;
			pRxPacket++;
			
			if(pRxPacket >= 4)
			{
				RxState = 2;
			}
			/*
			else
			{
				RxState = 1;
			}
			*/
		}
		// State 2 - wait for tail
		else if(RxState == 2)
		{
			if(RxData == 0xFE)	// Tail received
			{
				RxState = 0;
				
				Serial_RxFlag = 1;
			}
			/*
			else
			{
				RxState = 2;
			}
			*/
		}
		
		// Clear the USARTx's interrupt pending bits
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
