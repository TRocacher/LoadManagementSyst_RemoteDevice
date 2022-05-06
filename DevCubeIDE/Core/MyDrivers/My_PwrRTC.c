/*
 * My_Pwr_RTC.c
 *
 *  Created on: 28 avr. 2022
 *      Author: trocache
 */


#include "My_PwrRTC.h"



void My_PwrRTC_PORConf(void)
{
	// ***************** Type d'endormissement : standby with RTC (donc deepsleep pour le core)
	// bit SLEEPDEEP = 1 (Core M0, reg SCR - System Control_Reg du core-)

}
