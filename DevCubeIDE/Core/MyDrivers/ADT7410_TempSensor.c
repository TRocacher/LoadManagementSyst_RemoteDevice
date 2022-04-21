/*
 * ADT7410_TempSensor.c
 *
 *  Created on: 21 avr. 2022
 *      Author: trocache
 */



#include "ADT7410_TempSensor.h"



// on shot mode, 16bits data format
static uint8_t data[15];

void ADT7410_Init(void)
{
	uint8_t ConfRegVal;
	ConfRegVal=ConfReg_Reso_16;
	ConfRegVal|=ConfReg_Mode_Shutdown;

	data[0]=ConfRegAdr;
	data[1]=ConfRegVal;

	HAL_I2C_Master_Transmit(&hi2c1, ADT7410_Slave8bitsAdr, data,2, HAL_MAX_DELAY);

}
