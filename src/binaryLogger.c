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

void log_item(binLogger_t * logStruct)
{

	//log_string(logger_init0->payload);
	return;
}

void loggerEvent_init(binLogger_t * inputEvent,
		logger_status logEvent, uint8_t inputlogLength, char* inputpayload)
{
	inputEvent->logID = logEvent;
	inputEvent->RTCtimeStamp = 0;
	inputEvent->logLength = inputlogLength;
	inputEvent->payload = inputpayload;
	inputEvent->checkSum = 0;
	return;
}

void logger_init_all()
{
	logger_init0 = (binLogger_t*) malloc(sizeof(binLogger_t));
	if(logger_init0 == NULL)
	{
		return;
	}
	loggerEvent_init(logger_init0, LOGGER_INIT, 32, "logger initialized");
	return;
}
uint32_t checksumOnesCount(binLogger_t inputEvent)
{
	/*uint8_t x = 0;
	uint8_t addedValue;
	uint32_t checkSumValue = 0;

	for(x=0; x < ((sizeof(inputEvent))*8); x++)
	{
		addedValue = inputEvent & 0x1;
		if(addedValue == 1)
		{
			checkSumValue++;
			inputEvent<<1;
		}
		else
		{
			inputEvent<<1;
		}
	}

	return checkSumValue;*/
	return 0;
}



