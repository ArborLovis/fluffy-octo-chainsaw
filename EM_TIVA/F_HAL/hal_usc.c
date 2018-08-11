/*
 * hal_usc.c
 *
 *  Created on: 22.03.2018
 *      Author: Patrick
 */


#include "hal_gpio.h"
#include "hal_usc.h"
#include "tiva_headers.h"

void halSysClkInit()
{
    // use main osc with ext. quarz of 16 MHz
    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_1);
    //SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_SYSDIV_5 | SYSCTL_XTAL_16MHZ);
}

uint32_t getClkValue()
{
    return SysCtlClockGet();
}
