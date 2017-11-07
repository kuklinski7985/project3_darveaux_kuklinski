/**
* @file uart.h
* @brief creating a UART interface in order to have a method of transmitting
         and receiving data to development board
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/


#include <stdio.h>
#include <stdint.h>

#ifndef uart_h_
#define uart_h_

#define UART0_CLK_GATE_MASK 0x00000400	//bit 10
#define UART1_CLK_GATE_MASK 0x00000800  //bit 11
#define UART2_CLK_GATE_MASK 0x00001000  //bit 12
#define BUSCLK 48000000u // BUS_CLK
#define BAUD 38400 // define UART baud rate here

#define U0
//#define U1
//#define U2




/**
 *@brief configures UART
 *
 *@param none
 *
 *@return VOID
 */
void UART_configure();


/**
 *@brief sends a single btye down a specific UART device, should block  on transmitting data
 *
 *@param "data" data to be sent
 *
 *@return VOID
 */
void UART_send(uint8_t * data);


/**
 *@brief transmits a continuous block of data
 *
 *@param "data" pointer to data to be transmitted
 *@param "length" length of data to be transmitted
 *
 *@return VOID
 */
void UART_send_n(uint8_t * data, uint8_t length);


/**
 *@brief receives a single byte of data, should block until character is received
 *
 *@param "inputData"
 *
 *@return byte of data that was sent
 */
uint8_t UART_receive(uint8_t* data);


/**
 *@brief receives a number of bytes on the UART
 *
 *@param "data" pointer to memory location to place data that is being received
 *@param "length" number of items to receive
 *
 *@return
 */
uint8_t * UART_receive_n(uint8_t * data, uint8_t length);


/**
 *@brief handles receive and transmitt interrups and clears their associated flag when completed but only if they were set
 *
 *@param none
 *
 *@return VOID
 */
//void UART0_IRQHandler();


#endif /*__uart_h__*/
