#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDR	0xD0			// 1101 000 0, Slave address + R/W (W here)

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout = 10000;
	
	// Check event
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout--;
		
		if(Timeout == 0)
		{
			break;
		}
	}
}

void MPU6050_WriteReg(uint8_t RegAddr, uint8_t Data)
{
	/*
	MyI2C_Start();						// Start
	MyI2C_SendByte(MPU6050_ADDR);		// MPU6050 address + R/W (W here)
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddr);			// Register address
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);				// Master sends data
	MyI2C_ReceiveAck();
	MyI2C_Stop();						// Stop
	*/
	
	// *************************************************
	// 1. Start
	// *************************************************
	// Generate I2Cx communication START condition
	I2C_GenerateSTART(I2C2, ENABLE);
	
	// Check EV5
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	// *************************************************
	// 2. Slave address + W
	// *************************************************
	// Transmit the address byte to select the slave device
	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
	
	// ACK generated
	
	// Check EV6 - TX
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	// No need to wait for EV8_1
	
	// *************************************************
	// 3. Send register address, data (one byte)
	// *************************************************
	I2C_SendData(I2C2, RegAddr);	// Register address
	
	// Check EV8
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);	// Very quick
	
	I2C_SendData(I2C2, Data);		// Master sends data
	
	// Check EV8_2
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	// *************************************************
	// 4. Stop
	// *************************************************
	// Generate I2Cx communication STOP condition
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t MPU6050_ReadReg(uint8_t RegAddr)
{
	uint8_t Data;
	
//	MyI2C_Start();						// Start
//	MyI2C_SendByte(MPU6050_ADDR);		// MPU6050 address + R/W (W here)
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RegAddr);			// Register address
//	MyI2C_ReceiveAck();
//	
//	MyI2C_Start();						// Start
//	MyI2C_SendByte(MPU6050_ADDR | 0x01);// MPU6050 address + R/W (R here)
//	MyI2C_ReceiveAck();
//	Data = MyI2C_ReceiveByte();			// Master receives data
//	MyI2C_SendAck(1);					// Only one byte in this case => no ACK (1)
//	MyI2C_Stop();
	
	// *************************************************
	// 1. Start
	// *************************************************
	// Generate I2Cx communication START condition
	I2C_GenerateSTART(I2C2, ENABLE);
	
	// Check EV5
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	// *************************************************
	// 2. Slave address + W
	// *************************************************
	// Transmit the address byte to select the slave device
	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
	
	// ACK generated
	
	// Check EV6 - TX
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	// No need to wait for EV8_1
	
	// *************************************************
	// 3. Send register address
	// *************************************************
	I2C_SendData(I2C2, RegAddr);	// Register address
	
	// (Optional) Check EV8
	// Check EV8_2
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	// *************************************************
	// 4. Start
	// *************************************************
	// Generate I2Cx communication START condition
	I2C_GenerateSTART(I2C2, ENABLE);
	
	// Check EV5
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	// *************************************************
	// 5. Slave address + R
	// *************************************************
	// Transmit the address byte to select the slave device
	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Receiver);
	
	// ACK generated
	
	// Check EV6 - RX
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	// No need to wait for EV6_1
	
	// *************************************************
	// Send byte: to generate ACK and STOP before sending data
	// *************************************************
	// Disable the specified I2C acknowledge feature
	I2C_AcknowledgeConfig(I2C2, DISABLE);	// ACK = 0
	
	// Generate I2Cx communication STOP condition
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	// *************************************************
	// 6. Receive data (one byte)
	// *************************************************
	// Check EV7
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	
	// Return the most recent received data by the I2Cx peripheral
	Data = I2C_ReceiveData(I2C2);
	
	// *************************************************
	// Default: ACK = 1 (for multiple bytes)
	// *************************************************
	// Enable the specified I2C acknowledge feature 
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	return Data;
}

void MPU6050_Init(void)
{
//	MyI2C_Init();
	
	// *************************************************
	// 1. RCC clock
	// *************************************************
	// Enable the Low Speed APB (APB1) peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);	// I2C2
	
	// Enable the High Speed APB (APB2) peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOB
	
	// *************************************************
	// 2. GPIO
	// *************************************************
	// GPIO Init structure
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;	// Alternate function open drain
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	// Initialize the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// *************************************************
	// 3. I2C
	// *************************************************
	// I2C Init structure
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_ClockSpeed = 50000;			// Must be set to a value lower than 400kHz
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	// I2C fast mode (100kHz - 400kHz) Tlow/Thigh = 2
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;		
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	// Slave
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;									// Slave
	
	// Initialize the I2Cx peripheral according to the specified  parameters in the I2C_InitStruct
	I2C_Init(I2C2, &I2C_InitStruct);
	
	// Enable the specified I2C peripheral
	I2C_Cmd(I2C2, ENABLE);
	
	// ******************************
	// PWR_MGMT_1
	// ******************************
	// Turn off sleep mode (bit6 = 0)
	
	// It is highly recommended that the device be configured to use one of the gyroscopes (or an external clock source) as the clock reference for improved stability
	// 001: PLL with X axis gyroscope reference
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	
	// ******************************
	// PWR_MGMT_2
	// ******************************
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	
	// ******************************
	// MPU6050_SMPLRT_DIV
	// ******************************
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		// Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
	
	// ******************************
	// MPU6050_CONFIG
	// ******************************
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);			// EXT_SYNC_SET = 0: Input disabled; DLPF_CFG = 3b'110: 
	
	// ******************************
	// MPU6050_GYRO_CONFIG
	// ******************************
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	// FS_SEL = 2b'11: +/- 2000 degrees / sec
	
	// ******************************
	// MPU6050_ACCEL_CONFIG
	// ******************************
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);	// AFS_SEL = 2'b11: +/- 16g; ACCEL_HPF = 3'b000: Reset
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint16_t DataH, DataL;
	
	// ******************************
	// Accelerometer Measurements
	// ******************************
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	// ******************************
	// Gyroscope Measurements
	// ******************************
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}
