/*
 * hal_uart1.c
 *
 *  Created on: 19.04.2018
 *      Author: Patrick
 */

#include "tiva_headers.h"
#include "dl_LCD.h"
#include "dl_RF_Module.h"
#include "hal_uart1.h"
#include "hal_gpio.h"

COM_Status uart_status_;
uint8_t switch_backlight_;

void halUart1Init()
{
    // Enable Peripheral for TX and RX
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1));

    // Configure Clock for UART
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
                        UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);

    // Configure the GPIO Pin Mux for PC4
    // for U1RX
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, RF_TXD);   //RF-Transmit --> UART-Receive

    // Configure the GPIO Pin Mux for PC5
    // for U1TX
    GPIOPinConfigure(GPIO_PC5_U1TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, RF_RXD);   //UART-Transmit --> RF-Receive

    // enable UART for interrupt handling
    // Level-set: Inform about when interrupt should be triggert. After which buffer size?
    UARTIntRegister(UART1_BASE, &halGetUart1Data);
    UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    UARTIntEnable(UART1_BASE, UART_INT_RX);

    UARTEnable(UART1_BASE);

    // init uart status, no data available
    // data and len is zero by default, because of global variable
    //
    uart_status_.RxData.is_empty = 1;
}

uint8_t halSendUart1Data(uint8_t *data, uint8_t len)
{
    uint8_t cnt_send = 0, sending_failure = 0;

    if(len < 200)
    {
        for(cnt_send = 0; cnt_send < len; cnt_send++)
        {
            if(UARTCharPutNonBlocking(UART1_BASE, data[cnt_send]))
            {
                sending_failure = 0;
            }
            else
            {
                sending_failure = 1;
                break;
            }
        }
    }
    else
    {
        return 0;
    }

    if(!sending_failure)
        return 1;
    else
        return 0;
}

void halGetUart1Data()
{
    uint32_t uart_status = 0;
    static uint8_t cnt_rx = 0;
    uint16_t cnt_delete = 0;// wait_next = 0;

    if(uart_status_.RxData.is_empty)
    {
        for(cnt_delete = 0;  cnt_delete < 256; cnt_delete++)    //delete received buffer
            uart_status_.RxData.Data[cnt_delete] = 0;

        cnt_rx = 0;
    }
    else
    {
        if(uart_status_.RxData.len < 201)       // start where buffer has end, because buffer has
            cnt_rx = uart_status_.RxData.len;   // not been read yet
    }

    // get uart flags, so interrupt source can be determined
    //
    uart_status = UARTIntStatus(UART1_BASE, UART_INT_RX);

    // clear interrupt flag, so after exiting the function,
    // function do not get called again
    //
    UARTIntClear(UART1_BASE, UART_INT_RX);

    if(uart_status & UART_INT_RX)   //Received an Interrupt?
    {
        while(UARTCharsAvail(UART1_BASE))
        {
            uart_status_.RxData.Data[cnt_rx++] = UARTCharGet(UART1_BASE);
            SysCtlDelay(100);
        }
        uart_status_.RxData.len = cnt_rx;
        uart_status_.Status.R = 1;
        uart_status_.RxData.is_empty = 0;
    }

    if(uart_status_.RxData.Data[4] == 0x05)
        switch_backlight_ = 1;
    else if(uart_status_.RxData.Data[4] == 0x06)
        switch_backlight_ = 0;

}

/*
             if(((cnt_rx != 1) && (uart_status_.RxData.Data[cnt_rx-1] == 0x02)) || wait_next)    //new transmission
            {
                wait_next = 1;

                if((uart_status_.RxData.Data[cnt_rx-1] == CMD_GET_CHANNEL)  ||
                   (uart_status_.RxData.Data[cnt_rx-1] == CMD_GET_NODE)     ||
                   (uart_status_.RxData.Data[cnt_rx-1] == CMD_GET_FIRMWARE) ||
                   (uart_status_.RxData.Data[cnt_rx-1] == CMD_SEND_DATA)       )
                {
                    uart_status_.RxData.Data[0] = uart_status_.RxData.Data[cnt_rx-2];
                    uart_status_.RxData.Data[1] = uart_status_.RxData.Data[cnt_rx-1];

                    for(cnt_delete = cnt_rx-3; cnt_delete > 0; cnt_delete--)
                        uart_status_.RxData.Data[cnt_delete] = 0;
                    cnt_rx = 2;
                    wait_next = 0;
                }
            }
 */

