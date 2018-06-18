/*
 * hal_mpu_9250.c
 *
 *  Created on: 18.06.2018
 *      Author: Michael
 */
#include "hal_mpu_9250.h"
#include "hal_i2c.h"

void halMpuReset()
{
    uint8_t reg = 0x6B;
    uint8_t data[0];

    data[0] = 0x80;

    I2cSendBurst(ACCELGYRO_ADDR, reg, 1, data);
}

uint8_t halWhoAmI()
{
    return I2CReceiveSingle(ACCELGYRO_ADDR, 0x75);
}


