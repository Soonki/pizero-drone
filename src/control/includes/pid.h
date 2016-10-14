/*
 * pid.h
 *
 *  Created on: Oct 13, 2016
 *      Author: anand
 */

#ifndef CONTROL_INCLUDES_PID_H_
#define CONTROL_INCLUDES_PID_H_

#include <stdint.h>

#define	DEFAULT_UPDATE_RATE	10	// ms	(100Hz)

typedef struct {
	float setpoint;
	float lastTime;
	float init;

	float dt;
	float kp;
	float kd;
	float ki;

	float outputLimitHigh;
	float outputLimitLow;

	float integ;
	float lastInput;

	float output;
	float error;
} pid_controller_t;

void initPID	(pid_controller_t* pid, const float pid_input, const float setpoint,
				const float kp, const float ki, const float kd,
				const float outMin, const float outMax);

void tunePID		(pid_controller_t* pid, const float kp, const float ki, const float kd);
void setSampleTime 	(pid_controller_t* pid, const float dt);
void setOutputLimit (pid_controller_t* pid, float outMax, float outMin);
void resetTarget	(pid_controller_t* pid, float target, float current_input);
float updatePID		(pid_controller_t* pid, float input);




#endif /* CONTROL_INCLUDES_PID_H_ */
