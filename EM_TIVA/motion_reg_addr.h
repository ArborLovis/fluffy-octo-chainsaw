/*
 * motion_reg_addr.h
 *
 *  Created on: 1 Apr 2017
 *      Author: Mayer
 */

#ifndef DRIVER_MOTION_REG_ADDR_H_
#define DRIVER_MOTION_REG_ADDR_H_

#define ACCELGYRO_ADDR      0x68
#define MAG_ADDR            0x0C

char Accel_reg_addr[] =
{
        0x3B,           // ACCEL XOUT H
        0x3C,           // ACCEL XOUT L
        0x3D,           // ACCEL YOUT H
        0x3E,           // ACCEL YOUT L
        0x3F,           // ACCEL ZOUT H
        0x40           // ACCEL ZOUT L
};
char gyro_reg_addr[] =
{
         0x43,           // GYRO XOUT H
         0x44,           // GYRO XOUT L
         0x45,           // GYRO YOUT H
         0x46,           // GYRO YOUT L
         0x47,           // GYRO ZOUT H
         0x48           // GYRO ZOUT L
};

char magn_reg_addr[] =
{
         0x03,           // MAGN XOUT L
         0x04,           // MAGN XOUT H
         0x05,           // MAGN YOUT L
         0x06,           // MAGN YOUT H
         0x07,           // MAGN ZOUT L
         0x08            // MAGN ZOUT H
};

char MPU_rst [] =
{
     0x6B, // POWER Management // Reset
     0x00,
     /*0x6B,
     0x01*/
};

char Acc_init_reg[] =
{
        0x1D, // Register 29 set lowpass @5Hz
        0x06,
        0x1C, // Register 28 set accelerometer to @±4G
        0x08,
        0x37, // INT_PIN_CFG
        0x02
        //0x38, // INT_ENABLE
        //0x01
};

char gyro_init_reg[] =
{
        0x1A, //Register 26 set lowpass @5Hz
        0x06,
        0x1B, //Register 27 set 1000 dps
        0x10
};

char magn_pwrDown[] =
{             // RESET MAGNETO METER
       // 0x0A, // CNTL2 Register
       //  0x00  // RESET
};

char magneto_fuse_rom[] =
{
        0x0A,
        0x0F
};

char magneto_requMeas[] =
{
        0x0A,
        0x16
};

char acc_test[] =
{
     0x75, // Who I AM
     0x1C
};

char magno_test[] =
{
     0x00 // Who I AM
};

char magno_getASA[] =
{
     0x10, // Who I AM
     0x11,
     0x12
};

#endif /* DRIVER_MOTION_REG_ADDR_H_ */
