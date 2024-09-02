#include"port_dev.h"

/*前向使用----------------------------------------------------------*/

void I2c_Init(void)
{
    GPIO_InitTypeDef gpioBsaeConfig;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

   gpioBsaeConfig.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
   gpioBsaeConfig.GPIO_Mode = GPIO_Mode_Out_OD;
   gpioBsaeConfig.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &gpioBsaeConfig);
	
	gpioBsaeConfig.GPIO_Pin = GPIO_Pin_13;
   gpioBsaeConfig.GPIO_Mode = GPIO_Mode_Out_PP;
   gpioBsaeConfig.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &gpioBsaeConfig);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

   gpioBsaeConfig.GPIO_Pin = GPIO_Pin_15;
   gpioBsaeConfig.GPIO_Mode = GPIO_Mode_Out_PP;
   gpioBsaeConfig.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &gpioBsaeConfig);
      GPIO_SetBits(GPIOA, GPIO_Pin_15);

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

/*后向使用----------------------------------------------------------*/

void I2cre_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}                 

void I2C_W_SDA(bool BitValue)
{
    GPIO_WriteBit(PORT, GPIO_Pin_15,(BitAction) BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
  										
}

void I2C_W_SCL(bool BitValue)
{
    GPIO_WriteBit(PORT, GPIO_Pin_14,(BitAction) BitValue);		//根据BitValue，设置SCL引脚的电平
    							
}
bool I2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(PORT, GPIO_Pin_15);		//读取SDA电平
    // 延时10us，防止时序频率超过要求
    return BitValue;											//返回SDA电平

}

