
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
#include "hal_rtc.h"

extern COM_Status uart_status_;
extern uint8_t switch_backlight_;
extern uint8_t is_switch_active;
extern int pwm_burst_cnt_;
extern unsigned short burst_finished_;

uint8_t data_sensor_[6] =  {0};

void main()
{


    hal_init();
    halMpuInit();

    dlLcdInit();
    dlLcdClear();

    dlSetChannelRF(CHANNEL_ADR);
    dlSetCarAddress(CAR_NODE_ADR);

    LCD_BACKLIGHT_ON;

    // Init Magnetometer
    SysCtlDelay(1000);   //simulate wait state for answer of the us module
    //dlShowMagData();
    halWhoAmI_Mag();

    SysCtlDelay(1000);   //simulate wait state for answer of the us module
    halStatus1_Mag();

    SysCtlDelay(5000);   //simulate wait state for answer of the us module
    halStatus2_Mag();

    SysCtlDelay(1000);   //simulate wait state for answer of the us module
    halCtlRead_Mag();

    while(1)
   {
        //SysCtlDelay(10000);   //simulate wait state for answer of the us module
        dlShowAccData();

        //SysCtlDelay(1000);   //simulate wait state for answer of the us module
        //dlShowGyrData();

        SysCtlDelay(10000);   //simulate wait state for answer of the us module
        uint8_t status = halStatus1_Mag();
        if(status == 0x01)
        {
            dlSendSensorData(PC_NODE_ADR);
            //dlShowMagData();
            SysCtlDelay(100000);   //simulate wait state for answer of the us module
        }
        else if(status > 0x01)
        {
            halStatus2_Mag();
            SysCtlDelay(50000);
        }

        //SysCtlDelay(1000);   //simulate wait state for answer of the us module
        //halStatus2_Mag();
        //SysCtlDelay(33333);
        //halShowTime();
        //SysCtlDelay(33333);



   }
}
