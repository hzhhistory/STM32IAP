
#include "iap.h"

#include "stm32f4xx_flash.h"

/**
 * The function flashWrite writes data to flash memory and returns the status of the operation.
 *
 * @param addBase The base address where the data will be written in flash memory.
 * @param data A pointer to the data that needs to be written to flash memory.
 * @param len The parameter "len" represents the length of the data to be written to flash memory.
 *
 * @return an IAP_STATE_T enum value.
 */
IAP_STATE_T flashWrite(uint32_t addBase, uint8_t *data, uint32_t len)
{
    IAP_STATE_T ret = IAP_STATE_SUCCEED;
    FLASH_Status flashFlag;
    uint32_t i = 0;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    for (i = 0; i < len; i++)
    {
        flashFlag = FLASH_ProgramByte(addBase + i, data[i]);
        if (flashFlag != FLASH_COMPLETE)
        {
            ret = IAP_STATE_FAILD;
            break;
        }
    }

    FLASH_Lock();
    return ret;
}

/**
 * The function flashErase erases a specified sector of flash memory and returns the status of the
 * operation.
 *
 * @param sector The sector parameter is the sector number of the flash memory that needs to be erased.
 *
 * @return an IAP_STATE_T value, which is the result of the flash erase operation.
 */
IAP_STATE_T flashErase(uint32_t add, uint32_t size)
{
    IAP_STATE_T ret = IAP_STATE_SUCCEED;
    FLASH_Status flashFlag;
    FLASH_Unlock();

    // flashFlag = FLASH_EraseSector(sector, VoltageRange_3);
    if (flashFlag != FLASH_COMPLETE)
    {
        ret = IAP_STATE_FAILD;
    }

    FLASH_Lock();
    return ret;
}

bool getMessage(void *msg)
{
    return false;
}

bool sendMessage(void *msg)
{
    return false;
}

void debug(const char *data, ...)
{
    return;
}

void custom(void)
{
    return;
}

uint32_t getSysTick(void)
{
    return 0;
}

int main()
{
    iapInit(
#ifdef BOOT
        getSysTick, flashWrite, flashErase,
#endif
        getMessage, sendMessage,
        debug, custom);

    while (1)
    {
        iapHandle();
    }
}
