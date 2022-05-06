/*
 * My_Pwr_RTC.h
 *
 *  Created on: 28 avr. 2022
 *      Author: trocache
 *
 *      NB : Reg definitions : stm32l041xx.h
 *
 *      This Driver is HAL free...
 *       *       *      Module Aims :
 *      - manage Pwr mode between RunMode and deepsleep Standby with RTC
 *      - manage RTC, just the counter and interrupt concerning the Periodic WakeUp Timer.
 *      the calendar won't be used.
 *      - manage BackUp Regs (5 32bits, ie 20 bytes)
 *
 *
 *      *****************************************
 *      Periheral used :
 *      - PWR
 *      - RTC
 *      - Core (for Pwr mode)
 *
 *      *****************************************
 *      Peripheral initialisation :
 *      In this file... Nothing with HAL
 *
 *      *****************************************
 *      Interrupts: RTC / WakeUp just µµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµµ PRECISER
 *
 *      *****************************************
 *      Functions :
 *      - My_PwrRTC_PORConf() : initalise le système pour avoir une RTC et un Pwr configuré pour basculer
 *      						entre standby et RunMode.
 *      - My_PwrRTC_WriteBackupRegs( char * ByteTab) : écrit entièrement les 20 octets du
 *      Backup regs
 *      - My_PwrRTC_ReadBackupRegs( char * ByteTab) : Récupère entièrement les 20 octets du
 *      Backup regs
 *
 */

#ifndef MYDRIVERS_MY_PWRRTC_H_
#define MYDRIVERS_MY_PWRRTC_H_



#endif /* MYDRIVERS_MY_PWRRTC_H_ */
