/*
 * spidev.c
 *
 *  Created on: Oct 11, 2016
 *      Author: anand
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <linux/spi/spidev.h>
#include "spidev.h"

const static char *spiDev0 = "/dev/spidev0.0";
const static char *spiDev1 = "/dev/spidev0.1";
const static uint8_t spiBPW = 8;
const static uint16_t spiDelay = 0;

static uint32_t spiSpeeds[2];
static int spiFds[2];

int setupSPI(int channel, uint32_t speed, int mode) {
	int fd;
	mode &= 3;
	channel &= 1;

	if ((fd = open(channel == 0 ? spiDev0 : spiDev1, O_RDWR)) < 0) {
		fprintf(stderr, "Could not open SPI device at channel %d:%s\n",
				channel, strerror(errno));
		return -1;
	}

	spiSpeeds [channel] = speed ;
	spiFds    [channel] = fd ;
	return fd;
}

int spiDataRW(int channel, uint8_t* tx, uint8_t* rx, int len)
{
	struct spi_ioc_transfer spi;
	memset(&spi, 0, sizeof(spi));

	spi.tx_buf			=	(unsigned long)tx;
	spi.rx_buf			=	(unsigned long)rx;
	spi.len				=	len;
	spi.delay_usecs		=	spiDelay;
	spi.speed_hz		=	spiSpeeds[channel];
	spi.bits_per_word	=	spiBPW;

	return ioctl(spiFds[channel], SPI_IOC_MESSAGE(1), &spi);

}
