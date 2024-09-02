#ifndef __FLASH_OPEATE_H__
#define __FLASH_OPEATE_H__
#include"stm32f10x_flash.h"

FLASH_Status Flash_Ease(uint32_t PageAddress);

FLASH_Status FLASH_ProgramByte(uint32_t address, uint8_t data);




#endif