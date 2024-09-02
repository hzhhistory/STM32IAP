#include"Flash_opeate.h"

FLASH_Status Flash_Ease(uint32_t PageAddress)
{
    FLASH_Status flage_ease=FLASH_COMPLETE;
    FLASH_Unlock();
    flage_ease=FLASH_ErasePage(PageAddress);
    FLASH_Lock();
    return flage_ease;
}


 