
/**
 * main.c
 */
// Test Github auf Herz und Nieren ;-)

// here stays a comment

#include "../tiva_headers.h"
#include "hal_init.h"
#include "hal_gpio.h"
#include "hal_uart1.h"
#include "hal_ssi1.h"
#include "hal_us.h"

#include "dl_general.h"
#include "dl_RF_Module.h"
#include "dl_LCD.h"

extern COM_Status uart_status_;
extern uint8_t switch_backlight_;
extern uint8_t is_switch_active;

void main()
{
    int16_t pwm_ctr = 0;
    uint8_t dir = 0;
    uint8_t text_node[19] = {"Car Address: "};
    uint8_t text_channel[10] = {"Channel: "};
    uint8_t lock = 0, lock2 = 0;
    uint8_t lock_old = 0, lock_old2 = 0;


    uint8_t uart_load[128] = {};
    uint8_t data_len = 0;

    hal_init();
    dlLcdInit();

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, 1999);
    PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, false); //enable selected output states

    //dlSetChannelRF(CHANNEL_ADR);
    //dlSetCarAddress(CAR_NODE_ADR);

    while(1)
    {
        //****************** PWM US Test - Begin ******************
        if(halIsBurstFinished())
        {
            halStartBurstModeUS();
            SysCtlDelay(100000);   //simulate wait state for answer of the us module
        }

        //****************** PWM US Test - End   ******************
        if(is_switch_active)
        {
            lock = 1;

            if(lock != lock_old)
            {
                if(CAR_NODE_ADR == dlGetSettingInfoRF(CAR_ADDRESS))
                {
                    dlLcdWriteText(text_node, 19, 0, 0);
                    dlLcdWriteUInt(CAR_NODE_ADR, 0, 80);
                }
                lock_old = lock;
            }
        }
        else
        {
            lock = 0;

            if(lock != lock_old)
            {
                if(CHANNEL_ADR == dlGetSettingInfoRF(CHANNEL))
                {
                    dlLcdWriteText(text_channel, 10, 1, 0);
                    dlLcdWriteUInt(CHANNEL_ADR, 1, 80);
                }
                lock_old = lock;
            }
        }

        //UART Test 2: Turning on LCD Backlight
        if(switch_backlight_)
        {
            if(uart_status_.Status.R)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, LCD_BACKLIGHT, LCD_BACKLIGHT);
                uart_status_.Status.R = 0;
            }
        }
        else
        {
            if(uart_status_.Status.R)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, LCD_BACKLIGHT, ~LCD_BACKLIGHT);
                uart_status_.Status.R = 0;
            }
        }

        if(dlGetDataPayloadRF(uart_load, &data_len))
        {
            lock2 = 1;
            if(lock2 != lock_old2)
            {
                //dlLcdClear();
                dlLcdWriteText(text_channel, 10, 4, 0);
                lock_old2 = lock2;
            }

        }
        else
        {
            lock2 = 0;
            if(lock2 != lock_old2)
            {
                //dlLcdClear();
                dlLcdWriteText(text_node, 19, 5, 0);
                lock_old2 = lock2;
            }
        }
        //dlSendDataRF(9, test, 5);

        // UART Test 1: Turning on LCD Backlight
        /*
        if(uart_status_.Status.R)
        {
            uart_status_.Status.R = 0;
            if(uart_status_.RxData.Data[4] == 0x05)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, LCD_BACKLIGHT, LCD_BACKLIGHT);
            }
            else if(uart_status_.RxData.Data[4] == 0x06)
            {
                GPIOPinWrite(GPIO_PORTA_BASE, LCD_BACKLIGHT, ~LCD_BACKLIGHT);
            }
        }
        */

        //SSIDataPut(SSI1_BASE, 0xFA);
        //while(SSIBusy(SSI1_BASE));

        /*

        if(!cnt)
            dlGetChannelRF(CHANNEL);
        else if(cnt == 1)
            dlGetChannelRF(CAR_ADDRESS);
        else if(cnt == 2)
            dlGetChannelRF(FIRMWARE);
        else {
            dlSendDataRF(0x09, test, 5);
        }

        SysCtlDelay(5000000);
        SysCtlDelay(5000000);
        SysCtlDelay(5000000);

        if(++cnt == 4)
            cnt = 0;
*/

        if(!dir)
        {
            dlSetSteering(pwm_ctr);
            pwm_ctr += 10;
            if(pwm_ctr >= 110)
            {
                pwm_ctr = 100;
                dir = 1;
            }
        }
        else
        {
            dlSetSteering(pwm_ctr);
            pwm_ctr -= 10;
            if(pwm_ctr <= -110)
            {
                pwm_ctr = -100;
                dir = 0;
            }
        }
        //SysCtlDelay(5000000);
    }


}

//Here is an GIT-Test ;)

/*
    while(1)
    {
        //Turn on green led
        GPIOPinWrite(GPIO_PORTF_BASE, RED_LED | GREEN_LED | BLUE_LED , GREEN_LED);

        //wait a time -> delay
        SysCtlDelay(2000000);

        //Turn on blue led
        GPIOPinWrite(GPIO_PORTF_BASE, RED_LED | GREEN_LED | BLUE_LED , BLUE_LED);

        //wait a time -> delay
        SysCtlDelay(2000000);

        //Turn on red led
        GPIOPinWrite(GPIO_PORTF_BASE, RED_LED | GREEN_LED | BLUE_LED , RED_LED);

        //wait a time -> delay
        SysCtlDelay(2000000);

    }
 */
