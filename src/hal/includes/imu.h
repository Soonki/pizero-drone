/*
 * imu.h
 *
 *  Created on: Oct 12, 2016
 *      Author: anand
 */

#ifndef HAL_INCLUDES_IMU_H_
#define HAL_INCLUDES_IMU_H_

int  initIMU	();
int	 testIMU	();
void getMotion9	(float *ax, float *ay, float *az, float *gx, float *gy, float *gz, float *mx, float *my, float *mz);
void getMotion6	(float *ax, float *ay, float *az, float *gx, float *gy, float *gz);

void getMotion9_raw	(float *ax, float *ay, float *az, float *gx, float *gy, float *gz, float *mx, float *my, float *mz);
void getMotion6_raw	(float *ax, float *ay, float *az, float *gx, float *gy, float *gz);

#endif /* HAL_INCLUDES_IMU_H_ */
