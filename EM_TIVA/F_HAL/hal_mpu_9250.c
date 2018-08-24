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
    // Delay for Reset-
    SysCtlDelay(1000000);   //simulate wait state for answer of the us module
    halAccInit();
    SysCtlDelay(1000000);   //simulate wait state for answer of the us module
    halGyrInit();
    SysCtlDelay(1000000);   //simulate wait state for answer of the us module
    halMagInit();

}

void halMpuReset()
{
    uint8_t reg = 0x6B;
    uint8_t data[1];

    data[0] = 0x80;

    I2cSendBurst(ACCELGYRO_ADDR, reg, 1, data);
}

uint8_t halWhoAmI()
{
    return I2CReceiveSingle(ACCELGYRO_ADDR, 0x75);
}

void halAccInit()
{
    //uint8_t reg[3] = {0x1D, 0x1C, 0x37};
    //uint8_t data[3] = {0x06, 0x08, 0x02};

    uint8_t reg[2] = {0x1D, 0x1C};
    uint8_t data[2] = {0x06, 0x08};
    int i = 0;

    for(i = 0; i < sizeof(reg); i++)
    {
        I2cSendBurst(ACCELGYRO_ADDR, reg[i], 1, data+i);
    }

}


void halGyrInit()
{
    uint8_t reg[2] = {0x1A, 0x1B};
    uint8_t data[2] = {0x06, 0x10};

    int i=0;

   for(i = 0; i < sizeof(reg); i++)
   {
       I2cSendBurst(ACCELGYRO_ADDR, reg[i], 1, data+i);
   }
}

void halMagInit()
{
    uint8_t data = 0x02;

    I2cSendBurst(ACCELGYRO_ADDR, 0x37, 1, &data);
    SysCtlDelay(10000);   //simulate wait state for answer of the us module

    data = 0x00;
    I2cSendBurst(MAG_ADDR, 0x0A, 1, &data);

}

uint32_t halWhoAmI_Gyr()
{
    return I2CReceiveSingle(MAG_ADDR, 0x00);
}


void halGetAccData(float* accel_x, float* accel_y, float* accel_z)
{
    float resolution = 8.0/65536.0;

    // read MPU Acc value x
    int16_t temp_x = I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[0]);
    int16_t accel16_x = (temp_x << 8) | I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[1]);

    // read MPU Acc value y
    int16_t temp_y = I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[2]);
    int16_t accel16_y = (temp_y << 8) | I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[3]);

    // read MPU Acc value z
    int16_t temp_z = I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[4]);
    int16_t accel16_z = (temp_z << 8) | I2CReceiveSingle(ACCELGYRO_ADDR, Accel_reg_addr[5]);

    float x_value = (float)accel16_x * resolution;
    float y_value = (float)accel16_y * resolution;
    float z_value = (float)accel16_z * resolution;

    *accel_x = x_value;
    *accel_y = y_value;
    *accel_z = z_value;


}

void halGetGyrData(float* gyr_x, float* gyr_y, float* gyr_z)
{
    float resolution = (2*1000)/65536.0;

   // read MPU Gyr value x
   int16_t temp_x = I2CReceiveSingle(ACCELGYRO_ADDR, gyro_reg_addr[0]);
   int16_t gyr_16_x = (temp_x << 8) | I2CReceiveSingle(ACCELGYRO_ADDR, gyro_reg_addr[1]);

   // read MPU Acc value y
   int16_t temp_y = I2CReceiveSingle(ACCELGYRO_ADDR, gyro_reg_addr[2]);
   int16_t gyr_16_y = (temp_y << 8) | I2CReceiveSingle(ACCELGYRO_ADDR, gyro_reg_addr[3]);

   // read MPU Acc value z
   int16_t temp_z = I2CReceiveSingle(ACCELGYRO_ADDR, gyro_reg_addr[4]);
   int16_t gyr_16_z = (temp_z << 8) | I2CReceiveSingle(ACCELGYRO_ADDR, gyro_reg_addr[5]);

   *gyr_x = gyr_16_x*resolution;
   *gyr_y = gyr_16_y*resolution;
   *gyr_z = gyr_16_z*resolution;
}


























