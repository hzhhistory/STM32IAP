
#include "iap.h"
#include"LM3644YFFR.h"
#include"SEGGER_RTT.h"
#include "stm32f10x_flash.h"
#include"stm32f10x_rcc.h"

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
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);

    for (i = 0; i < len; i++)
    {
        flashFlag = FLASH_ProgramOptionByteData(addBase + i, data[i]);
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

// uint32_t getSysTick(void)
// {
//     return 0;
// }




uint32_t sysTickMs = 0;
void sysTickInit(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1)
        {
            __NOP();
        }
    }
}

void SysTick_Handler(void)
{
    sysTickMs++;
}

uint32_t getSysTick(void)
{
    // static uint64_t count = 0;
    // count++;
    // return count >> 13;

    return sysTickMs;
}

int main()
{
    // SEGGER_RTT_Init();
    // SEGGER_RTT_printf(0,"RTT test\r\n");
    sysTickInit();
 iapInit(getMessage,
             sendMessage,
             debug,
             custom
#ifdef BOOT
                 ,
             getSysTick,
             flashWrite,
             flashErase
#endif
);
    while (1)
    {
        iapHandle();
    }
}
