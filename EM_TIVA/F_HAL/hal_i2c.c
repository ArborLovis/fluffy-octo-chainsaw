/*
 * hali2c.c
 *
 *  Created on: 14.06.2018
 *      Author: Michael
 */

#include <hal_i2c.h>
#include "hal_gpio.h"
#include "../F_TIVA/tiva_headers.h"

void halI2cInit()
{
    //enable I2C module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0)){};

    //Reset the module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    //enable GPIO peripheral that contains I2C 0
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); -- Done in gpio.c

    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, MOTION_I2C_SCL);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, MOTION_I2C_SDA);

    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    //clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}


void I2cSendByte(uint8_t slave_addr, uint8_t data)
{
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);    //--false = Writing, true = reading

    // Put data into FIFO
    I2CMasterDataPut(I2C0_BASE, data);

    //Sending the data from MCU
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C0_BASE)){};

}

void I2cSendBurst(uint8_t slave_addr, uint8_t reg, uint8_t count, uint8_t* data)
{
       // Tell the master module what address it will place on the bus when
       // communicating with the slave.
       I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);    //--false = Writing, true = reading

       // Put register to start into FIFO
       I2CMasterDataPut(I2C0_BASE, reg);

       //Sending the data from MCU
       I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
       while(I2CMasterBusy(I2C0_BASE)){};

       if(count > 1)
       {

           int i =0;
           for(i=0; i<count-1; i++)
           {
               I2CMasterDataPut(I2C0_BASE, data[i]);

               I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);  //Read the next register 0x3C
               //wait for MCU to finish transaction
               while(I2CMasterBusy(I2C0_BASE));
           }
       }
       I2CMasterDataPut(I2C0_BASE, data[count-1]);
       I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);  //Read the next register 0x3C
       //wait for MCU to finish transaction
       while(I2CMasterBusy(I2C0_BASE));
}


//Read a Single Cmd from Slaveadress start with register "reg"
uint32_t I2CReceiveSingle(uint8_t slave_addr, uint8_t reg)
{

    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //specify register to be read
    I2CMasterDataPut(I2C0_BASE, reg);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);

    //send control byte and read from the register we
    //specified
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    //I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //return data pulled from the specified register
    return I2CMasterDataGet(I2C0_BASE);
}



//Read a BURST from the Slave_Address from reg up to end
void I2CReceiveBurst(uint8_t slave_addr, uint8_t reg, uint8_t count, uint32_t* data)
{
    int i=0;

    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //specify register to be read
    I2CMasterDataPut(I2C0_BASE, reg);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);

    //send control byte and read from the register we
    //specified
    //I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    if(count > 2)
    {
        for(i=0; i<count-2; i++)
        {
           data[i] = I2CMasterDataGet(I2C0_BASE);
           I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);  //Read the next register 0x3C
           //wait for MCU to finish transaction
           while(I2CMasterBusy(I2C0_BASE));
        }
    }

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); //Finish the "burst read"
    data[count] = I2CMasterDataGet(I2C0_BASE);
}
































