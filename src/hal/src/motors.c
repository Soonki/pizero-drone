/*
 * motors.c
 *
 *  Created on: Oct 12, 2016
 *      Author: anand
 */

#include "motors.h"
#include "pca9685.h"
#include "gpio.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#define PCA_ENABLE_PIN		RPI_GPIO_27

static pca9685_dev pwm_dev;
volatile float pwm_ms[4];
static int armed_p = FALSE;		// Predicate for if armed or not
static int init_p = FALSE;

int initMotors() {
	if (init_p) {
		return 0;
	}
	init_pca9685(&pwm_dev, PCA9685_DEFAULT_ADDRESS);
	enable_pca9685(&pwm_dev);
	init_p = TRUE;
	return 0;
}

int armMotors() {
	if (armed_p)
		return 0;
	pinMode(PCA_ENABLE_PIN, OUTPUT);
	digitalWrite(PCA_ENABLE_PIN, 0);

	if (test_pca9685(&pwm_dev) < 0) {
		fprintf(stderr, "Failed to initialize motors: %s\n", strerror(errno));
		return -1;
	}

	setPWMFrequency(&pwm_dev, MOTOR_FREQUENCY);

	setAllPWMmS(&pwm_dev, MOTOR_MIN_MS);
	armed_p = TRUE;
	return 0;
}

int disarmMotors() {
	if (!armed_p)
		return 0;
	setAllPWMmS(&pwm_dev, 0);

	restart_pwm(&pwm_dev);

	pinMode(PCA_ENABLE_PIN, OUTPUT);
	digitalWrite(PCA_ENABLE_PIN, 0);
	armed_p = FALSE;
	return 0;
}

int resetMotors() {
	if (armed_p)
		return -1;
	setAllPWMmS(&pwm_dev, 0);

	restart_pwm(&pwm_dev);

	pinMode(PCA_ENABLE_PIN, OUTPUT);
	digitalWrite(PCA_ENABLE_PIN, 0);
	return 0;
}

/**
 *	Channel of PCA to update is:
 *		MotorNum + Offset	=	MotorNum + 2
 *
 *
 */
void updateMotorPWM(motor_t motor, uint16_t offset, uint16_t length) {
	if (!armed_p) {
		fprintf(stderr, "Attempt to run disarmed motor: %s\n", strerror(errno));
		return;
	}

	setPWM(&pwm_dev, ((int) motor) + 2, offset, length);

}

void updateMotorPWMms(motor_t motor, float length_ms) {
	if (!armed_p) {
//		fprintf(stderr, "Attempt to run disarmed motor: %s\n", strerror(errno));
		return;
	}

	setPWMmS(&pwm_dev, ((int) motor) + 2, length_ms);
}

void updateMotorPWMus(motor_t motor, float length_us) {
	if (!armed_p) {
		fprintf(stderr, "Attempt to run disarmed motor: %s\n", strerror(errno));
		return;
	}

	setPWMuS(&pwm_dev, ((int) motor) + 2, length_us);
}

void updateAllMotors_ms(float length_ms) {
	if (!armed_p) {
		fprintf(stderr, "Attempt to run disarmed motor: %s\n", strerror(errno));
		return;
	}

	setAllPWMmS(&pwm_dev, length_ms);
}

int	is_armed()
{
	return armed_p;
}
