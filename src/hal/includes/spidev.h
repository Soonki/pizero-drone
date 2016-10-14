/*
 * spidev.h
 *
 *  Created on: Oct 11, 2016
 *      Author: anand
 */

#ifndef HAL_SPIDEV_H_
#define HAL_SPIDEV_H_

#include <stdint.h>

int setupSPI	(int channel, uint32_t speed, int mode);
int spiDataRW	(int fd, uint8_t* tx, uint8_t* rx, int len);

#endif /* HAL_SPIDEV_H_ */
