/**
* @file binaryLogger.c
* @brief function definitions for binaryLogger, project3
* @author Andrew Kuklinski and Mason Darveaux
* @date 12/08/2017
**/


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "conversion.h"
#include "profilingFxn.h"

void log_data (uint8_t * data, uint8_t length)
{
	UART_send_n(data,length);
	return;
}

void log_string (uint8_t *input)
{
	uint8_t inputLength = 0;
	inputLength = strlen((char*)input);

	UART_send_n(input, inputLength);
	return;
}

void log_integer(uint16_t integerInput)
{
	uint8_t *valuePtr;
	uint32_t valueLength = 0;
	uint8_t arrayLength[5] = {0};

	valuePtr = my_itoa(integerInput,arrayLength,10);
	valueLength = getValueLength(integerInput);

	UART_send_n(valuePtr,valueLength);
	return;
}
