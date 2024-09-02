#include "IIC.h"
#include "stdbool.h"
/**
 * The iicStart function is used to initiate an I2C start condition.
 *
 * @param dev The parameter "dev" is of type "IIC_SIM_DRV_T", which is a pointer to a structure or
 * object of type "IIC_SIM_DRV_T".
 *
 * @return a boolean value.
 */


bool iicStart(IIC_SIM_DRV_T *dev)
{
	dev->setSDA(true);
	dev->setSCL(true);
	dev->delay();
	if (!dev->readSDA())
	{
		return false;
	}
	dev->setSDA(false);
	dev->delay();
	if (dev->readSDA())
	{
		return false;
	}
	dev->setSCL(false);
	dev->delay();
	return true;
}

/**
 * The iicStop function stops the IIC communication by setting the SCL and SDA lines to specific
 * values.
 *
 * @param dev The parameter "dev" is a pointer to an object of type "IIC_SIM_DRV_T".
 */
void iicStop(IIC_SIM_DRV_T *dev)
{
	dev->setSCL(false);
	dev->delay();
	dev->setSDA(false);
	dev->delay();
	dev->setSCL(true);
	dev->delay();
	dev->setSDA(true);
	dev->delay();
}

/**
 * The iicAck function sets the SCL and SDA pins to specific values and delays for a certain period of
 * time.
 *
 * @param dev The parameter "dev" is of type "IIC_SIM_DRV_T", which is likely a structure or class
 * representing a driver for an I2C (Inter-Integrated Circuit) communication interface.
 */
void iicAck(IIC_SIM_DRV_T *dev)
{
	dev->setSCL(false);
	dev->delay();
	dev->setSDA(false);
	dev->delay();
	dev->setSCL(true);
	dev->delay();
	dev->setSCL(false);
	dev->delay();
}

/**
 * The function iicNoAck sends a no-acknowledge signal during I2C communication.
 *
 * @param dev The parameter "dev" is of type "IIC_SIM_DRV_T", which is likely a structure or class
 * representing a driver for an I2C (Inter-Integrated Circuit) communication interface.
 */
void iicNoAck(IIC_SIM_DRV_T *dev)
{
	dev->setSCL(false);
	dev->delay();
	dev->setSDA(true);
	dev->delay();
	dev->setSCL(true);
	dev->delay();
	dev->setSCL(false);
	dev->delay();
}

/**
 * The iicWaitAck function waits for an acknowledgment signal from an IIC device.
 *
 * @param dev The parameter "dev" is a pointer to an object of type "IIC_SIM_DRV_T".
 *
 * @return a boolean value.
 */
bool iicWaitAck(IIC_SIM_DRV_T *dev)
{
	bool ret = true;
	dev->setSCL(false);
	dev->delay();
	dev->setSDA(true);
	dev->delay();
	dev->setSCL(true);
	dev->delay();
	if (dev->readSDA())
	{
		ret = false;
	}
	dev->setSCL(false);
	return ret;
}

/**
 * The function iicSendByte sends a byte of data over an I2C bus using the provided driver.
 *
 * @param dev The parameter "dev" is a pointer to an object of type "IIC_SIM_DRV_T".
 * @param byte The "byte" parameter is a variable of type uint8_t, which represents an 8-bit unsigned
 * integer. It is used to store the byte value that needs to be sent over the I2C bus.
 */
void iicSendByte(IIC_SIM_DRV_T *dev, uint8_t byte)
{
	uint8_t count = 8;
	while (count--)
	{
		dev->setSCL(false);
		dev->delay();
		if (byte & 0x80)
		{
			dev->setSDA(true);
		}
		else
		{
			dev->setSDA(false);
		}
		byte <<= 1;
		dev->delay();
		dev->setSCL(true);
		dev->delay();
	}
	dev->setSCL(false);
}

/**
 * The function iicReadByte reads a byte of data using the I2C protocol.
 *
 * @param dev The parameter "dev" is a pointer to an object of type "IIC_SIM_DRV_T". This object is
 * used to control the I2C communication.
 *
 * @return a uint8_t value, which is an 8-bit unsigned integer.
 */
uint8_t iicReadByte(IIC_SIM_DRV_T *dev)
{
	uint8_t count = 8, ret = 0;
	dev->setSDA(true);
	while (count--)
	{
		ret <<= 1;
		dev->setSCL(false);
		dev->delay();
		dev->setSCL(true);
		dev->delay();
		if (dev->readSDA())
		{
			ret |= 0X01;
		}
	}
	dev->setSCL(false);
	return ret;
}

