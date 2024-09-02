#include "iap.h"
#include"SEGGER_RTT.h"
#include"Delay.h"
#include "stm32f10x_flash.h"
#include"misc.h"
#include"Serial.h"
#include"amovlink.h"
#include"stdbool.h"
#include"Flash_opeate.h"

static AMOVLINK_SYS_MESSAGE_T amov_message;
uint8_t flage;
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
    volatile FLASH_Status flashFlag;
    uint32_t i = 0;
    uint16_t halfWord;

  
    
    FLASH_Unlock();
   FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    for (i = 0; i < len; i += 2)
    {
     
        halfWord = (data[i+1] << 8) | data[i];   // 将两个字节打包成一个半字
       
        flashFlag = FLASH_ProgramHalfWord(addBase + (i/2)* 2, halfWord);
  
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
    size=15360;
    uint32_t page=size/1024;
    FLASH_Unlock();
    for(int i=0;i<page;i++)
    {
    flashFlag = Flash_Ease(add+i*0x400);
    if (flashFlag != FLASH_COMPLETE)
    {
        ret = IAP_STATE_FAILD;
        break;
    }
    }
     FLASH_Lock();
    return ret;
}


void amovlink_callbacl(AMOVLINK_SYS_MESSAGE_T *msg)
{
    SEGGER_RTT_printf(0,"2\r\n");
    amov_message.cmd=msg->cmd;
    amov_message.len=msg->len;
    amov_message.target=msg->target;
    amov_message.source=msg->source;
    memcpy(amov_message.data,msg->data,msg->len);
    flage=1;

}

bool getMessage(void *msg)
{
  
    if(flage)
    {  SEGGER_RTT_printf(0,"3\r\n");
        ((IAP_MESSAGE_T *)msg)->type = amov_message.cmd;
        ((IAP_MESSAGE_T *)msg)->len = amov_message.len;
        memcpy(((IAP_MESSAGE_T *)msg)->data, amov_message.data, amov_message.len);
        flage=0;
        return true;
    }
    else{
        return false;
    }
    
}

bool sendMessage(void *msg)
{
        static AMOVLINK_SYS_MESSAGE_T sysmsg;
        sysmsg.cmd = ((IAP_MESSAGE_T *)msg)->type;
        sysmsg.len = ((IAP_MESSAGE_T *)msg)->len;
        sysmsg.target = 0X01;
        memcpy(sysmsg.data, ((IAP_MESSAGE_T *)msg)->data, sysmsg.len);

        return amovLinkSendMsg(&sysmsg);
      
}

void debug(const char *data, ...)
{
    return;
}

void custom(void)
{
    return;
}

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
    return sysTickMs;
}

void jumpFailCB(void)
{
    
}


AMOVLINK_REG_MOUDLE_CB(iap,amovlink_callbacl);


int main()
{
/*sysTickInit -------------------*/
sysTickInit();

/*USART INIT ---------------*/
Usart_Init();

/*amovlink -----------------*/
AMOVLINK_IO_T amov_link_temp;
amovLinkInit();
amovLinkIoReg(&amov_link_temp, getUartByte, sendUartBytes, NULL);

/*IAP INIT ---------------------*/    
 iapInit(getMessage,
         sendMessage,
             debug,
             custom
#ifdef BOOT
                 ,
             getSysTick,
             flashWrite,
             flashErase,
             jumpFailCB
#endif
);

    while (1)
    {
    
        iapHandle();
       
        amovLinkHandle();
        
    }
}
