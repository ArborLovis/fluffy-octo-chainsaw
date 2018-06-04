/*
 * hal_uart.h
 *
 *  Created on: 19.04.2018
 *      Author: Patrick
 */

#ifndef F_HAL_HAL_UART1_H_
#define F_HAL_HAL_UART1_H_

/* ~~~~~~~~~~~~ FUNCTIONS ~~~~~~~~~~~~ */

void halUart1Init();
void halGetUart1Data();
uint8_t halSendUart1Data(uint8_t *data, uint8_t len);


#endif /* F_HAL_HAL_UART1_H_ */
