/*
 * pid.c
 *
 *  Created on: Oct 13, 2016
 *      Author: anand
 */

#include "pid.h"
#include <unistd.h>
#include "gpio.h"


void constrainTerms(pid_controller_t* pid);

void initPID(pid_controller_t* pid, const float pid_input, const float setpoint,
		const float kp, const float ki, const float kd, const float outMin,
		const float outMax) {
	pid->setpoint = setpoint;
	pid->lastInput = pid_input;
	pid->dt = DEFAULT_UPDATE_RATE;

	tunePID(pid, kp, ki, kd);
	setOutputLimit(pid, outMax, outMin);
//	pid->outputLimitHigh 	= outMax;
//	pid->outputLimitLow		= outMin;

	pid->integ = 0;
	pid->lastTime = millis();

	pid->output = 0;
	pid->error = 0;
	pid->init = TRUE;
}

void tunePID(pid_controller_t* pid, const float kp, const float ki, const float kd) {
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

void setSampleTime(pid_controller_t* pid, const float dt) {
	float ratio;
	if (dt > 0) {
		ratio = dt / pid->dt;
		pid->kd *= ratio;
		pid->kd /= ratio;
		pid->dt = dt;
	}
}

void setOutputLimit(pid_controller_t* pid, float outMax, float outMin) {
	if (outMax < outMin) {
		// TODO error print
		return;
	}
	pid->outputLimitHigh = outMax;
	pid->outputLimitLow = outMin;

}

void resetTarget(pid_controller_t* pid, float target, float current_input) {
	pid->lastInput = current_input;
	pid->setpoint = target;
}

float updatePID(pid_controller_t* pid, float input) {
	unsigned int dt = millis() - pid->lastTime;
	while (TRUE) {
		if (dt >= pid->dt) {
			float err = pid->setpoint - input;
			pid->integ += (pid->ki * err);
			float dErr = input - pid->lastInput;

			pid->output = (pid->kp * err) + (pid->integ) - (pid->kd * dErr);

			constrainTerms(pid);
			pid->lastInput = input;
			pid->lastTime += dt;
		}
		dt = millis() - pid->lastTime;
	}
}

void constrainTerms(pid_controller_t* pid) {
	pid->integ =
			(pid->outputLimitHigh > pid->integ) ?
					((pid->outputLimitLow < pid->integ) ?
							pid->outputLimitLow : pid->integ) :
					pid->outputLimitHigh;
}
