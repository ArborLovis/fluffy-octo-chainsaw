/*
 * hal_timer2.c
 *
 *  Created on: 06.06.2018
 *      Author: Patrick
 */

#include "tiva_headers.h"
#include "hal_timer2.h"
#include "hal_gpio.h"

void extIntBHandler();
void timer2ISR();

void halTimer2Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2)); //wait for PORT to be ready
    TimerClockSourceSet(TIMER2_BASE, TIMER_CLOCK_SYSTEM);
    TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC);
    TimerLoadSet(TIMER2_BASE, TIMER_A, 16);
    TimerLoadSet(TIMER2_BASE, TIMER_B, 16);
    TimerEnable(TIMER2_BASE, TIMER_BOTH);

    //configuration for the interrupt handling
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);
    TimerIntRegister(TIMER2_BASE, TIMER_BOTH, timer2ISR);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);


    //activate external Interrupt
    GPIOIntClear(GPIO_PORTB_BASE, 0xFF);    //clear all interrupt flags
    GPIOIntRegister(GPIO_PORTB_BASE, extIntBHandler);
    GPIOIntTypeSet(GPIO_PORTB_BASE, US1_SIGNAL_OUT, GPIO_RISING_EDGE);  //interrupt is triggerd  by rising edge
    GPIOIntTypeSet(GPIO_PORTB_BASE, US2_SIGNAL_OUT, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTB_BASE, US1_SIGNAL_OUT); //enable ext. Int. of Pin 4 -> Switch
    GPIOPadConfigSet(GPIO_PORTB_BASE, US2_SIGNAL_OUT, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntEnable(GPIO_PORTB_BASE, US1_SIGNAL_OUT); //enable ext. Int. of Pin 4 -> Switch
    GPIOPadConfigSet(GPIO_PORTB_BASE, US2_SIGNAL_OUT, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

}

void extIntBHandler()
{
    static uint8_t is_switch_active = 0;

    if(US1_SIGNAL_OUT & GPIOIntStatus(GPIO_PORTB_BASE, US1_SIGNAL_OUT))   //returns masked interrupt status; SW1 on Tiva Board (PF4)
    {
        GPIOIntClear(GPIO_PORTB_BASE, US1_SIGNAL_OUT);      //clear flag of Switch
        is_switch_active = !is_switch_active;
        SysCtlDelay(10000); //Debounce --> just for fast debugging
    }
    if(US2_SIGNAL_OUT & GPIOIntStatus(GPIO_PORTB_BASE, US2_SIGNAL_OUT))   //returns masked interrupt status; SW1 on Tiva Board (PF4)
    {
        GPIOIntClear(GPIO_PORTB_BASE, US2_SIGNAL_OUT);      //clear flag of Switch
        is_switch_active = !is_switch_active;
        SysCtlDelay(10000); //Debounce --> just for fast debugging
    }
}

void timer2ISR()
{
    uint32_t reg_value_A = TimerLoadGet(TIMER2_BASE, TIMER_A);
    uint32_t reg_value_B = TimerLoadGet(TIMER2_BASE, TIMER_B);
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);
}

