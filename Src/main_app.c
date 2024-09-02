#include"stm32f10x.h"
#include"SEGGER_RTT.h"
#include"LM3644YFFR.h"
#include"amovlink.h"
#include"Serial.h"
#include"LM3644YFFRRE.h"
#include"iap.h"

static AMOVLINK_SYS_MESSAGE_T amov_message;
uint8_t flage;
uint8_t LED_FM;

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

void amovlink_callback(AMOVLINK_SYS_MESSAGE_T *msg)
{
    amov_message.cmd=msg->cmd;
    amov_message.len=msg->len;
    amov_message.target=msg->target;
    amov_message.source=msg->source;
    memcpy(amov_message.data,msg->data,msg->len);
    flage=1;
}



AMOVLINK_REG_MOUDLE_CB(iap,amovlink_callback);

int main()
{
/*USART Init-----------------------------------------------------------*/

    Usart_Init();

/*amovLink Init-------------------------------------------------------*/

    AMOVLINK_IO_T amov_link_temp;
    amovLinkInit();
    amovLinkIoReg(&amov_link_temp, getUartByte, sendUartBytes, NULL);

/*IAP Init-----------------------------------------------------------*/
  iapInit(getMessage,
                sendMessage,
                debug,
                custom);

/*红外灯的初始化--------------------------------------------------------*/    

    LM3644YFFR_INIT();
    LM3644YFFRRE_INIT();

    while(1)
    {
        LM3644YFFR_LEDFlash(Infrared_8);
        LM3644YFFRRE_LEDFlash(Infrared_8);

        iapHandle();
        amovLinkHandle();

       
        
    }


}