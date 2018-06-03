/*
 * hal_ssi1.c
 *
 *  Created on: 10.04.2018
 *      Author: Patrick
 */

#include "hal_ssi1.h"
#include "hal_gpio.h"
#include "tiva_headers.h"
#include "dl_LCD.h"

extern COM_Status LCD_cmd_;

void halSsi1Int()
{
    //enable peripherals and wait until peripherals are ready
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI1));

    GPIOPinConfigure(GPIO_PF2_SSI1CLK); //enable SSI clk for the module
    GPIOPinConfigure(GPIO_PF1_SSI1TX);  //enable MOSI for the ssi module
    //GPIOPinConfigure(GPIO_PF3_SSI1FSS); //enable chip select
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, LCD_BACKLIGHT | LCD_A0 | LCD_RESET);     //conf Chip select as output
    GPIOPinWrite(GPIO_PORTA_BASE, LCD_BACKLIGHT | LCD_A0 | LCD_RESET, ~(LCD_BACKLIGHT | LCD_A0 | LCD_RESET));   //turn off LCD Backlight and reset A0 --> COMAND
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LCD_SPI_CS | LCD_SPI_MOSI | LCD_SPI_SCLK);
    GPIOPinWrite(GPIO_PORTF_BASE, LCD_SPI_CS, ~LCD_SPI_CS);

    GPIOPinTypeSSI(GPIO_PORTF_BASE, (LCD_SPI_SCLK | LCD_SPI_MOSI));

    //Moto fmt, polarity 1, phase 0 --> Mode 2
    //use clk from system, also SysCtlClockGet() possible
    //Master-mode, 100kHz, 8 Bit, phase 0, polarity 1
    //SysCtlClockGet() returns the clock speed of the system
    SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_2, SSI_MODE_MASTER, SSI_CLK_SPEED, SSI_DATA_LEN);
    SSIEnable(SSI1_BASE);   //enable SSI1 module
    LCD_cmd_.Status.B.TxSuc = 1;
}

void halUsciB1Transmit(unsigned short len)
{
    LCD_cmd_.Status.B.TxSuc = 0;        //SPI is loaded with data, not ready anymore

    for(LCD_cmd_.TxData.cnt = 0; LCD_cmd_.TxData.cnt < len; LCD_cmd_.TxData.cnt++)
    {
        SSIDataPut(SSI1_BASE, LCD_cmd_.TxData.Data[LCD_cmd_.TxData.cnt]);
        while(SSIBusy(SSI1_BASE));      //wait until byte has been send --> blocking function
    }

    LCD_cmd_.Status.B.TxSuc = 1;
}
