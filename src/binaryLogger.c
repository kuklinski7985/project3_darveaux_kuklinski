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
#include "circbuff.h"
#include "binaryLogger.h"

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

void log_integer(uint32_t integerInput)
{
	uint8_t *valuePtr;
	uint16_t integerLength = 0;
	uint8_t arrayLength[5] = {0};

	valuePtr = my_itoa(integerInput,arrayLength,10);
	integerLength = getValueLength(integerInput);

	UART_send_n(valuePtr,integerLength);
	return;
}

void log_flush(CB_t * circBuff)
{
	CB_status statusCheck = CB_is_empty(circBuff);
	while(statusCheck != buffer_empty)
	{
		statusCheck = CB_buffer_remove_item(circBuff, circBuff->poppedData);
		UART_send(circBuff->poppedData);
	}
	return;
}

void log_item(binLogger_t * userLogger, uint32_t logLength)
{

	return;
}

void loggerEvent_init(binLogger_t * inputEvent)
{
	inputEvent->logID = LOGGER_INIT;
	inputEvent->RTCtimeStamp = 34;
	inputEvent->logLength = 32;
	inputEvent->payload = 5;
	inputEvent->checkSum = 9;
	return;
}



