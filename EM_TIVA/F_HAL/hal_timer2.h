/*
 * hal_timer2.h
 *
 *  Created on: 06.06.2018
 *      Author: Patrick
 */

#ifndef F_HAL_HAL_TIMER2_H_
#define F_HAL_HAL_TIMER2_H_

#define TIMER_CNT_LEN   16

void halTimer2Init();

void halStartTimer2_A();
void halStopTimer2_A();
void halStartTimer2_B();
void halStopTimer2_B();
void halStartTimer2_Both();
void halStopTimer2_Both();

#endif /* F_HAL_HAL_TIMER2_H_ */
