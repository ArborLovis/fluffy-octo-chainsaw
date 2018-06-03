/*
 * dl_general.h
 *
 *  Created on: 22.03.2018
 *      Author: Patrick
 */

#ifndef F_DL_DL_GENERAL_H_
#define F_DL_DL_GENERAL_H_

#include "../tiva_headers.h"

#define STEERING_MIDDLE         1499
#define STEERING_MAX_RIGHT      1799
#define STEERING_MAX_LEFT       1199

#define MIN_RPW_ESC      5000
#define MAX_RPW_ESC      2500
#define MIN_FPW_ESC      7500
#define MAX_FPW_ESC      10000
#define MAX_BRAKE_ESC    6250

typedef enum speed_mode {FORWARD, BACKWARD, BRAKE} SpeedMode;

/*  Name:           dlSetSteering
 *  Parameter:      int16_t ctr_val
 *  Description:    The steering (servo) can be controlled.
 *                  By setting the ctr_val between -100 and 100
 *                  the angle of the servo can be cotrolled.        */
void dlSetSteering(int16_t ctr_val);

/*  Name:           dlSetThrottle
 *  Parameter:      SpeedMode dir, int16_t speed
 *  Description:    The ESC (dc-motor) of the car can be controlled.
 *                  By setting a value for "dir" the direction can be selected.
 *                  With "speed", the speed of the car can be controlled by setting
 *                  it from 0 to 100.                         */
void dlSetThrottle(SpeedMode dir, int16_t speed);

#endif /* F_DL_DL_GENERAL_H_ */
