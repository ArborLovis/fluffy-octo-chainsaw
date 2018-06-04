/*
 * hal_ssi1.h
 *
 *  Created on: 10.04.2018
 *      Author: Patrick
 */

#ifndef F_HAL_HAL_SSI1_H_
#define F_HAL_HAL_SSI1_H_

/* ~~~~~~~~~~~~ DEFINES ~~~~~~~~~~~~ */

#define SSI_DATA_LEN    8
#define SSI_CLK_SPEED   100000

/* ~~~~~~~~~~~~ FUNCTIONS ~~~~~~~~~~~~ */

void halSsi1Int();
void halUsciB1Transmit(unsigned short len);

#endif /* F_HAL_HAL_SSI1_H_ */
