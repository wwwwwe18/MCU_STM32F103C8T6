#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init(void)
{
	MySPI_Init();
}

// Read manufacturer ID and device ID
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_JEDEC_ID);			// Master sends instruction to slave (JEDEC ID)
	
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// Master receives data from slave (Manufacturer ID)
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// Master receives data from slave (Device ID H)
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	// Master receives data from slave (Device ID L)
	
	MySPI_Stop();
}

// Write Enable
void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}

void W25Q64_WaitBusy(void)
{
	uint32_t Timeout = 100000;
	
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);				// Read Status Register-1
	
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)	// Bit[0] - busy
	{
		Timeout--;
		
		if(Timeout == 0)
		{
			break;
		}
	}
	
	MySPI_Stop();
}

// Page program
void W25Q64_PageProgram(uint32_t Addr, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();					// Write Enable
	
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);	// Page program
	
	MySPI_SwapByte(Addr >> 16);				// Byte H
	MySPI_SwapByte(Addr >> 8);				// Byte M
	MySPI_SwapByte(Addr);					// Byte L
	
	for(i = 0; i < Count; i++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

// Sector Erase (4KB)
void W25Q64_SectorErase(uint32_t Addr)
{
	W25Q64_WriteEnable();						// Write Enable
	
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);	// Sector Erase (4KB)
	
	MySPI_SwapByte(Addr >> 16);					// Byte H
	MySPI_SwapByte(Addr >> 8);					// Byte M
	MySPI_SwapByte(Addr);						// Byte L
	
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

// Read Data
void W25Q64_ReadData(uint32_t Addr, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_READ_DATA);	// Read Data
	
	MySPI_SwapByte(Addr >> 16);			// Byte H
	MySPI_SwapByte(Addr >> 8);			// Byte M
	MySPI_SwapByte(Addr);				// Byte L
	
	for(i = 0; i < Count; i++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	
	MySPI_Stop();
}
