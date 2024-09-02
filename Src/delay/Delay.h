#ifndef _DELAY_H
#define _DELAY_H
#include "sys.h"
#include"stm32f10x.h"

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);
void i2c_Delay(void);
#endif

