/*
 * i2cdev.h
 *
 *  Created on: Oct 4, 2016
 *      Author: anand
 */

#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#include "config.h"

#define I2CDEV		"/dev/i2c-1"

#include <stdint.h>

typedef struct {
	uint8_t addr;
	int fd;
	uint16_t timeout;
} i2cdev;

int init_dev(i2cdev* dev, uint8_t addr);
void setDevTimeout (i2cdev* dev, uint16_t timeout);

/* Read Device */
int8_t readWords(i2cdev* dev, uint8_t regAddr, uint8_t length, uint16_t *data);
int8_t readWord	(i2cdev* dev, uint8_t regAddr, uint16_t *data);

int8_t readBytes(i2cdev* dev, uint8_t regAddr, uint8_t length, uint8_t *data);
int8_t readByte	(i2cdev* dev, uint8_t regAddr, uint8_t *data);
int8_t readBytesNoRegAddr(i2cdev* dev, uint8_t length, uint8_t *data);

int8_t readBits	(i2cdev* dev, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
int8_t readBitsW(i2cdev* dev, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data);

int8_t readBit	(i2cdev* dev, uint8_t regAddr, uint8_t bitNum, uint8_t 	*data);
int8_t readBitW	(i2cdev* dev, uint8_t regAddr, uint8_t bitNum, uint16_t *data);

/* Write to Device */
int writeWords	(i2cdev* dev, uint8_t regAddr, uint8_t length, uint16_t *data);
int writeWord	(i2cdev* dev, uint8_t regAddr, uint16_t data);

int writeBytes	(i2cdev* dev, uint8_t regAddr, uint8_t length, uint8_t *data);
int writeByte	(i2cdev* dev, uint8_t regAddr, uint8_t data);

int writeBits	(i2cdev* dev, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
int writeBitsW	(i2cdev* dev, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);

int writeBit	(i2cdev* dev, uint8_t regAddr, uint8_t bitNum, uint8_t data);
int writeBitW	(i2cdev* dev, uint8_t regAddr, uint8_t bitNum, uint16_t data);

#endif /* DRIVERS_INCLUDES_I2CDEV_H_ */
