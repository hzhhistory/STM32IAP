#include"port_dev.h"
void I2c_Init(void)
{
     GPIO_InitTypeDef gpioBsaeConfig;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpioBsaeConfig.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gpioBsaeConfig.GPIO_Mode = GPIO_Mode_Out_OD;
    gpioBsaeConfig.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpioBsaeConfig);
	
	gpioBsaeConfig.GPIO_Pin = GPIO_Pin_6;
    gpioBsaeConfig.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioBsaeConfig.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpioBsaeConfig);
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
GPIO_SetBits(GPIOA, GPIO_Pin_15);
    gpioBsaeConfig.GPIO_Pin = GPIO_Pin_15;
    gpioBsaeConfig.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioBsaeConfig.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpioBsaeConfig);

}

void MyI2C_W_SDA(bool BitValue)
{
    GPIO_WriteBit(PORT, SDA_PIN,(BitAction) BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
  										
}

void MyI2C_W_SCL(bool BitValue)
{
    GPIO_WriteBit(PORT, SCL_PIN,(BitAction) BitValue);		//根据BitValue，设置SCL引脚的电平
    							
}
bool MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(PORT, SDA_PIN);		//读取SDA电平
    // 延时10us，防止时序频率超过要求
    return BitValue;											//返回SDA电平
}
