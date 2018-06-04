/*
 * dl_RF_Module.c
 *
 *  Created on: 20.04.2018
 *      Author: Patrick
 */

#include "../tiva_headers.h"
#include "dl_RF_Module.h"
#include "dl_LCD.h"
#include "hal_uart1.h"

extern COM_Status uart_status_;

void dlSetChannelRF(uint8_t adress)
{
    uint8_t cmd_channel_conf[CMD_LEN] = {STX_BYTE, CMD_SET_CHANNEL, adress};
    while(UARTBusy(UART1_BASE));    //wait until UART is ready
    halSendUart1Data(cmd_channel_conf, CMD_LEN);

    while(UARTBusy(UART1_BASE));    //wait until data is transmitted

    SysCtlDelay(3000000);   //wait some time, before next transmit takes place
}

short dlGetSettingInfoRF(RF_info select)
{
    uint8_t cmd_channel_conf[CMD_LEN] = {STX_BYTE, CMD_GET_CHANNEL, CMD_DUMMY};
    uint8_t data_length = 0;
    unsigned int wait = 0;

    while(UARTBusy(UART1_BASE));    //wait until UART is ready

    if(select == CAR_ADDRESS)
        cmd_channel_conf[1] = CMD_GET_NODE;
    else if(select == FIRMWARE)
        cmd_channel_conf[1] = CMD_GET_FIRMWARE;

    halSendUart1Data(cmd_channel_conf, CMD_LEN);
    while(UARTBusy(UART1_BASE));    //wait until data is transmitted
    SysCtlDelay(30000);   //wait some time for an answer

    while(!uart_status_.Status.R)  //wait until data has arrived and has read from buffer
    {
        if(wait++ == 65530)     //exit loop, escape plan (here: code has to be adapted)
            break;
    }
    if(dlGetDataPayloadRF(cmd_channel_conf, &data_length))  //reuse of variable ;-)
    {
        if(data_length == 1)
            return cmd_channel_conf[0];     //return channel or node address
        else
        {
            return 1;   //for now, ignore firmware --> update follow
        }
    }
    else
        return -1;  //error by reading info
}

void dlSendDataRF(uint8_t rx_address, uint8_t* data, uint8_t len)
{
    uint8_t send_buffer[CMD_LEN+1] = {STX_BYTE, CMD_SEND_DATA, rx_address, len};
    while(UARTBusy(UART1_BASE));
    halSendUart1Data(send_buffer, CMD_LEN+1);
    halSendUart1Data(data, len);
    SysCtlDelay(3000000);
    while(UARTBusy(UART1_BASE));
}

void dlSetCarAddress(uint8_t car_address)
{
    uint8_t send_buffer[CMD_LEN] = {STX_BYTE, CMD_SET_NODE, car_address};
    while(UARTBusy(UART1_BASE));
    halSendUart1Data(send_buffer, CMD_LEN);
    while(UARTBusy(UART1_BASE));
    SysCtlDelay(3000000);
}

short dlGetDataPayloadRF(uint8_t *payload, uint8_t *len)
{
    short loop_cnt = 0;
    short cnt_payload = 0;

    if(uart_status_.RxData.is_empty)
        return 0;   //no data available
    else
    {
        do
        {
            if(uart_status_.RxData.Data[loop_cnt] == STX_BYTE)  // start byte detection --> new data cycle
            {
                loop_cnt++;

                switch (uart_status_.RxData.Data[loop_cnt])
                {
                    case CMD_GET_CHANNEL:
                        *payload = uart_status_.RxData.Data[++loop_cnt];
                        *len = 1;
                        break;
                    case CMD_GET_NODE:
                        *payload = uart_status_.RxData.Data[++loop_cnt];
                        *len = 1;
                        break;
                    case CMD_GET_FIRMWARE:
                        if((loop_cnt + 3) <= uart_status_.RxData.len)
                        {
                            short i = 0;

                            loop_cnt++;
                            for(; i < 3; i++)
                                *payload = uart_status_.RxData.Data[loop_cnt + i];
                            loop_cnt += i;
                            *len = 3;
                        }
                        break;
                    case CMD_SEND_DATA:         // data from uart was a data command
                        loop_cnt++;
                        if(uart_status_.RxData.Data[loop_cnt] == PC_NODE_ADR)   //came the data from PC? --> else: ignore data
                        {
                            *len = uart_status_.RxData.Data[++loop_cnt];
                            loop_cnt++;
                            while((cnt_payload + loop_cnt) != (uart_status_.RxData.len))    //get payload, start by zero
                            {
                                payload[cnt_payload] = uart_status_.RxData.Data[cnt_payload + loop_cnt];
                                cnt_payload++;
                            }
                            loop_cnt += cnt_payload;
                        }
                        break;
                    default:
                        // 0x02 was not a start byte
                        break;
                }
            }
            else
                loop_cnt++;

        }while(uart_status_.RxData.len != loop_cnt);

        uart_status_.RxData.is_empty = 1;
        uart_status_.Status.R = 0;
        return 1;   //payload ready
    }
}

