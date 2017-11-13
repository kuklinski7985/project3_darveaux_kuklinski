/**
* @file timerInit.h
* @brief creating a UART interface in order to have a method of transmitting
         and receiving data to development board
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/
#include <stdio.h>
#include <stdint.h>

#ifndef timerInit_h_
#define timerInit_h_


/**
 *@brief configures TMP for timer0 as counter
 *
 *@param none
 *
 *@return VOID
 */
void myTPM_init();


#endif /*__timerInit_h__*/
