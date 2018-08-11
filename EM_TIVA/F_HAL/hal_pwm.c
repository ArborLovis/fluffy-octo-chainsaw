/*
 * hal_pwm.c
 *
 *  Created on: 22.03.2018
 *      Author: Patrick
 */

#include "hal_pwm.h"
#include "hal_gpio.h"
#include "tiva_headers.h"


void halPWMInit()
{
    //enable pwm module and wait until module works satisfied
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));

    //map Pin A6 & A7 to the PWM module
    //fct has to be called separatly, so makro can be divided
    GPIOPinConfigure(GPIO_PA6_M1PWM2);
    GPIOPinConfigure(GPIO_PA7_M1PWM3);
    GPIOPinTypePWM(GPIO_PORTA_BASE, STEERING | THROTTLE);

    SysCtlPWMClockSet(SYSCTL_PWMDIV_16);    //divide 16MHz through 16 --> 1MHz

    PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, 16666);   //16,67ms (60Hz)
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, 1999);   //2ms
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, 1999);   //2ms

    PWMGenEnable(PWM1_BASE, PWM_GEN_1);                             //enable PWM Generator
    PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT | PWM_OUT_3_BIT, true); //enable selected output states

}
