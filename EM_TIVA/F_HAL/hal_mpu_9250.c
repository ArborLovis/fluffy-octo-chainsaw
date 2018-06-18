/*
 * hal_mpu_9250.c
 *
 *  Created on: 18.06.2018
 *      Author: Michael
 */
#include "hal_mpu_9250.h"
#include "hal_i2c.h"
#include "motion_reg_addr.h"

void halMpuInit()
{
    halMpuReset();
    halAccInit();

}

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

void halAccInit()
{
    uint8_t reg[3] = {0x1D, 0x1C, 0x37};
    uint8_t data[3] = {0x06, 0x08, 0x02};
    int i=0;

    for(i = 0; i < sizeof(reg); i++)
    {
        I2cSendBurst(ACCELGYRO_ADDR, reg[i], 1, data+i);
    }
}

void halGetAccData(float* accel_x, float* accel_y, float* accel_z)
{
    float resolution = 8.0/65536.0;

    // read MPU Acc value x
    uint16_t temp_x = I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[0]);
    uint16_t accel16_x = (temp_x << 8) | I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[1]);

    // read MPU Acc value y
    uint16_t temp_y = I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[2]);
    uint16_t accel16_y = (temp_y << 8) | I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[3]);

    // read MPU Acc value z
    uint16_t temp_z = I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[4]);
    uint16_t accel16_z = (temp_z << 8) | I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[5]);

    *accel_x = (float)accel16_x * resolution;
    *accel_y = (float)accel16_y * resolution;
    *accel_z = (float)accel16_z * resolution;


}



























