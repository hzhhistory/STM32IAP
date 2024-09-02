#include"Serial.h"

uint8_t uart2RecvData[2048] = {0};    // 接收数据缓冲区
uint8_t uart2RecvFlag = 0;          // 接收完成标志位
uint8_t uart2RecvLen = 0;           // 接收的数据长度

uint8_t uart2SendData[1024] = {0};    // 发送数据缓冲区
uint8_t uart2SendFlag = 0;         


void Usart_Init()
{
    DMA_DeInit(DMA1_Channel6);
    DMA_DeInit(DMA1_Channel7);

    GPIO_InitTypeDef gpioBsaeConfig;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    gpioBsaeConfig.GPIO_Mode=GPIO_Mode_AF_PP;
    gpioBsaeConfig.GPIO_Pin=GPIO_Pin_2;
    gpioBsaeConfig.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpioBsaeConfig);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    gpioBsaeConfig.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    gpioBsaeConfig.GPIO_Pin=GPIO_Pin_3;
    gpioBsaeConfig.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpioBsaeConfig);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    USART_InitTypeDef uartConfig;
    uartConfig.USART_BaudRate=115200;
    uartConfig.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    uartConfig.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
    uartConfig.USART_Parity=USART_Parity_No;
    uartConfig.USART_StopBits=USART_StopBits_1;
    uartConfig.USART_WordLength=USART_WordLength_8b;

    USART_Init(USART2,&uartConfig);
    USART_ClearFlag(USART2, USART_FLAG_TC);
    USART_ClearITPendingBit(USART2, USART_IT_TC);

   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

   DMA_InitTypeDef dmaConfig;
   USART_DMACmd(USART2, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);

    //uart2chanl6_RX
   
   dmaConfig.DMA_BufferSize=sizeof(uart2RecvData);
   dmaConfig.DMA_DIR= DMA_DIR_PeripheralSRC;
   dmaConfig.DMA_M2M=DMA_M2M_Disable;
   dmaConfig.DMA_MemoryBaseAddr=(uint32_t)uart2RecvData;
   dmaConfig.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte ;
   dmaConfig.DMA_MemoryInc=DMA_MemoryInc_Enable;
   dmaConfig.DMA_Mode=DMA_Mode_Circular;
   dmaConfig.DMA_PeripheralBaseAddr=(uint32_t)&USART2->DR;
   dmaConfig.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
   dmaConfig.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
   dmaConfig.DMA_Priority=DMA_Priority_High;
    DMA_Init(DMA1_Channel6,&dmaConfig);
    DMA_Cmd(DMA1_Channel6,ENABLE);
    // DMA_ITConfig(DMA1_Channel6,DMA_IT_TC, ENABLE);
     //uart2chanl7_TX
   dmaConfig.DMA_BufferSize=0;
   dmaConfig.DMA_DIR= DMA_DIR_PeripheralDST;
   dmaConfig.DMA_M2M=DMA_M2M_Disable;
   dmaConfig.DMA_MemoryBaseAddr=(uint32_t)uart2SendData;
   dmaConfig.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte ;
   dmaConfig.DMA_MemoryInc=DMA_MemoryInc_Enable;
   dmaConfig.DMA_Mode=DMA_Mode_Normal;
   dmaConfig.DMA_PeripheralBaseAddr=(uint32_t)&USART2->DR;
   dmaConfig.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
   dmaConfig.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
   dmaConfig.DMA_Priority=DMA_Priority_High;
    DMA_Init(DMA1_Channel7,&dmaConfig);
    DMA_ITConfig(DMA1_Channel7,DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Channel7,ENABLE);

    NVIC_InitTypeDef nvicConfig;
    nvicConfig.NVIC_IRQChannel=DMA1_Channel6_IRQn;
    nvicConfig.NVIC_IRQChannelCmd=ENABLE;
    nvicConfig.NVIC_IRQChannelPreemptionPriority=1;
    nvicConfig.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&nvicConfig);
    DMA_ClearFlag(DMA1_FLAG_GL6);
    DMA_ClearITPendingBit(DMA1_IT_GL6);
    USART_Cmd(USART2, ENABLE);

    nvicConfig.NVIC_IRQChannel=DMA1_Channel7_IRQn;
    nvicConfig.NVIC_IRQChannelCmd=ENABLE;
    nvicConfig.NVIC_IRQChannelPreemptionPriority=1;
    nvicConfig.NVIC_IRQChannelSubPriority=2;
    NVIC_Init(&nvicConfig);
    DMA_ClearFlag(DMA1_FLAG_GL7);
    DMA_ClearITPendingBit(DMA1_IT_GL7);
    USART_Cmd(USART2, ENABLE);
}

// void DMA1_Channel6_IRQHandler(void)
// {
//      if(DMA_GetITStatus(DMA1_IT_TC6) != RESET)
//     {
//         DMA_ClearITPendingBit(DMA1_IT_TC6);   
//         DMA_Cmd(DMA1_Channel6, DISABLE);       
//         DMA1_Channel4->CNDTR=0;                                   
//     }
// }

void DMA1_Channel7_IRQHandler(void)		
{
    if(DMA_GetITStatus(DMA1_IT_TC7) != RESET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC7);    
        // DMA_Cmd(DMA1_Channel7, DISABLE);       
        // DMA1_Channel7->CNDTR=0;               
        // uart2SendFlag = 1;                    
    }
}


/*--------------------   send and get    -----------------*/
uint16_t getUartByte(uint8_t *data,void *handle)
{
    static uint8_t *pDmaDataOut = uart2RecvData;
    uint16_t ret = 0;
    if ((uart2RecvData + sizeof(uart2RecvData)) - DMA1_Channel6->CNDTR != pDmaDataOut)
    {
        *data = *pDmaDataOut;
        pDmaDataOut++;
        if (pDmaDataOut > &uart2RecvData[2047])
        {
            pDmaDataOut = uart2RecvData;
        }
        ret = 1;
    }
    return ret;
}

uint16_t sendUartBytes(uint8_t *data, uint16_t len, void *userHandle)
{
   while (DMA1_Channel7->CNDTR != 0)
    {
        // __nop();
    }
    memcpy(uart2SendData, data, len);
    DMA_Cmd(DMA1_Channel7, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel7, len); 
     // DMA_ClearITPendingBit( DMA1_IT_TC7);    
    DMA_Cmd(DMA1_Channel7, ENABLE);

    return true;
}
