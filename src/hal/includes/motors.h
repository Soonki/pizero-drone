/*
 * motors.h
 *
 *  Created on: Oct 12, 2016
 *      Author: anand
 */

#ifndef HAL_INCLUDES_MOTORS_H_
#define HAL_INCLUDES_MOTORS_H_

#include <stdint.h>

typedef enum {
	MOTOR_1 = 0, MOTOR_2 = 1, MOTOR_3 = 2, MOTOR_4 = 3
} motor_t;

#define MOTOR_FREQUENCY	50	// Hz
#define	MOTOR_MIN_MS	1.0	// milli secs
#define MOTOR_MAX_MS	2.0	// milli secs

int initMotors();
int armMotors();
int disarmMotors();
int resetMotors();
void updateMotorPWM(motor_t motor, uint16_t offset, uint16_t length);
void updateMotorPWMms(motor_t motor, float length_ms);
void updateMotorPWMus(motor_t motor, float length_us);
void updateAllMotors_ms(float length_ms);
int	is_armed();

#endif /* HAL_INCLUDES_MOTORS_H_ */
