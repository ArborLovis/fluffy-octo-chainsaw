/*
 * dl_general.c
 *
 *  Created on: 22.03.2018
 *      Author: Patrick
 */

#include "dl_general.h"

void dlSetSteering(int16_t ctr_val)
{
    int pwm_value = 0;

    // scale values to pwm_values
    if(ctr_val < 0)
    {
        pwm_value = STEERING_MIDDLE + ctr_val * 5;
    }
    else if(ctr_val > 0)
    {
        pwm_value = STEERING_MIDDLE + ctr_val * 5;
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

void dlSetThrottle(SpeedMode dir, int16_t speed)
{
    static int16_t pwm_speed;

    switch(dir)
    {
        case FORWARD:
            if((speed >= 0) && (speed <= 100))
                pwm_speed = MIN_FPW_ESC + speed * 25;
            else if(speed < 0)
                pwm_speed = MIN_FPW_ESC;
            else
                pwm_speed = MAX_FPW_ESC;
        break;

        case BACKWARD:
            if((speed >= 0) && (speed <= 100))
               pwm_speed = MIN_RPW_ESC - speed * 25;
           else if(speed > 100)
               pwm_speed = MAX_RPW_ESC;
           else
               pwm_speed = MIN_RPW_ESC;
        break;

        case BRAKE:
            if(pwm_speed <= MAX_BRAKE_ESC)
            {
                if((speed >= 0) && (speed <= 50))
                    pwm_speed = MIN_RPW_ESC + speed * 25;
                else if(speed > 50)
                    pwm_speed = MAX_BRAKE_ESC;
                else
                    pwm_speed = MIN_RPW_ESC;
            }
            else
            {
               if((speed >= 0) && (speed <= 50))
                   pwm_speed = MIN_FPW_ESC - speed * 25;
               else if(speed > 50)
                   pwm_speed = MAX_BRAKE_ESC;
               else
                   pwm_speed = MIN_RPW_ESC;
            }
        break;
    }

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, pwm_speed);
}
