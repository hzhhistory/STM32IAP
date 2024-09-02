#ifndef __SERIAL_H__
#define __SERIAL_H__

/* Include ------------------------------*/
#include"stm32f10x_rcc.h"
#include"stm32f10x_usart.h"
#include"stm32f10x_gpio.h"
#include"stm32f10x_dma.h"
#include"stm32f10x.h "
#include"stdio.h"
#include"stm32f10x.h"
#include<string.h>
#include "stdarg.h"
#include"misc.h"
#include"stdbool.h"
/* Define--------------------------------*/
#define USART2TX_PIN GPIO_Pin_2
#define USART2RX_PIN GPIO_Pin_3
#define USART_MAX_LEN 1024

/* Function -----------------------------*/
void Usart_Init();
uint16_t getUartByte(uint8_t *data,void *handle);
uint16_t sendUartBytes(uint8_t *data, uint16_t len, void *userHandle);

#endif