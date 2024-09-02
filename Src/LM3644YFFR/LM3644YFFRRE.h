#ifndef __LM3644YFFRRE_H__
#define __LM3644YFFRRE_H__

#include"IIC.h"

/*-------------------------Power Enable--------------------------------*/
// #define PowerRE_Enable GPIO_SetBits(GPIOB,GPIO_Pin_3)

/*---------------------Function Definitions--------------------------*/
void LM3644YFFRRE_INIT(void);                                             // Initialization of the LM3644YFFR
void LM3644YFFRRE_WriteReg(uint8_t PointAddr,uint8_t Data);               // Registers written to the LM3644YFFR
uint8_t LM3644YFFRRE_ReadReg(void);                                           // Reads the LM3644YFFR
void LM3644YFFRRE_LEDFlash(uint8_t Luminance);                            // Reads the LM3644YFFR





#endif