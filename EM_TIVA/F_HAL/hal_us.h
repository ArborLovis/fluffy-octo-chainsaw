/*
 * hal_us.h
 *
 *  Created on: 17.05.2018
 *      Author: Patrick
 */

#ifndef F_HAL_HAL_US_H_
#define F_HAL_HAL_US_H_

/* ~~~~~~~~~~~~ DEFINES ~~~~~~~~~~~~ */

#define CLK_FREQ        16000000    //16MHz
#define CLK_FREQ_PWM    1000000     //1MHz
#define US_BURST_FREQ   40000       //40kHz
#define CNT_PERIOD_US   CLK_FREQ_PWM / US_BURST_FREQ    //25
#define CNT_WIDTH_US    (CNT_PERIOD_US / 2)             // 25/2 = ~12

#define BURST_CNT_LEN   40


/* ~~~~~~~~~~~~ FUNCTIONS ~~~~~~~~~~~~ */

void halUsInit();
void halStartBurstModeUS();
unsigned short halIsBurstFinished();

#endif /* F_HAL_HAL_US_H_ */
