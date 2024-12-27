#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDR	0xD0			// 1101 000 0, Slave address + R/W (W here)

void MPU6050_WriteReg(uint8_t RegAddr, uint8_t Data)
{
	MyI2C_Start();						// Start
	MyI2C_SendByte(MPU6050_ADDR);		// MPU6050 address + R/W (W here)
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddr);			// Register address
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);				// Master sends data
	MyI2C_ReceiveAck();
	MyI2C_Stop();						// Stop
}

uint8_t MPU6050_ReadReg(uint8_t RegAddr)
{
	uint8_t Data;
	
	MyI2C_Start();						// Start
	MyI2C_SendByte(MPU6050_ADDR);		// MPU6050 address + R/W (W here)
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddr);			// Register address
	MyI2C_ReceiveAck();
	
	MyI2C_Start();						// Start
	MyI2C_SendByte(MPU6050_ADDR | 0x01);// MPU6050 address + R/W (R here)
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();			// Master receives data
	MyI2C_SendAck(1);					// Only one byte in this case => no ACK (1)
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_Init(void)
{
	MyI2C_Init();
	
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
