/*
 * hal_us.c
 *
 *  Created on: 17.05.2018
 *      Author: Patrick
 */
#include "hal_us.h"
#include "hal_gpio.h"
#include "tiva_headers.h"
#include "dl_LCD.h"

//Note: PE4: Module 0; PWM 4; Generator 2
//      PE5: Module 0; PWM 5; Generator 2

//*************************************************************
//  ATTENTION: interrupt handler (function) has to be overwriten
//             in file: tm4c123gh6pm_startup_ccs.c  !!!!!!!!!!
//*************************************************************
void PWM0_Int_handler();
void halResetBurstIndicator();

unsigned short burst_finished_ = 0;
int pwm_burst_cnt_ = 0;


void halUsInit()
{
    //enable pwm module 0 and wait until module works satisfied
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));

    //map PE4 & PE5 to the PWM module 0
    //fct has to be called separately, so makro can be divided
    //
    GPIOPinConfigure(GPIO_PE4_M0PWM4);
    GPIOPinConfigure(GPIO_PE5_M0PWM5);
    GPIOPinTypePWM(GPIO_PORTE_BASE, US1_SIGNAL_IN | US2_SIGNAL_IN);

    SysCtlPWMClockSet(SYSCTL_PWMDIV_16);    //divide 16MHz through 16 --> 1MHz

    // Configure the PWM generator for count down mode with immediate updates
    // to parameters.
    //
    PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, CNT_PERIOD_US);     //25
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, CNT_WIDTH_US);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, CNT_WIDTH_US);

    //PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 16666);     //25
    //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, 1999);
    //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, 1999);

    //enable PWM0 Interrupt
    IntEnable(INT_PWM0_2);

    PWMIntEnable(PWM0_BASE, PWM_INT_GEN_2); // |PWM_INT_CNT_ZERO
    PWMGenIntTrigEnable(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_LOAD);    //enable interrupt on load and set trigger to load
    PWMGenIntClear(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_LOAD);         //clear interrupt flags, just to be sure
    PWMGenIntRegister(PWM0_BASE, PWM_GEN_2, PWM0_Int_handler);      //name of function is equal to the address

    PWMGenEnable(PWM0_BASE, PWM_GEN_2);                             //enable PWM Generator
    PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT | PWM_OUT_5_BIT, true); //enable selected output states
}

void halStartBurstModeUS()
{
    if(halIsBurstFinished())
    {
        halResetBurstIndicator();
        PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT | PWM_OUT_5_BIT, true); //enable selected output states
       // LCD_BACKLIGHT_OFF;
    }
}

unsigned short halIsBurstFinished()
{
    return burst_finished_;
}

void halResetBurstIndicator()
{
    burst_finished_ = 0;
}

//Note: To implement the burst mode a 40kHz (=25µs) clk of a period of 1ms is needed
//      The PWM Generator will be disabled after 40 puls-periodes of the 40kHz signal
//      counter = 1ms / 25µs = 40

void PWM0_Int_handler()
{
    static int x=8;
    if(PWM_INT_CNT_LOAD & PWMGenIntStatus(PWM0_BASE, PWM_GEN_2 , true))
        {
            PWMGenIntClear(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_LOAD);   //clear interrupt flag
        }

    if(pwm_burst_cnt_ > x && !burst_finished_)
    {
        burst_finished_ = 1;
        PWMGenDisable(PWM0_BASE, PWM_GEN_2);
        LCD_BACKLIGHT_OFF;
    }

    pwm_burst_cnt_ ++;

  /*  static unsigned short burst_cnt = BURST_CNT_LEN;

    if(PWM_INT_CNT_LOAD & PWMGenIntStatus(PWM0_BASE, PWM_GEN_2 , true))
    {
        PWMGenIntClear(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_LOAD);   //clear interrupt flag

        if(--burst_cnt == 0)
        {
            PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT | PWM_OUT_5_BIT, false); //disable selected output states
            burst_cnt = BURST_CNT_LEN;
            burst_finished_ = 1;
        }
        else
        {
            if(burst_cnt > BURST_CNT_LEN)
            {
                burst_cnt = BURST_CNT_LEN;  //safety check
            }
            burst_finished_ = 0;
        }
    }
    else
    {
        //other interrupt occurred -> will be ignored
    }*/
}
