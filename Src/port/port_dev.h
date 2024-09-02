#ifndef __PORT_DEV_H__
#define __PORT_DEV_H__
#include"stdbool.h"
#include"delay.h"
#include"stm32f10x.h"
#include"stm32f10x_gpio.h"
#include"stm32f10x_rcc.h"

#define PORT GPIOB
#define SDA_PIN  GPIO_Pin_13
#define SCL_PIN  GPIO_Pin_12
#define Power_Pin GPIO_Pin_15

/*-------------------------Register address----------------------------*/
#define IIC_SLAVE_ADDRW 0xC6
#define IIC_SLAVE_ADDRR 0xC7
#define IVFM_Register 0x02
#define Enable_Register 0x01
#define LED1_Brightness 0x03
#define LED2_Brightness 0x04
#define Torch1_Brightness 0x05
#define Torch2_Brightness 0x06
#define Boost_Configuration 0x07
#define Timing_Configuration 0x08
#define Device_ID 0x0C

/*-------------------------Brightness level----------------------------*/
typedef enum
{
    Infrared_1=0,
    Infrared_2,
    Infrared_3,
    Infrared_4,
    Infrared_5,
    Infrared_6,
    Infrared_7,
    Infrared_8
}LED_LM;

void I2c_Init(void);
void MyI2C_W_SDA(bool BitValue);
void MyI2C_W_SCL(bool BitValue);
bool MyI2C_R_SDA(void);

void I2cre_Init(void);
void I2C_W_SDA(bool BitValue);
void I2C_W_SCL(bool BitValue);
bool I2C_R_SDA(void);

#endif


