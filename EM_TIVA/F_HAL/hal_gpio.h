/*
 * hal_gpio.h
 *
 *  Created on: 15.03.2018
 *      Author: Patrick
 */

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

/* ~~~~~~~~~~~~~~~~~~~~ PORT A ~~~~~~~~~~~~~~~~~~~~ */
#define LCD_BACKLIGHT   GPIO_PIN_2
#define LCD_A0          GPIO_PIN_3
#define LCD_RESET       GPIO_PIN_4
#define US2_DRIVER_EN   GPIO_PIN_5
#define STEERING        GPIO_PIN_6
#define THROTTLE        GPIO_PIN_7

/* ~~~~~~~~~~~~~~~~~~~~ PORT B ~~~~~~~~~~~~~~~~~~~~ */
#define US1_SIGNAL_OUT  GPIO_PIN_0
#define US2_SIGNAL_OUT  GPIO_PIN_1
#define MOTION_I2C_SCL  GPIO_PIN_2
#define MOTION_I2C_SDA  GPIO_PIN_3
#define RADAR_SPI_SCLK  GPIO_PIN_4
#define RADAR_SPI_SYNC  GPIO_PIN_5

/* ~~~~~~~~~~~~~~~~~~~~ PORT C ~~~~~~~~~~~~~~~~~~~~ */
#define RF_TXD          GPIO_PIN_4
#define RF_RXD          GPIO_PIN_5

/* ~~~~~~~~~~~~~~~~~~~~ PORT D ~~~~~~~~~~~~~~~~~~~~ */
#define US1_DRIVER_EN   GPIO_PIN_0
#define RADAR_SPI_MOSI  GPIO_PIN_1
#define IR3_SENSE_OUT   GPIO_PIN_2
#define IR2_SENSE_OUT   GPIO_PIN_3
#define REMOTE_CH1      GPIO_PIN_6
#define REMOTE_CH2      GPIO_PIN_7

/* ~~~~~~~~~~~~~~~~~~~~ PORT E ~~~~~~~~~~~~~~~~~~~~ */
#define VBAT_MEASURE    GPIO_PIN_0
#define IR1_SENSE_OUT   GPIO_PIN_1
#define RADAR_IF1       GPIO_PIN_2
#define RADAR_IF2       GPIO_PIN_3
#define US2_SIGNAL_IN   GPIO_PIN_4
#define US1_SIGNAL_IN   GPIO_PIN_5

/* ~~~~~~~~~~~~~~~~~~~~ PORT F ~~~~~~~~~~~~~~~~~~~~ */
#define MOTION_INT      GPIO_PIN_0
#define LCD_SPI_MOSI    GPIO_PIN_1
#define LCD_SPI_SCLK    GPIO_PIN_2
#define LCD_SPI_CS      GPIO_PIN_3

#define GREEN_LED       GPIO_PIN_3
#define BLUE_LED        GPIO_PIN_2
#define RED_LED         GPIO_PIN_1
#define SWITCH1         GPIO_PIN_4

void halGpioInit();


#endif /* HAL_GPIO_H_ */
