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
#include "loggerQueue.h"
#include "rtc.h"
#include "MKL25Z4.h"


void log_data (uint8_t * data, uint8_t length)
{
	UART_send_n(data,length);
	return;
}

void log_data_single(uint8_t * data)
{
	UART_send(data);
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

CB_status log_flush(CB_t * inputBuffer)
{
	CB_status statusCheck = loggingBuffer_empty(inputBuffer);
	while(statusCheck != buffer_empty)
	{
		statusCheck = loggerRemoveFromBuffer(inputBuffer, inputBuffer->poppedData);
		if(statusCheck == buffer_empty)
		{
			return statusCheck;
		}
		#ifdef PROFILEKL25Z
			UART_send(inputBuffer->poppedData);
		#endif

		#if defined (PROFILEHOST) || defined (PROFILEBBB)
			printf("%c",inputBuffer->poppedData);
		#endif
	}
	return statusCheck;
}

void log_item(binLogger_t * inputEvent, CB_t* logBuff)
{
	uint8_t idPtr[2] = {0};
	uint8_t loglengthPtr[10] = {0};
	uint16_t integerLength = 0;
	uint8_t arrayASCIIRTC[10] = {0};
	uint8_t arrayASCIIchecksum[10] = {0};

	/*****************adding ID to buffer***********************/
	my_itoa(inputEvent->logID, idPtr,10);
	integerLength = getValueLength(inputEvent->logID);

	for(uint8_t i=0; i<integerLength; i++)
	{
		loggerEventToBuffer(logBuff, idPtr[i]);
	}

	/*****************adding format spacer***********************/

	CB_buffer_add_item(logBuff, 0x20);		//adding a space
	CB_buffer_add_item(logBuff, 0x7C);		//adding a bar
	CB_buffer_add_item(logBuff, 0x20);		//adding a space

	/*****************adding RTC timestamp to buffer*************/
	my_itoa(inputEvent->RTCtimeStamp, arrayASCIIRTC, 10);
	integerLength = getValueLength(inputEvent->RTCtimeStamp);

	for(uint8_t i=0; i<integerLength; i++)
	{
		loggerEventToBuffer(logBuff, arrayASCIIRTC[i]);
	}

	/*****************adding format spacer***********************/
	loggerEventToBuffer(logBuff, 0x20);		//adding a space
	loggerEventToBuffer(logBuff, 0x7C);		//adding a bar
	loggerEventToBuffer(logBuff, 0x20);		//adding a space

	/*****************adding log length to buffer****************/
	my_itoa(inputEvent->logLength, loglengthPtr,10);
	integerLength = getValueLength(inputEvent->logLength);

	for(uint8_t i=0; i<integerLength; i++)
	{
		loggerEventToBuffer(logBuff, loglengthPtr[i]);
	}

	/*****************adding format spacer***********************/
	CB_buffer_add_item(logBuff, 0x20);		//adding a space
	CB_buffer_add_item(logBuff, 0x7C);		//adding a bar
	CB_buffer_add_item(logBuff, 0x20);		//adding a space

	/*****************adding payload to buffer*******************/
	for(uint8_t i=0; i<inputEvent->logLength; i++)
	{
		loggerEventToBuffer(logBuff,*inputEvent->payload);
		inputEvent->payload++;
	}

	/*****************adding format spacer***********************/
	loggerEventToBuffer(logBuff, 0x20);		//adding a space
	loggerEventToBuffer(logBuff, 0x7C);		//adding a bar
	loggerEventToBuffer(logBuff, 0x20);		//adding a space

	/*****************adding checksum to buffer******************/

	integerLength = 0;
	my_itoa(inputEvent->checkSum, arrayASCIIchecksum, 10);
	integerLength = getValueLength(inputEvent->checkSum);

	for(uint8_t i=0; i<integerLength; i++)
	{
		loggerEventToBuffer(logBuff, arrayASCIIchecksum[i]);
	}

	loggerEventToBuffer(logBuff,0x0d);

	log_flush(logBuff);		//outputs the entire buffer up this point
	return;
}

void logOutputData(binLogger_t *logEvent, uint8_t * inputPayload,
		logger_status enumStatus)
{
	logEvent->logID = enumStatus;
	logEvent->payload = inputPayload;
	logEvent->logLength = strlen((char*)logEvent->payload);
	logEvent->RTCtimeStamp = RTC_TSR;  //need a function

	uint32_t checkSumValue = 0;
	checkSumValue = (logEvent->logID) + (logEvent->RTCtimeStamp) +
			(logEvent->logLength);
	logEvent->checkSum = checkSumValue;
	return;
}

