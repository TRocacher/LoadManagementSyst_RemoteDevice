/*
 * ADT7410_TempSensor.c
 *
 *  Created on: 21 avr. 2022
 *      Author: trocache
 */



#include "ADT7410_TempSensor.h"




static uint8_t data[4];

// on shot mode, 16bits data format
void ADT7410_Init(void)
{
	uint8_t ConfRegVal;
	ConfRegVal=ConfReg_Reso_16;
	ConfRegVal|=ConfReg_Mode_Shutdown;

	data[0]=ConfRegAdr;
	data[1]=ConfRegVal;

	HAL_I2C_Master_Transmit(&hi2c1, ADT7410_Slave8bitsAdr, data,2, HAL_MAX_DELAY);
}


short int ADT7410_GetTemp_fract_9_7(void)
{
	uint8_t ConfRegVal;
	short int ReturnValue;

	// Lancement one shot
	ConfRegVal=ConfReg_Reso_16;
	ConfRegVal|=ConfReg_Mode_OneShot;
	data[0]=ConfRegAdr;
	data[1]=ConfRegVal;
	HAL_I2C_Master_Transmit(&hi2c1, ADT7410_Slave8bitsAdr, data,2, HAL_MAX_DELAY);


	// Wait at least 240ms
	HAL_Delay(250);


	// Read temperature
	HAL_I2C_Mem_Read(&hi2c1, ADT7410_Slave8bitsAdr, TempHighAdr,1,data,2,HAL_MAX_DELAY);
	ReturnValue=(data[0]<<8)+data[1];

	return ReturnValue;

	}

