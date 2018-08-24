/*
 * hal_mpu_9250.h
 *
 *  Created on: 18.06.2018
 *      Author: Michael
 */

#ifndef F_HAL_HAL_MPU_9250_H_
#define F_HAL_HAL_MPU_9250_H_

#include "../F_TIVA/tiva_headers.h"

#define ACCELGYRO_ADDR 0x68
#define MAG_ADDR 0x0C

void halMpuInit();
void halMpuReset();
uint8_t halWhoAmI();
uint32_t halWhoAmI_Gyr();

void halAccInit();
void halGyrInit();
void halMagInit();

void halGetAccData(float* accel_x, float* accel_y, float* accel_z);
void halGetGyrData(float* gyr_x, float* gyr_y, float* gyr_z);

#endif /* F_HAL_HAL_MPU_9250_H_ */
