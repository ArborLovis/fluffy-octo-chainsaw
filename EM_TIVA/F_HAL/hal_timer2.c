/*
 * hal_timer2.c
 *
 *  Created on: 06.06.2018
 *      Author: Patrick
 */

#include "tiva_headers.h"
#include "hal_timer2.h"
#include "hal_gpio.h"
#include "dl_general.h"

void extIntBHandler();
void timer2ISR();

extern volatile Sensor sensor_data_;

void halTimer2Init()
{
    sensor_data_.Sonic.timer_cnt_us1 = 0;
    sensor_data_.Sonic.timer_cnt_us2 = 0;
    sensor_data_.Sonic.us1_data_ready = false;
    sensor_data_.Sonic.us2_data_ready = false;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2)); //wait for PORT to be ready
    TimerClockSourceSet(TIMER2_BASE, TIMER_CLOCK_SYSTEM);
    TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC);
    TimerLoadSet(TIMER2_BASE, TIMER_A, TIMER_CNT_LEN);  //precision is 1us
    TimerLoadSet(TIMER2_BASE, TIMER_B, TIMER_CNT_LEN);
    //TimerEnable(TIMER2_BASE, TIMER_BOTH);     //start later, after burst has finished

    //configuration for the interrupt handling
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);
    TimerIntRegister(TIMER2_BASE, TIMER_BOTH, timer2ISR);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);

    //activate external Interrupt
    GPIOIntClear(GPIO_PORTB_BASE, 0xFF);    //clear all interrupt flags
    GPIOIntRegister(GPIO_PORTB_BASE, extIntBHandler);
    GPIOIntTypeSet(GPIO_PORTB_BASE, US1_SIGNAL_OUT, GPIO_RISING_EDGE);  //interrupt is triggerd  by rising edge
    GPIOIntTypeSet(GPIO_PORTB_BASE, US2_SIGNAL_OUT, GPIO_RISING_EDGE);
    GPIOPadConfigSet(GPIO_PORTB_BASE, US1_SIGNAL_OUT, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTB_BASE, US2_SIGNAL_OUT, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntEnable(GPIO_PORTB_BASE, US2_SIGNAL_OUT); //enable ext. Int. of US1
    GPIOIntEnable(GPIO_PORTB_BASE, US1_SIGNAL_OUT); //enable ext. Int. of US2

}

void extIntBHandler()
{
    static uint8_t is_switch_active = 0;

    if(US1_SIGNAL_OUT & GPIOIntStatus(GPIO_PORTB_BASE, US1_SIGNAL_OUT))   //returns masked interrupt status;
    {
        GPIOIntClear(GPIO_PORTB_BASE, US1_SIGNAL_OUT);      //clear flag US1 - answer
        halStopTimer2_A();
        GPIOIntDisable(GPIO_PORTB_BASE, US1_SIGNAL_OUT);
        sensor_data_.Sonic.us1_data_ready = true;
        SysCtlDelay(100); //Debounce --> just for fast debugging
    }
    if(US2_SIGNAL_OUT & GPIOIntStatus(GPIO_PORTB_BASE, US2_SIGNAL_OUT))   //returns masked interrupt status;
    {
        GPIOIntClear(GPIO_PORTB_BASE, US2_SIGNAL_OUT);      //clear flag of US2 - answer
        halStopTimer2_B();
        GPIOIntDisable(GPIO_PORTB_BASE, US2_SIGNAL_OUT);
        sensor_data_.Sonic.us2_data_ready = true;
        SysCtlDelay(100); //Debounce --> just for fast debugging
    }
}

void timer2ISR()
{
    uint32_t reg_value_A = TimerLoadGet(TIMER2_BASE, TIMER_A);
    uint32_t reg_value_B = TimerLoadGet(TIMER2_BASE, TIMER_B);

    if(TIMER_TIMA_TIMEOUT & TimerIntStatus(TIMER2_BASE, true))
    {
        TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
        if(++sensor_data_.Sonic.timer_cnt_us1 == 1000)      //wait until cross-talk has finished, time adjustable (1ms)
        {
            GPIOIntEnable(GPIO_PORTB_BASE, US1_SIGNAL_OUT);     //wait for answer us1
        }
    }
    if(TIMER_TIMB_TIMEOUT & TimerIntStatus(TIMER2_BASE, true))
    {
        TimerIntClear(TIMER2_BASE, TIMER_TIMB_TIMEOUT);
        if(++sensor_data_.Sonic.timer_cnt_us2 == 1000)      //wait until cross-talk has finished, time adjustable (1ms)
        {
            GPIOIntEnable(GPIO_PORTB_BASE, US2_SIGNAL_OUT);     //wait for answer us2
        }
    }
}

void halStartTimer2_A()
{
    TimerEnable(TIMER2_BASE, TIMER_A);
}

void halStopTimer2_A()
{
    TimerDisable(TIMER2_BASE, TIMER_A);
}

void halStartTimer2_B()
{
    TimerEnable(TIMER2_BASE, TIMER_B);
}
void halStopTimer2_B()
{
    TimerDisable(TIMER2_BASE, TIMER_B);
}
void halStartTimer2_Both()
{
    TimerEnable(TIMER2_BASE, TIMER_BOTH);
}
void halStopTimer2_Both()
{
    TimerDisable(TIMER2_BASE, TIMER_BOTH);
}

