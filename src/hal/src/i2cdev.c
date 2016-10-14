/*
 * i2cdev.c
 *
 *  Created on: Oct 4, 2016
 *      Author: anand
 */

#include "i2cdev.h"

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "string.h"

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/i2c-dev.h>


int init_dev(i2cdev* dev, uint8_t addr) {
	dev->addr = addr;
	dev->timeout = 0;

	return 0;
}

void setDevTimeout (i2cdev* dev, uint16_t timeout)
{
	dev->timeout = timeout;
}

int8_t readWords(i2cdev* dev, uint8_t regAddr, uint8_t length, uint16_t *data) {
	int8_t count = 0;
	count = readBytes(dev, regAddr, length, (uint8_t*) data);
	return count / 2;
}

int8_t readWord(i2cdev* dev, uint8_t regAddr, uint16_t *data) {
	return readWords(dev, regAddr, 1, data);
}

int8_t readBytes(i2cdev* dev, uint8_t regAddr, uint8_t length, uint8_t *data) {
	int fd = open(I2CDEV, O_RDWR);
	int count = 0;
	if (fd < 0) {
		fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
		return -1;
	}
	if (ioctl(fd, I2C_SLAVE, dev->addr) < 0) {
		fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
		close(fd);
		return (-1);
	}
	if (write(fd, &regAddr, 1) != 1) {
		fprintf(stderr, "Failed to write reg: %s\n", strerror(errno));
	}

	count = read(fd, data, length);
	if (count < 0) {
		fprintf(stderr, "Failed to read device(%d): %s\n", count,
				strerror(errno));
		close(fd);
		return (-1);
	} else if (count != length) {
		fprintf(stderr, "Short read from device, expected %d, got %d\n", length,
				count);
		close(fd);
		return (-1);
	}
	close(fd);
	return count;
}

int8_t readByte(i2cdev* dev, uint8_t regAddr, uint8_t *data) {
	return readBytes(dev, regAddr, 1, data);
}

int8_t readBytesNoRegAddr(i2cdev* dev, uint8_t length, uint8_t *data) {
	int8_t count = 0;
	int fd = open(I2CDEV, O_RDWR);

	if (fd < 0) {
		fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
		return (-1);
	}
	if (ioctl(fd, I2C_SLAVE, dev->addr) < 0) {
		fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
		close(fd);
		return (-1);
	}
	count = read(fd, data, length);
	if (count < 0) {
		fprintf(stderr, "Failed to read device(%d): %s\n", count,
				strerror(errno));
		close(fd);
		return (-1);
	} else if (count != length) {
		fprintf(stderr, "Short read  from device, expected %d, got %d\n",
				length, count);
		close(fd);
		return (-1);
	}
	close(fd);
	return count;
}

int8_t readBits(i2cdev* dev, uint8_t regAddr, uint8_t bitStart, uint8_t length,
		uint8_t *data) {
	/*
	 * 01101001		read bits
	 * 76543210		bit positions
	 *    xxx  		if bitStart=4, length=3
	 *    010		after masking
	 *    >>010		shift
	 */

	uint8_t count, b;
	count = readByte(dev, regAddr, &b);
	if (count != 0) {
		uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
		b &= mask;
		*data = b;
	}
	return count;
}

int8_t readBitsW(i2cdev* dev, uint8_t regAddr, uint8_t bitStart, uint8_t length,
		uint16_t* data) {
	uint8_t count;
	uint16_t w;
	count = readWord(dev, regAddr, &w);
	if (count != 0) {
		uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
		w &= mask;
		*data = w;
	}
	return count;
}

int8_t readBit(i2cdev* dev, uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
	uint8_t b;
	uint8_t count = readByte(dev, regAddr, &b);
	*data = b & (1 << bitNum);
	return count;
}

int8_t readBitW(i2cdev* dev, uint8_t regAddr, uint8_t bitNum, uint16_t *data) {
	uint16_t b;
	uint8_t count = readWord(dev, regAddr, &b);
	*data = b & (1 << bitNum);
	return count;
}

int writeBit(i2cdev* dev, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
	uint8_t b;
	readByte(dev, regAddr, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	return writeByte(dev, regAddr, b);
}

int writeBitW(i2cdev* dev, uint8_t regAddr, uint8_t bitNum, uint16_t data) {
	uint16_t w;
	readWord(dev, regAddr, &w);
	w = (data != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
	return writeWord(dev, regAddr, w);
}

int writeBits(i2cdev* dev, uint8_t regAddr, uint8_t bitStart, uint8_t length,
		uint8_t data) {
	/* 010 value to write
	 * 76543210 bit numbers
	 *    xxx   args: bitStart=4, length=3
	 * 00011100 mask byte
	 * 10101111 original value (sample)
	 * 10100011 original & ~mask
	 * 10101011 masked | value
	 */
	uint8_t b;
	if (readByte(dev, regAddr, &b) != 0) {
		uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
		data <<= (bitStart - length + 1);
		data &= mask;
		b &= ~(mask);
		b |= data;
		return writeByte(dev, regAddr, b);
	} else {
		return -1;
	}
}

int writeBitsW(i2cdev* dev, uint8_t regAddr, uint8_t bitStart, uint8_t length,
		uint16_t data) {
	uint16_t w;
	if (readWord(dev, regAddr, &w) != 0) {
		uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
		data <<= (bitStart - length + 1); // shift data into correct position
		data &= mask; // zero all non-important bits in data
		w &= ~(mask); // zero all important bits in existing word
		w |= data; // combine data with existing word
		return writeWord(dev, regAddr, w);
	} else {
		return -1;
	}
}

int writeBytes	(i2cdev* dev, uint8_t regAddr, uint8_t length, uint8_t *data)
{
	int8_t count = 0;
	uint8_t buf[128];
	int fd;
	if (length > 127) {
		fprintf(stderr, "Byte write count (%d) > 127\n", length);
		return -1;
	}
	fd = open(I2CDEV, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
		return -1;
	}
	buf[0] = regAddr;
	memcpy(buf+1, data, length);
	count = write(fd, buf, length+1);
	if (count < 0) {
		fprintf (stderr, "Failed to write device(%d): %s\n", count, strerror(errno));
		close(fd);
		return -1;
	} else if (count != length+1) {
		fprintf(stderr, "Short write to device, expected %d, got %d\n", length+1, count);
		close(fd);
		return -1;
	}
	close(fd);
	return count;
}

int writeByte	(i2cdev* dev, uint8_t regAddr, uint8_t data)
{
	return writeBytes(dev, regAddr, 1, &data);
}

int writeWords	(i2cdev* dev, uint8_t regAddr, uint8_t length, uint16_t *data)
{
	int8_t count = 0;
	uint8_t buf[128];
	int i, fd;

	// Should do potential byteswap and call writeBytes() really, but that
	    // messes with the callers buffer

	    if (length > 63) {
	        fprintf(stderr, "Word write count (%d) > 63\n", length);
	        return -1;
	    }

	    fd = open(I2CDEV, O_RDWR);
	    if (fd < 0) {
	        fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
	        return -1;
	    }
	    if (ioctl(fd, I2C_SLAVE, dev->addr) < 0) {
	        fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
	        close(fd);
	        return -1;
	    }
	    buf[0] = regAddr;
	    for (i = 0; i < length; i++) {
	        buf[i*2+1] = data[i] >> 8;
	        buf[i*2+2] = data[i];
	    }
	    count = write(fd, buf, length*2+1);
	    if (count < 0) {
	        fprintf(stderr, "Failed to write device(%d): %s\n", count, strerror(errno));
	        close(fd);
	        return -1;
	    } else if (count != length*2+1) {
	        fprintf(stderr, "Short write to device, expected %d, got %d\n", length+1, count);
	        close(fd);
	        return -1;
	    }
	    close(fd);
	    return count;
}

int writeWord	(i2cdev* dev, uint8_t regAddr, uint16_t data)
{
	return writeWords(dev,regAddr, 1, &data);
}

