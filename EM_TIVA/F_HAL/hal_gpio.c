/*
 * hal_gpio.c
 *
 *  Created on: 15.03.2018
 *      Author: Patrick
 */

// Note: Avoid using "true"and "false" as parameter.
//       For setting Pins it is recommended using the mask to ensure a safe working.

#include "hal_gpio.h"
#include "../F_TIVA/tiva_headers.h"

/* ~~~~~~~~~~~~ GLOBAL VARIABLES ~~~~~~~~~~~~ */

uint8_t is_switch_active = 0;

/* ~~~~~~~~~~~~ FUNCTIONS ~~~~~~~~~~~~ */

void configPort_A();
void configPort_B();
void configPort_C();
void configPort_D();
void configPort_E();
void configPort_F();

void extIntFHandler();

void halGpioInit()
{
/* ~~~~~~~~~~~~~~~~~~~~ PORT A ~~~~~~~~~~~~~~~~~~~~ */
    configPort_A();

/* ~~~~~~~~~~~~~~~~~~~~ PORT B ~~~~~~~~~~~~~~~~~~~~ */
    configPort_B();

/* ~~~~~~~~~~~~~~~~~~~~ PORT C ~~~~~~~~~~~~~~~~~~~~ */
    configPort_C();

/* ~~~~~~~~~~~~~~~~~~~~ PORT D ~~~~~~~~~~~~~~~~~~~~ */
    configPort_D();

/* ~~~~~~~~~~~~~~~~~~~~ PORT E ~~~~~~~~~~~~~~~~~~~~ */
    configPort_E();

/* ~~~~~~~~~~~~~~~~~~~~ PORT F ~~~~~~~~~~~~~~~~~~~~ */
    configPort_F();
}

void configPort_A()
{
    //enable peripheral of Port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)); //wait for PORT to be ready

    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, THROTTLE | STEERING);

    //enable US2 Driver Pin - config as output
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, US2_DRIVER_EN);
    GPIOPinWrite(GPIO_PORTA_BASE, US2_DRIVER_EN, US2_DRIVER_EN);       // enable US2
}

void configPort_B()
{
    //enable peripheral of Port B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)); //wait for PORT to be ready

    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);     //LED on Port B - Pin 5
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, ~GPIO_PIN_5);       // turn off LED

    //TEST other output pins
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);     //LED on Port B - Pin 5
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, ~(GPIO_PIN_0 | GPIO_PIN_1));

    // ***** DEBUG BEGIN *****
    uint32_t driver_strength = 99, pad_type = 99;
    GPIOPadConfigGet(GPIO_PORTB_BASE, GPIO_PIN_5, &driver_strength, &pad_type);
    driver_strength = 99; pad_type = 99;
    GPIOPadConfigGet(GPIO_PORTB_BASE, GPIO_PIN_0, &driver_strength, &pad_type);
    driver_strength = 99; pad_type = 99;
    GPIOPadConfigGet(GPIO_PORTB_BASE, GPIO_PIN_1, &driver_strength, &pad_type);
    // ***** DEBUG END *****

    //enable US1 & US2 Receive Pin - config as input
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, US1_SIGNAL_OUT | US2_SIGNAL_OUT);
}

void configPort_C()
{
    //enable peripheral of Port C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)); //wait for PORT to be ready
}

void configPort_D()
{
    //enable peripheral of Port D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)); //wait for PORT to be ready

    //enable US1 Driver Pin - config as output
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, US1_DRIVER_EN);
    GPIOPinWrite(GPIO_PORTD_BASE, US1_DRIVER_EN, US1_DRIVER_EN);    //enable US1

    //Debug of PWM0 - Interrupt
    //Test-Pin for ISR of PWM0 Interrupt
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
}

void configPort_E()
{
    //enable peripheral of Port E
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)); //wait for PORT to be ready

    //controlling pins of the US module - for sending an impulse (burst mode)
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, US1_SIGNAL_IN | US2_SIGNAL_IN);

    //set analog inputs
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, VBAT_MEASURE);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, IR1_SENSE_OUT);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, IR2_SENSE_OUT);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, IR3_SENSE_OUT);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, RADAR_IF1);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, RADAR_IF2);
}

void configPort_F()
{
    //enable peripheral of Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); //wait for PORT to be ready

    //config switch SW2 on PF as input
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SWITCH1);

    //activate external Interrupt
    GPIOIntClear(GPIO_PORTF_BASE, 0xFF);    //clear all interrupt flags
    GPIOIntRegister(GPIO_PORTF_BASE, extIntFHandler);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_INT_PIN_4, GPIO_RISING_EDGE);  //interrupt is triggerd  by rising edge
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4); //enable ext. Int. of Pin 4 -> Switch
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_INT_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
}

void extIntFHandler()
{
    if(GPIO_INT_PIN_4 & GPIOIntStatus(GPIO_PORTF_BASE, GPIO_INT_PIN_4))   //returns masked interrupt status; SW1 on Tiva Board (PF4)
    {
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);      //clear flag of Switch
        is_switch_active = !is_switch_active;
        SysCtlDelay(10000); //Debounce --> just for fast debugging
    }
}
