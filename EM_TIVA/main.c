
/**
 * main.c
 */

// test comment - changes in .gitignore

//#define SET_TIME

#include "../tiva_headers.h"
#include "hal_init.h"
#include "hal_gpio.h"
#include "hal_uart1.h"
#include "hal_ssi1.h"
#include "hal_us.h"
#include "hal_i2c.h"

#include "dl_general.h"
#include "dl_RF_Module.h"
#include "dl_LCD.h"
#include "hal_mpu_9250.h"

extern COM_Status uart_status_;
extern uint8_t switch_backlight_;
extern uint8_t is_switch_active;
extern int pwm_burst_cnt_;
extern unsigned short burst_finished_;

void main()
{
    float acc_x = 0;
    float acc_y = 0;
    float acc_z = 0;
    float acc_data[3] = 0;

    hal_init();
    halMpuInit();
    dlLcdInit();
    dlLcdClear();

    //halWhoAmI();
    while(1)
   {
        SysCtlDelay(1000000);   //simulate wait state for answer of the us module
       //I2C Acceleromenter Reset
        halGetAccData(&acc_x, &acc_y, &acc_z);


        acc_data[0] = acc_x*1000;
        acc_data[1] = acc_y*1000;
        acc_data[2] = acc_z*1000;




        LCD_BACKLIGHT_ON;
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
}

/*

    uint8_t data_send[63] = 0;
    uint32_t data_get[63] = 0;

#ifdef SET_TIME

    //Set Time to Clock in BCD-Code
    data_send[0] = 0x00;     //sec
    data_send[1] = 0x24;     //min
    data_send[2] = 0x00;     //hr

    data_send[3] = 0x00;     //Day of week
    //Set Date to Clock
    data_send[4] = 0x15;     //Date
    data_send[5] = 0x06;     //Month
    data_send[6] = 0x18;     //Year

    I2cSendBurst(TINY_RTC_ADR, SLAVE_REG_TIME, 7, data_send);

#endif


    int i =0;
    for(i=0; i<56; i++)
    {
        data_get[i] = 0;
        data_send[i] = i;
    }

    I2cSendBurst(TINY_RTC_ADR, SLAVE_REG_STORE, 56, data_send);


    while(1)
    {
        //I2cSendByte(TINY_RTC_ADR, data_send);
        //SysCtlDelay(1000);   //simulate wait state for answer of the us module

        I2CReceiveBurst(TINY_RTC_ADR, SLAVE_REG_TIME, 7, data_get);
        //I2CReceiveBurst(TINY_RTC_ADR, SLAVE_REG_STORE, 56, data_get);

        //I2C Acceleromenter Reset
        halMpuReset();

        SysCtlDelay(1000000);   //simulate wait state for answer of the us module

    }
}
*/
