/*
 * hal_mpu_9250.c
 *
 *  Created on: 18.06.2018
 *      Author: Michael
 */
#include "../F_TIVA/tiva_headers.h"
#include "hal_mpu_9250.h"
#include "hal_i2c.h"
#include "motion_reg_addr.h"

float asa_x_ = 0;
float asa_y_ = 0;
float asa_z_ = 0;

void halMpuInit()
{
    halMpuReset();
    // Delay for Reset-
    SysCtlDelay(1000);   //simulate wait state for answer of the us module
    halAccInit();
    SysCtlDelay(1000);   //simulate wait state for answer of the us module
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
    halCtlWrite_Mag(6);
    SysCtlDelay(10000);

    halGetMagASA(&asa_x_, &asa_y_, &asa_z_);

    SysCtlDelay(10000);
    halCtlWrite_Mag(2);
    SysCtlDelay(10000);
}

uint8_t halWhoAmI_Mag()
{
    return I2CReceiveSingle(MAG_ADDR, 0x00);
}

uint8_t halStatus1_Mag()
{
    return I2CReceiveSingle(MAG_ADDR, 0x02);
}

uint8_t halStatus2_Mag()
{
    return I2CReceiveSingle(MAG_ADDR, 0x09);
}

void halCtlWrite_Mag(int mode)
{
    uint8_t data = 0x00;
    switch(mode)
    {
        case 0:         //"0000": Power Down mode
        {
            data = 0x00;
            break;
        }
        case 1:         //"0001": Single measurement mode
        {
            data = 0x11;
            break;
        }
        case 2:         //"0010": Continuous measurement mode 1
        {
            data = 0x12;
            break;
        }
        case 3:         //"0110": Continuous measurement mode 2
        {
            data = 0x16;
            break;
        }
        case 4:         //"0100": External trigger measurement mode
        {
            data = 0x14;
            break;
        }
        case 5:         //"1000": Self-test mode
        {
            data = 0x18;
            break;
        }
        case 6:         //"1111": Fuse ROM access mode
        {
            data = 0x0F;
            break;
        }
        default:
        {
            data = 0x00;
            break;
        }
    }
    I2cSendBurst(MAG_ADDR, 0x0A, 1, &data);
}

uint8_t halCtlRead_Mag()
{
    return I2CReceiveSingle(MAG_ADDR, 0x0A);
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

void halGetMagData(float* mag_x, float* mag_y, float* mag_z)
{
   float resolution = 4912.0/32760.0;

   // read MPU Gyr value x
   int16_t temp_x = I2CReceiveSingle(MAG_ADDR, magn_reg_addr[0]);
   int16_t mag_16_x = (temp_x << 8) | I2CReceiveSingle(MAG_ADDR, magn_reg_addr[1]);
   SysCtlDelay(200);

   // read MPU Acc value y
   int16_t temp_y = I2CReceiveSingle(MAG_ADDR, magn_reg_addr[2]);
   int16_t mag_16_y = (temp_y << 8) | I2CReceiveSingle(MAG_ADDR, magn_reg_addr[3]);
   SysCtlDelay(200);

   // read MPU Acc value z
   int16_t temp_z = I2CReceiveSingle(MAG_ADDR, magn_reg_addr[4]);
   int16_t mag_16_z = (temp_z << 8) | I2CReceiveSingle(MAG_ADDR, magn_reg_addr[5]);

   *mag_x = mag_16_x * resolution * asa_x_;
   *mag_y = mag_16_y * resolution * asa_y_;
   *mag_z = mag_16_z * resolution * asa_z_;
}

void halGetRawMagData(int8_t* h_mag_x, int8_t* l_mag_x, int8_t* h_mag_y, int8_t* l_mag_y, int8_t* h_mag_z, int8_t* l_mag_z)
{
    *h_mag_x = I2CReceiveSingle(MAG_ADDR, magn_reg_addr[0]);
    *l_mag_x = I2CReceiveSingle(MAG_ADDR, magn_reg_addr[1]);
    *h_mag_y = I2CReceiveSingle(MAG_ADDR, magn_reg_addr[2]);
    *l_mag_y = I2CReceiveSingle(MAG_ADDR, magn_reg_addr[3]);
    *h_mag_z = I2CReceiveSingle(MAG_ADDR, magn_reg_addr[4]);
    *l_mag_z = I2CReceiveSingle(MAG_ADDR, magn_reg_addr[5]);
}

void halGetMagASA(float* asa_x, float* asa_y, float* asa_z)
{
    int8_t asa_x_raw = I2CReceiveSingle(MAG_ADDR, 0x10);
    *asa_x = ((((float)asa_x_raw-128)/2)/128)+1;

    int8_t asa_y_raw = I2CReceiveSingle(MAG_ADDR, 0x11);
    *asa_y = ((((float)asa_y_raw-128)/2)/128)+1;

    int8_t asa_z_raw = I2CReceiveSingle(MAG_ADDR, 0x12);
    *asa_z = ((((float)asa_z_raw-128)/2)/128)+1;
}

























