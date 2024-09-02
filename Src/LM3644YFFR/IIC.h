/*
 * @Description:	模拟IIC驱动文件
 * @Author: L LC @amov
 * @Date: 2023-08-15 11:28:02
 * @LastEditors: L LC @amov
 * @LastEditTime: 2023-08-23 11:51:18
 * @FilePath: \SPF210-power-gimbalControl\application\power_control\INA219\IIC.h
 */
#ifndef _I2C_H_
#define _I2C_H_

#include "stdbool.h"
#include "stdint.h"
#include"delay.h"

#define I2C_Direction_Trans 0
#define I2C_Direction_Rec 1

typedef struct
{
	void (*delay)(void);
	void (*setSDA)(bool state);
	void (*setSCL)(bool state);
	bool (*readSDA)(void);
} IIC_SIM_DRV_T;

bool iicStart(IIC_SIM_DRV_T *dev);
void iicStop(IIC_SIM_DRV_T *dev);
void iicAck(IIC_SIM_DRV_T *dev);
void iicNoAck(IIC_SIM_DRV_T *dev);
bool iicWaitAck(IIC_SIM_DRV_T *dev);
void iicSendByte(IIC_SIM_DRV_T *dev, uint8_t byte);
uint8_t iicReadByte(IIC_SIM_DRV_T *dev);

#endif
