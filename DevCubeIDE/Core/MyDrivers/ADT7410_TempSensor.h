/*
 * ADT7410_TempSensor.h
 *
 *  Created on: 21 avr. 2022
 *      Author: trocache
 *
 *
 *      NB : Please check the i2c driver used in CubeMx. It must match the I2C_HandleTypeDef declared as
 *      externe at the begining of this header file.
 *      Default used : hi2c1
 *
 *      *****************************************
 *      Periheral used :
 *      I2C (I2C1)
 *
 *      *****************************************
 *      Peripheral initialisation :
 *      done with CubeMx, see main.c
 *
 *      *****************************************
 *      Interrupts : None
 *
 *
 *
 *      *****************************************
 *      Functions :
 *      void ADT7410_Init(void) - Prepare ADT7410 Device in shutdown mode, 16bits (LSB = 7.8125m°C)
 *      int ADT7410_GetTemp_fract_9_7(void) - blocking function (delay systick 250ms). Return Temperature
 *      on a 16 bits value, fractional format 9.7, °C.
 *
 */

#ifndef MYDRIVERS_ADT7410_TEMPSENSOR_H_
#define MYDRIVERS_ADT7410_TEMPSENSOR_H_


#include "stm32l0xx_hal.h"



// i2C associé (handle déclaré dans le main par cubemx)
extern I2C_HandleTypeDef hi2c1;

// Adresse I2C du ADT7410
#define ADT7410_Slave8bitsAdr (0x48<<1)

// Adresse des registres internes
#define ConfRegAdr 3
#define TempHighAdr 0
#define TempLowAdr 1



/**************************************************************
		Modes de fonctionnement

		Rem : 700µW (200µA / 3.3V) en Mode Normal
			  _      (2µA à 15µA / 3.3V et 5.2 à 25µA / 5V) en shutdown mode
			  150µW (45µA / 3.3V) en Mode 1 SPS

**************************************************************/

// Normal mode : rafale, la mesure se lance en rafale (240ms par mesure)
#define ConfReg_Mode_Normal (0x0)
// One Shot Mode : une fois demandé, la conversion est faite (240ms)  puis le circuit repasse en shutdown mode
#define ConfReg_Mode_OneShot (0x01<<5)
// 1 SPS - One Sample per Second (60ms de tps de conversion), le reste en idle mode
#define ConfReg_Mode_1SPS (0x2<<5)
// Shutdown mode :
#define ConfReg_Mode_Shutdown (0x3<<5)


/**************************************************************
		Résolution température

**************************************************************/
// 13 bits résolution = 3 LSB à jeter, bits 15..3 / 0.0625°C
#define ConfReg_Reso_13 (0x0)
// 16 bits résolution =  bits 15..0 / 7.8125m°C
#define ConfReg_Reso_16 (1<<7)



/**************************************************************
		Module Functions

**************************************************************/

void ADT7410_Init(void);
short int ADT7410_GetTemp_fract_9_7(void);


#endif /* MYDRIVERS_ADT7410_TEMPSENSOR_H_ */
