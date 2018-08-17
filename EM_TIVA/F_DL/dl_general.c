/*
 * dl_general.c
 *
 *  Created on: 22.03.2018
 *      Author: Patrick
 */

#include "dl_general.h"

volatile Sensor sensor_data_;

void dlSetSteering(int16_t ctr_val)
{
    int pwm_value = 0;

    // scale values to pwm_values
    if(ctr_val < 0)
    {
        pwm_value = STEERING_MIDDLE + ctr_val * 4;
    }
    else if(ctr_val > 0)
    {
        pwm_value = STEERING_MIDDLE + ctr_val * 4;
    }
    else
    {
        pwm_value = STEERING_MIDDLE;
    }

    // value limit to min max values
    if(pwm_value > STEERING_MAX_RIGHT)
        pwm_value = STEERING_MAX_RIGHT;
    else if(pwm_value < STEERING_MAX_LEFT)
        pwm_value = STEERING_MAX_LEFT;

    //set new pulse width, write to register
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, pwm_value);
}

//NOTE: min. forward speed: 1560 PMW value
    //  max. forward speed: 2470 PWM value
void dlSetThrottle(SpeedMode dir, int16_t speed)
{
    static int16_t pwm_speed;
    uint32_t wait = 50000;

    switch(dir)
    {
        case FORWARD:
            if((speed >= 0) && (speed <= 100))
                pwm_speed = MIN_FPW_ESC + speed * 9;
            else if(speed < 0)
                pwm_speed = MIN_FPW_ESC;
            else
                pwm_speed = MAX_FPW_ESC;
        break;

        case BACKWARD:                          //ranges for driving backward and braking are fictive
            if((speed >= 0) && (speed <= 100))  //motor controller drives only forward at the moment
               pwm_speed = MAX_RPW_ESC - (speed * 25);
           else if(speed > 100)
               pwm_speed = MAX_RPW_ESC;
           else
               pwm_speed = MIN_RPW_ESC;
        break;

        case BRAKE:
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, 1000);
            while(--wait);

            if((speed >= 0) && (speed <= 100))
            {
                pwm_speed = MAX_BRAKE_ESC - (speed << 3);
            }
            else if (speed > 100)
                pwm_speed = MAX_BRAKE_ESC - (100 << 3);
            else
                pwm_speed = MAX_BRAKE_ESC;
        break;
    }

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, pwm_speed);
}
