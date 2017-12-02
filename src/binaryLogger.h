/**
* @file binaryLogger.h
* @brief functions for creating a binary logger, using log ID and payload to log data to serial output
* @author Andrew Kuklinski and Mason Darveaux
* @date 12/08/2017
**/


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "uart.h"


#ifndef binaryLogger_h_
#define binaryLogger_h_


/**
 *@brief UART transmit wrap function
 *
 *@param data: pointer to a sequence of bytes, length: length of bytes to log
 *
 *@return VOID
 */
void log_data (uint16_t * data, uint32_t length);

/**
 *@brief UART function that takes a string and outputs to terminal
 *
 *@param input: c-string, example "this is a test"
 *
 *@return VOID
 */
void log_string (const char *input);

/**
 *@brief accepts an integer and outputs to terminal over UART
 *
 *@param integerInput: number value to output
 *
 *@return VOID
 */
void log_integer(uint8_t integerInput);


#endif /*binaryLogger_h_*/
