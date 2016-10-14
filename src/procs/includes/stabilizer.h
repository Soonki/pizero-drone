/*
 * stabilizer.h
 *
 *  Created on: Oct 13, 2016
 *      Author: anand
 */

#ifndef PROCS_INCLUDES_STABILIZER_H_
#define PROCS_INCLUDES_STABILIZER_H_

#include <stdint.h>

void stabilizer_init(void);
int stabilizer_test(void);
void setOrientation	(uint8_t r, uint8_t p, uint8_t y);
void setThrust		(uint8_t thr);

#endif /* PROCS_INCLUDES_STABILIZER_H_ */
