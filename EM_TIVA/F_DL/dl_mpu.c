/*
 * dl_mpu.c
 *
 *  Created on: 25.06.2018
 *      Author: Michael
 */
#include "dl_mpu.h"
#include "hal_mpu_9250.h"
#include "dl_LCD.h"


void dlShowAccData()
{
    float acc_x = 0;
    float acc_y = 0;
    float acc_z = 0;
    float acc_data[3] = {0};

    halGetAccData(&acc_x, &acc_y, &acc_z);


           acc_data[0] = acc_x*1000;
           acc_data[1] = acc_y*1000;
           acc_data[2] = acc_z*1000;


           if(acc_data[0] >= 0)
           {
               dlLcdWriteText("acc_x: ", 7, 0, 0);
               dlLcdWriteUInt((uint16_t)acc_data[0], 0, 46);
           }
           else
           {
               dlLcdWriteText("acc_x:-", 7, 0, 0);
               dlLcdWriteUInt((uint16_t)(acc_data[0]*-1), 0, 46);
           }

           if(acc_data[1] >= 0)
           {
               dlLcdWriteText("acc_y: ", 7, 1, 0);
               dlLcdWriteUInt((uint16_t)acc_data[1], 1, 46);
           }
           else
           {
               dlLcdWriteText("acc_y:-", 7, 1, 0);
               dlLcdWriteUInt((uint16_t)(acc_data[1]*-1), 1, 46);
           }

           if(acc_data[2] >= 0)
           {
               dlLcdWriteText("acc_z: ", 7, 2, 0);
               dlLcdWriteUInt((uint16_t)acc_data[2], 2, 46);
           }
           else
           {
               dlLcdWriteText("acc_z:-", 7, 2, 0);
               dlLcdWriteUInt((uint16_t)(acc_data[2]*-1), 2, 46);
           }
}


void dlShowGyrData()
{
   float gyr_x = 0;
   float gyr_y = 0;
   float gyr_z = 0;
   float gyr_data[3] = {0};

   halGetGyrData(&gyr_x, &gyr_y, &gyr_z);


   gyr_data[0] = gyr_x*1000;
   gyr_data[1] = gyr_y*1000;
   gyr_data[2] = gyr_z*1000;


          if(gyr_data[0] >= 0)
          {
              dlLcdWriteText("gyr_x: ", 7, 3, 0);
              dlLcdWriteUInt((uint16_t)gyr_data[0], 3, 46);
          }
          else
          {
              dlLcdWriteText("gyr_x:-", 7, 3, 0);
              dlLcdWriteUInt((uint16_t)(gyr_data[0]*-1), 3, 46);
          }

          if(gyr_data[1] >= 0)
          {
              dlLcdWriteText("gyr_y: ", 7, 4, 0);
              dlLcdWriteUInt((uint16_t)gyr_data[1], 4, 46);
          }
          else
          {
              dlLcdWriteText("gyr_y:-", 7, 4, 0);
              dlLcdWriteUInt((uint16_t)(gyr_data[1]*-1), 4, 46);
          }

          if(gyr_data[2] >= 0)
          {
              dlLcdWriteText("gyr_z: ", 7, 5, 0);
              dlLcdWriteUInt((uint16_t)gyr_data[2], 5, 46);
          }
          else
          {
              dlLcdWriteText("gyr_z:-", 7, 5, 0);
              dlLcdWriteUInt((uint16_t)(gyr_data[2]*-1), 5, 46);
          }
}

void dlShowMagData()
{
   float mag_x = 0;
   float mag_y = 0;
   float mag_z = 0;
   float mag_data[3] = {0};

   halGetMagData(&mag_x, &mag_y, &mag_z);

/*
   mag_data[0] = mag_x*1000;
   mag_data[1] = mag_y*1000;
   mag_data[2] = mag_z*1000;


          if(mag_data[0] >= 0)
          {
              dlLcdWriteText("mag_x: ", 7, 3, 0);
              dlLcdWriteUInt((uint16_t)mag_data[0], 3, 46);
          }
          else
          {
              dlLcdWriteText("mag_x:-", 7, 3, 0);
              dlLcdWriteUInt((uint16_t)(mag_data[0]*-1), 3, 46);
          }

          if(mag_data[1] >= 0)
          {
              dlLcdWriteText("mag_y: ", 7, 4, 0);
              dlLcdWriteUInt((uint16_t)mag_data[1], 4, 46);
          }
          else
          {
              dlLcdWriteText("mag_y:-", 7, 4, 0);
              dlLcdWriteUInt((uint16_t)(mag_data[1]*-1), 4, 46);
          }

          if(mag_data[2] >= 0)
          {
              dlLcdWriteText("mag_z: ", 7, 5, 0);
              dlLcdWriteUInt((uint16_t)mag_data[2], 5, 46);
          }
          else
          {
              dlLcdWriteText("mag_z:-", 7, 5, 0);
              dlLcdWriteUInt((uint16_t)(mag_data[2]*-1), 5, 46);
          }
          */
}

