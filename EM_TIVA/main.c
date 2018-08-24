
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
#include "dl_mpu.h"

extern COM_Status uart_status_;
extern uint8_t switch_backlight_;
extern uint8_t is_switch_active;
extern int pwm_burst_cnt_;
extern unsigned short burst_finished_;

void main()
{


    hal_init();
    halMpuInit();
    dlLcdInit();
    dlLcdClear();

    LCD_BACKLIGHT_ON;
    uint8_t test;

    //halWhoAmI();
    while(1)
   {
        test = 0;
        SysCtlDelay(1000000);   //simulate wait state for answer of the us module
        dlShowAccData();
        dlShowGyrData();
        SysCtlDelay(1000);   //simulate wait state for answer of the us module
        test = halWhoAmI_Gyr();
        SysCtlDelay(1000);   //simulate wait state for answer of the us module

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
