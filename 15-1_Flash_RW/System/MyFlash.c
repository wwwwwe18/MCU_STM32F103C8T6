#include "stm32f10x.h"                  // Device header

uint32_t MyFlash_ReadWord(uint32_t Address)
{
	// #define     __IO    volatile                  /*!< defines 'read / write' permissions   */
	
	return *((__IO uint32_t *)(Address));
}

uint16_t MyFlash_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));
}

uint8_t MyFlash_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t *)(Address));
}

void MyFlash_EraseAllPages(void)
{
	// Unlock the FLASH Program Erase Controller
	FLASH_Unlock();
	
	// Erase all FLASH pages
	FLASH_EraseAllPages();
	
	// Lock the FLASH Program Erase Controller
	FLASH_Lock();
}

void MyFlash_ErasePage(uint32_t PageAddr)
{
	// Unlock the FLASH Program Erase Controller
	FLASH_Unlock();
	
	// Erase a specified FLASH page
	FLASH_ErasePage(PageAddr);
	
	// Lock the FLASH Program Erase Controller
	FLASH_Lock();
}

void MyFlash_ProgramWord(uint32_t Address, uint32_t Data)
{
	// Unlock the FLASH Program Erase Controller
	FLASH_Unlock();
	
	// Program a word at a specified address
	FLASH_ProgramWord(Address, Data);
	
	// Lock the FLASH Program Erase Controller
	FLASH_Lock();
}

void MyFlash_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
	// Unlock the FLASH Program Erase Controller
	FLASH_Unlock();
	
	// Program a half word at a specified address
	FLASH_ProgramHalfWord(Address, Data);
	
	// Lock the FLASH Program Erase Controller
	FLASH_Lock();
}
