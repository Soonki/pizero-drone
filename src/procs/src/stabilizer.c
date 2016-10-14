/*
 * stabilizer.c
 *
 *  Created on: Oct 13, 2016
 *      Author: anand
 */

#include "imu.h"
#include "motors.h"
#include "MadgwickAHRS.h"
#include "gpio.h"
#include "i2cdev.h"
#include "pid.h"

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "config.h"
#include <pthread.h>
#include <math.h>
#include "stabilizer.h"
#include "util.h"

/*Config for z-axis*/
#define Z_LOWPASS_ALPHA	0.02
#define Z_KP 			30
#define Z_KI 			0
#define Z_KD 			10

/*Config for x-axis*/
#define X_LOWPASS_ALPHA 0.02
#define X_KP 			250
#define X_KI 			500
#define X_KD 			2.5

/*Config for y-axis*/
#define Y_LOWPASS_ALPHA	0.02
#define Y_KP			250
#define Y_KI			500
#define Y_KD 			2.5

#define PITCH_MAX		0.2 // ms
#define ROLL_MAX		0.2	// ms


static int is_init = FALSE;
pthread_t stabilizer_task;

static float	roll	=	0.0f;
static float	pitch	=	0.0f;
static float	yaw		=	0.0f;
static float	thrust	=	0.0f;

pid_controller_t pid_x;
pid_controller_t pid_y;
pid_controller_t pid_z;

float ax, ay, az, gx, gy, gz, mx, my, mz;

void *stabilizer_loop(void* args);


void stabilizer_init(void)
{
	if(is_init) return;

	initIMU();
	initMotors();

	int ret = pthread_create(&stabilizer_task, NULL, stabilizer_loop, NULL);

	is_init = !ret;
	pthread_exit(NULL);
}

int stabilizer_test(void)
{
	int pass = 0;
	pass += testIMU();
	return pass;
}

void scaleParameters(float* r, float* p, float* y, float* t) {
	*r = ((roll*ROLL_MAX) < ROLL_MAX) ? ((roll*ROLL_MAX > 0-ROLL_MAX)? roll*ROLL_MAX : 0-ROLL_MAX): ROLL_MAX;
	*p = ((pitch*PITCH_MAX) < PITCH_MAX) ? ((pitch*PITCH_MAX > 0-PITCH_MAX)? pitch*PITCH_MAX : 0-PITCH_MAX): PITCH_MAX;
	*y = 0;
	*t = (MOTOR_MAX_MS - MOTOR_MIN_MS) * thrust/100;

}

void actuatorControl() {
	float r, p, y, t;
	scaleParameters(&r, &p, &y, &t);
	updateAllMotors_ms(t);
	updateMotorPWM(MOTOR_1, 0, t-(r/2)+(p/2));
	updateMotorPWM(MOTOR_2, 0, t+(r/2)-(p/2));
	updateMotorPWM(MOTOR_3, 0, t+(r/2)+(p/2));
	updateMotorPWM(MOTOR_4, 0, t-(r/2)-(p/2));
}

void *stabilizer_loop(void* args)
{
	waitForSys();
	armMotors();
	getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
	float vel_z = 0;
//	initPID(&pid_x, ax, 0, X_KP, X_KI, X_KD, -13.5, 13.5);
//	initPID(&pid_y, ay, 0, Y_KP, Y_KI, Y_KD, -13.5, 13.5);
//	initPID(&pid_z, vel_z, 0, Z_KP, Z_KI, Z_KD, -5, 5);

	unsigned int dt = millis();
	while(sys_running()) {
		if (dt - millis() >= DEFAULT_UPDATE_RATE) {
			actuatorControl();
		}
	}
	pthread_exit(NULL);
}

void setOrientation	(uint8_t r, uint8_t p, uint8_t y)
{
	roll = (100 >= r)? (r <= 0) ? r/100: 0: 1;
	pitch = p/100;
	yaw = 0;
}
void setThrust		(uint8_t thr)
{
	thrust = thr;
}

