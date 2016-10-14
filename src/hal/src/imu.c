/*
 * imu.c
 *
 *  Created on: Oct 12, 2016
 *      Author: anand
 */

#include "mpu9250.h"
#include "config.h"
#include "imu.h"

int  initIMU	()
{
	return init_mpu9250(BITS_DLPF_CFG_188HZ);
}

int	 testIMU	()
{
	int status = test_mpu9250();
	if (status == FALSE) return -1;
	return 0;
}

void getMotion6(float *ax, float *ay, float *az, float *gx, float *gy, float *gz)
{
	getMotion6_mpu(ax,ay,az,gx,gy,gz);
}

void getMotion9(float *ax, float *ay, float *az, float *gx, float *gy, float *gz, float *mx, float *my, float *mz)
{
	getMotion9_mpu(ax,ay,az,gx,gy,gz,mx,my,mz);
}

void getMotion6_raw(float *ax, float *ay, float *az, float *gx, float *gy, float *gz)
{
	getMotion6_mpu_raw(ax,ay,az,gx,gy,gz);
}

void getMotion9_raw(float *ax, float *ay, float *az, float *gx, float *gy, float *gz, float *mx, float *my, float *mz)
{
	getMotion9_mpu_raw(ax,ay,az,gx,gy,gz,mx,my,mz);
}
