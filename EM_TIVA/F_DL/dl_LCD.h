/*
 * dl_LCD.h
 *
 *  Created on: 08.11.2017
 *      Author: Patrick
 */

#include "hal_gpio.h"

#ifndef DL_DL_LCD_H_
#define DL_DL_LCD_H_

#define LCD_RESET_CMD           0xD2
#define LCD_BIAS                0xA3
#define ADC_SEL_NORMAL          0xA0
#define COMMON_REVERSE          0xC8
#define RES_RATIO               0x24
#define ELEC_VOL_MODE           0x81
#define ELEC_VOL_VALUE          0x0F    //settings for contrast
#define POWER_CONT              0x2F
#define DISPLAY_ON              0xAF
#define DISPLAY_ALL_POINTS_ON   0xA5

#define NUM_PAGES               8
#define FIRST_PAGE              0xB0    //set courser to first page
#define LAST_PAGE               0xB7    //last page of the display
#define D4_COL_ADD              0x10    //Bit 4 has to be set while writing the MSB address and reset while writing the LSB

#define MAX_COLUMN              128

#define LCD_SEND_DATA           GPIOPinWrite(GPIO_PORTA_BASE, LCD_A0, LCD_A0)
#define LCD_SEND_CMD            GPIOPinWrite(GPIO_PORTA_BASE, LCD_A0, ~LCD_A0)
#define LCD_BACKLIGHT_ON        GPIOPinWrite(GPIO_PORTA_BASE, LCD_BACKLIGHT, LCD_BACKLIGHT)
#define LCD_BACKLIGHT_OFF       GPIOPinWrite(GPIO_PORTA_BASE, LCD_BACKLIGHT, ~LCD_BACKLIGHT)
#define LCD_SET_CS              GPIOPinWrite(GPIO_PORTF_BASE, LCD_SPI_CS, LCD_SPI_CS)
#define LCD_RESET_CS            GPIOPinWrite(GPIO_PORTF_BASE, LCD_SPI_CS, ~LCD_SPI_CS)

typedef struct
{
    union
    {
        unsigned char R;
        struct
        {
            unsigned char TxSuc:1;      //set Bit, if data transmitted
            unsigned char dummy:7;
        }B;

    }Status;

    struct
    {
        unsigned char len; // Länge der Daten in Bytes die übertragen werden
        unsigned char cnt; // Index auf momentan übertragene Daten
        unsigned char Data[256]; // Tx Daten Array
    }TxData;

    struct
    {
        unsigned char len;          // Länge der empfangenen Daten
        unsigned char is_empty;   // check if buffer has already read
        unsigned char Data[256];    // Rx Daten Array
    }RxData;

}COM_Status;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ dlLcdWriteCommand ~~~~~~~~~~~~~~~~~~~~  */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
void dlLcdWriteCommand(uint8_t *data, uint8_t len);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ dlLcdWriteCommand ~~~~~~~~~~~~~~~~~~~~  */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
void dlLcdInit();

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ dlLcdClear ~~~~~~~~~~~~~~~~~~~~~~~~~~~  */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
void dlLcdClear();

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ dlLcdSetPosition ~~~~~~~~~~~~~~~~~~~~~  */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
void dlLcdSetPosition(uint8_t page, uint8_t col);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ dlLcdWriteText ~~~~~~~~~~~~~~~~~~~~~~~  */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
short dlLcdWriteText(uint8_t *text, uint8_t len, uint8_t page, uint8_t col);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ dlLcdWriteUInt ~~~~~~~~~~~~~~~~~~~~~~~  */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
short dlLcdWriteUInt (uint32_t num, uint8_t page, uint8_t col);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ dlLcdWriteData ~~~~~~~~~~~~~~~~~~~~~~~  */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
void dlLcdWriteData(uint8_t *data, uint8_t len);

#endif /* DL_DL_LCD_H_ */
