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

binLogger_t logger_init;



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

CB_status log_flush(CB_t * circBuff)
{
	CB_status statusCheck = CB_is_empty(circBuff);
	while(statusCheck != buffer_empty)
	{
		statusCheck = CB_buffer_remove_item(circBuff, circBuff->poppedData);
		if(statusCheck == buffer_empty)
		{
			return statusCheck;
		}
		UART_send(circBuff->poppedData);
	}
	return statusCheck;
}

void log_item_KL25Z(binLogger_t * inputEvent, CB_t* logBuff)
{
	uint8_t spacer[] = " | ";
	uint8_t spacerLength = 3;
	uint8_t idPtr;
	uint16_t integerLength = 0;
	uint8_t arrayASCIIRTC[10] = {0};
	uint8_t arrayASCIIchecksum[10] = {0};

	/*****************adding ID to buffer***********************/
	my_itoa(inputEvent->logID, &idPtr,10);
	CB_buffer_add_item(logBuff, idPtr);


	/*****************adding format spacer***********************/
	for(uint8_t i=0; i<spacerLength; i++)
	{
		CB_buffer_add_item(logBuff, spacer[i]);
	}

	/*****************adding RTC timestamp to buffer*************/
	my_itoa(inputEvent->RTCtimeStamp, arrayASCIIRTC, 10);
	integerLength = getValueLength(inputEvent->RTCtimeStamp);

	for(uint8_t i=0; i<integerLength; i++)
	{
		CB_buffer_add_item(logBuff, arrayASCIIRTC[i]);
	}

	/*****************adding format spacer***********************/
	for(uint8_t i=0; i<spacerLength; i++)
	{
		CB_buffer_add_item(logBuff, spacer[i]);
	}

	/*****************adding payload to buffer*******************/
	for(uint8_t i=0; i<inputEvent->logLength; i++)
	{
		CB_buffer_add_item(logBuff,*inputEvent->payload);
		inputEvent->payload++;
	}

	/*****************adding format spacer***********************/
	for(uint8_t i=0; i<spacerLength; i++)
	{
		CB_buffer_add_item(logBuff, spacer[i]);
	}

	/*****************adding checksum to buffer******************/

	integerLength = 0;
	my_itoa(inputEvent->checkSum, arrayASCIIchecksum, 10);
	integerLength = getValueLength(inputEvent->checkSum);

	for(uint8_t i=0; i<integerLength; i++)
	{
		CB_buffer_add_item(logBuff, arrayASCIIchecksum[i]);
	}

	CB_buffer_add_item(logBuff,0x0d);
	log_flush(logBuff);
	return;
}
