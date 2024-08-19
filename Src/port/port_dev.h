#ifndef __PORT_DEV_H__
#define __PORT_DEV_H__
#include"stdbool.h"
#include"delay.h"
#include"stm32f10x.h"
#include"stm32f10x_gpio.h"
#include"stm32f10x_rcc.h"

#define PORT GPIOB
#define SDA_PIN  GPIO_Pin_7
#define SCL_PIN  GPIO_Pin_6
#define Power_Pin GPIO_Pin_15
void I2c_Init(void);
void MyI2C_W_SDA(bool BitValue);
void MyI2C_W_SCL(bool BitValue);
bool MyI2C_R_SDA(void);

#endif


