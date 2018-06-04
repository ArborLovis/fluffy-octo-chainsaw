/*
 * dl_LCD.c
 *
 *  Created on: 08.11.2017
 *      Author: Patrick
 */

#include "tiva_headers.h"
#include "dl_LCD.h"
#include "hal_gpio.h"
#include "hal_ssi1.h"
#include "driver_fonttable.h"

COM_Status LCD_cmd_;

uint8_t lcdInitCmd_[9] = {LCD_RESET_CMD, LCD_BIAS, ADC_SEL_NORMAL, COMMON_REVERSE,
                            RES_RATIO, ELEC_VOL_MODE, ELEC_VOL_VALUE, POWER_CONT, DISPLAY_ON};

uint8_t lcdCmdArray_[MAX_COLUMN] = {0};
uint8_t lcdPositionArray_[3] = {0};
uint8_t lcdDataArray_[MAX_COLUMN] = {0};

uint8_t lcd_data_buffer[256] = {0};

void dlLcdWriteCommand(uint8_t *data, uint8_t len)
{
    unsigned short cnt = 0;

    while(!LCD_cmd_.Status.B.TxSuc);
    LCD_cmd_.Status.B.TxSuc = 0;

    for(cnt = 0; cnt < len; cnt++)
    {
        LCD_cmd_.TxData.Data[cnt] = data[cnt];  //write to send buffer
    }

    LCD_SEND_CMD;      //pull A0 to Low, so sending commands are initialized
    SysCtlDelay(20);
    halUsciB1Transmit(len); //send data
}

void dlLcdInit()
{
    LCD_BACKLIGHT_ON;
    GPIOPinWrite(GPIO_PORTA_BASE, LCD_RESET, ~LCD_RESET);
    SysCtlDelay(1000000);
    GPIOPinWrite(GPIO_PORTA_BASE, LCD_RESET, LCD_RESET);
    //SysCtlDelay(100000);

    dlLcdWriteCommand(lcdInitCmd_, 9);
    while(!LCD_cmd_.Status.B.TxSuc);

    dlLcdClear();
}

void dlLcdClear()
{
    uint16_t sel_page = 0, j = MAX_COLUMN;

    for(j = MAX_COLUMN; j > 0; j--)
    {
        lcdDataArray_[j-1] = 0x00;  //delete data array
    }

    for(sel_page = NUM_PAGES; sel_page > 0; sel_page--)
    {
        dlLcdSetPosition(sel_page-1, 0);
        dlLcdWriteData(lcdDataArray_, MAX_COLUMN);
    }
}

void dlLcdSetPosition(uint8_t page, uint8_t col)
{
    lcdPositionArray_[0] = FIRST_PAGE + page;
    lcdPositionArray_[1] = ((col >> 4) | D4_COL_ADD);   //sending the MSB of the address; D4 has to be High
    lcdPositionArray_[2] = (col & 0x0F) & (~D4_COL_ADD);  //sending the LSB of the column address; D4 has to be Low

    //sending commands
    dlLcdWriteCommand(lcdPositionArray_, 3);
    while(!LCD_cmd_.Status.B.TxSuc);
}

void dlLcdWriteData(uint8_t *data, uint8_t len)
{
    unsigned short cnt = 0;

    while(!LCD_cmd_.Status.B.TxSuc);
    LCD_cmd_.Status.B.TxSuc = 0;

    for(cnt = 0; cnt < len; cnt++)
    {
        LCD_cmd_.TxData.Data[cnt] = data[cnt];
    }

    LCD_SEND_DATA;      //set A0 for sending data
    SysCtlDelay(20);
    halUsciB1Transmit(len);
    while(!LCD_cmd_.Status.B.TxSuc);
}

short dlLcdWriteText(uint8_t *text, uint8_t len, uint8_t page, uint8_t col)
{
    uint16_t i = 0, j = 0;
    int16_t offset = 0;
    uint8_t cnt_len = 0;

    if(page > 7 || col > 127)       //ERROR: page and column out of range
        return -1;

    //translate to text
    for(i = 0; i < len; i++)
    {
        for(j = 0; j < FONT_WIDTH; j++)
        {
            lcdDataArray_[offset+j] = font_table[*text][j];

            if(((++cnt_len + col) >= (MAX_COLUMN-FONT_WIDTH)) && (j >= FONT_WIDTH-1))     //the FONT_SIZE of 6 does not fit into 128 columns, so a line break has to made earlier
            {
                dlLcdSetPosition(page++, col);              //set courser position and go into the next row
                dlLcdWriteData(lcdDataArray_, cnt_len);     //writing data to display
                cnt_len = 0;
                col = 0;
                offset = -FONT_WIDTH;       //after writing data, offset has to be reseted
                if((page == NUM_PAGES) && (i < (len - 1)))
                    return -1;              //ERROR: text does not fit on whole display
            }
        }
        text++;                 //next sign
        offset += FONT_WIDTH;   //skip existing data in the array
    }

    if(!(page == NUM_PAGES))
    {
        //set courser on right position
        dlLcdSetPosition(page, col);

        //write data to the display
        dlLcdWriteData(lcdDataArray_, cnt_len);
    }

    return 0;   //no ERROR occured
}

short dlLcdWriteUInt (uint32_t num, uint8_t page, uint8_t col)
{
    int32_t size = 1;
    uint8_t duration = 0, i = 0;
    uint8_t NumInChar[5] = {0};

    if(page > 7 || col > 127)       //ERROR: page and column out of range
        return -1;

    if(!(num % 10000))
    {
        size = 10000;
        duration = 5;
    }
    else
    {
        while((num % size) != num)
        {
            size *= 10;
            duration++;
        }
    }

    if(duration != 5)
        size /= 10;

    for(i = 0; i<(5-duration);i++)
    {
        NumInChar[i] = 48;
    }

    for(i = (5-duration); i < 5; i++)
    {
        NumInChar[i] = (num / size) + 48;
        num = num % size;

        if(size >= 10)      //avoid division by zero
            size /= 10;
    }

    return dlLcdWriteText(NumInChar, 5, page, col);
}





