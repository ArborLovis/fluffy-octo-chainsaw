/*
 * hali2c.h
 *
 *  Created on: 14.06.2018
 *      Author: Michael
 */

#ifndef F_HAL_HAL_I2C_H_
#define F_HAL_HAL_I2C_H_

#include "../F_TIVA/tiva_headers.h"

#define TINY_RTC_ADR 0x68
#define SLAVE_REG_TIME 0x00
#define SLAVE_REG_CONTROL 0x07
#define SLAVE_REG_STORE 0x08

void halI2cInit();

void I2cSendByte(uint8_t slave_addr, uint8_t data);
void I2cSendBurst(uint8_t slave_addr, uint8_t reg, uint8_t count, uint8_t* data);



uint32_t I2CReceiveSingle(uint8_t slave_addr, uint8_t reg);
void I2CReceiveBurst(uint8_t slave_addr, uint8_t reg, uint8_t count, uint32_t* data);



#endif /* F_HAL_HAL_I2C_H_ */
