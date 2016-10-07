/*
 * pca9685.c
 *
 *  Created on: Oct 4, 2016
 *      Author: anand
 */

#include "pca9685.h"

int init_pca9685(pca9685_dev* dev, uint8_t addr)
{
	return init_dev(&(dev->dev), addr);
}

int enable_pca9685	(pca9685_dev* dev)
{
	int status;
	dev->frequency = getPWMFrequency(dev);
	status = writeBit(&(dev->dev), PCA9685_RA_MODE1, PCA9685_MODE1_AI_BIT, 1);
	status &= restart_pwm(dev);

	return status;
}

int test_pca9685	(pca9685_dev* dev)
{
	uint8_t data;
	int8_t status = readByte(&(dev->dev), PCA9685_RA_PRE_SCALE, &data);
	if (status > 0) return 0;
	else			return -1;
}


void sleep_pwm		(pca9685_dev* dev)
{
	writeBit(&(dev->dev), PCA9685_RA_MODE1, PCA9685_MODE1_SLEEP_BIT, 1);
}

int	restart_pwm		(pca9685_dev* dev)
{
	writeByte(&(dev->dev), PCA9685_RA_MODE1, (1 << PCA9685_MODE1_SLEEP_BIT));
	writeByte(&(dev->dev), PCA9685_RA_MODE1, ((1 << PCA9685_MODE1_SLEEP_BIT)
	    									| (1 << PCA9685_MODE1_EXTCLK_BIT)));
	writeByte(&(dev->dev), PCA9685_RA_MODE1, ((1 << PCA9685_MODE1_RESTART_BIT)
                                            | (1 << PCA9685_MODE1_EXTCLK_BIT)
                                            | (1 << PCA9685_MODE1_AI_BIT)));
	//TODO add error check
	return 0;
}

float getPWMFrequency	(pca9685_dev* dev)
{
	uint8_t data;
	readByte(&(dev->dev), PCA9685_RA_PRE_SCALE, &data);
	return 6000.f / (data + 1);							// 24576000.f/4096.f / (data + 1)
}

void setPWMFrequency	(pca9685_dev* dev, float frequency)
{
	sleep_pwm(dev);
	usleep(10000);
	uint8_t prescale = roundf(6000.f/frequency) - 1;
	writeByte(&(dev->dev), PCA9685_RA_PRE_SCALE, prescale);
	dev->frequency = getPWMFrequency(dev);
	dev->scale_factor = 4096.f * dev->frequency;
	restart_pwm(dev);
}

void setPWM		(pca9685_dev* dev, uint8_t channel, uint16_t offset, uint16_t length)
{
	uint8_t data[4] = {0,0,0,0};
	if(length == 0)				data[3] = 0x10;
	else if (length >= 4096)	data[1] = 0x10;
	else {
		data[0] = offset & 0xFF;
		data[1] = offset >> 8;
		data[2] = length & 0xFF;
		data[3] = length >> 8;
	}
	writeBytes(&(dev->dev), PCA9685_RA_LED0_ON_L + 4 * channel, 4, data);
}

void setPWMmS	(pca9685_dev* dev, uint8_t channel, float length_mS)
{
	setPWM (dev, channel,0, ceil(length_mS * (dev->scale_factor) / 1000.f));
}

void setPWMuS	(pca9685_dev* dev, uint8_t channel, float length_uS)
{
	setPWM(dev, channel,0, ceil(length_uS * (dev->scale_factor) / 1000000.f));
}

void setAllPWM	(pca9685_dev* dev, uint16_t offset, uint16_t length)
{
	uint8_t data[4] = {offset & 0xFF, offset >> 8, length & 0xFF, length >> 8};
	writeBytes(&(dev->dev), PCA9685_RA_ALL_LED_OFF_L, 4, data);
}

void setAllPWMmS(pca9685_dev* dev, float length_mS)
{
	setAllPWM(dev, 0, ceil(length_mS * (dev->scale_factor) / 1000.f));
}

void setAllPWMuS(pca9685_dev* dev, float length_uS)
{
	setAllPWM(dev, 0, ceil(length_uS * (dev->scale_factor) / 1000000.f));
}

