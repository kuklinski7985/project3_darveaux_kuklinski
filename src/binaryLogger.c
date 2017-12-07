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

/*this function now needs to take the struct and put into the buffer and then
 * use the UART to print to the screen
 * */


void log_item_KL25Z(binLogger_t * inputEvent, CB_t* logBuff)
{
	uint8_t spacer[] = " | ";
	uint8_t spacerLength = 3;
	uint16_t integerLength = 0;
	uint8_t arrayASCIIRTC[7] = {0};
	uint8_t arrayASCIIchecksum[7] = {0};
	//uint8_t  rtcMask;

	/*****************adding ID to buffer***********************/
	CB_buffer_add_item(logBuff, inputEvent->logID);


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
	/*
	for(uint8_t i=0; i<4; i++)
	{
		if (i==0)
		{
			rtcMask = (inputEvent->RTCtimeStamp & 0xff000000)>>24;
			CB_buffer_add_item(logBuff,rtcMask);
		}
		if (i==1)
		{
			rtcMask = (inputEvent->RTCtimeStamp & 0x00ff0000)>>16;
			CB_buffer_add_item(logBuff,rtcMask);
		}
		if (i==2)
		{
			rtcMask = (inputEvent->RTCtimeStamp & 0x0000ff00)>>8;
			CB_buffer_add_item(logBuff,rtcMask);
		}
		if (i==3)
		{
			rtcMask = (inputEvent->RTCtimeStamp & 0x000000ff);
			CB_buffer_add_item(logBuff,rtcMask);
		}
	}
	*/

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


	/*for(uint8_t i=0; i<4; i++)				//adding the checksum value
	{
		uint8_t *valuePtr;
		uint8_t arrayLength[4] = {0};

		if (i==0)
		{
			rtcMask = (inputEvent->checkSum & 0xff000000)>>24;
			valuePtr = my_itoa(rtcMask,arrayLength,10);
			CB_buffer_add_item(logBuff,*valuePtr);
		}
		if (i==1)
		{
			rtcMask = (inputEvent->checkSum & 0x00ff0000)>>16;
			valuePtr = my_itoa(rtcMask,arrayLength,10);
			CB_buffer_add_item(logBuff,*valuePtr);
		}
		if (i==2)
		{
			rtcMask = (inputEvent->checkSum & 0x0000ff00)>>8;
			valuePtr = my_itoa(rtcMask,arrayLength,10);
			CB_buffer_add_item(logBuff,*valuePtr);
		}
		if (i==3)
		{
			rtcMask = (inputEvent->checkSum & 0x000000ff);
			valuePtr = my_itoa(rtcMask,arrayLength,10);
			CB_buffer_add_item(logBuff,*valuePtr);
		}
	}*/
	CB_buffer_add_item(logBuff,0x0d);
	log_flush(logBuff);
	return;
}

/*
void logger_init_fxn()
{
	logger_init->logID = LOGGER_INIT;
	logger_init->RTCtimeStamp = 0x12121212;  //replace with fxn
	logger_init->logLength = 32;		//need to change this depending on type
	logger_init->payload = "";
	logger_init->checkSum = 0x12345678;  //replace with fxn
	return;
}

void info_fxn()
{
	event->checkSum = 0x101010101;  //replace with fxn
	event->RTCtimeStamp = 0x35353535;  //replace with fxn
	event->logID = INFO;
	event->logLength = 32;		//need to change this depending on type
	event->payload = "InfoInfoInfoInfoInfo";
	return;
}

void profiling_result_fxn()
{
	event->checkSum = 0x78787878;  //replace with fxn
	event->RTCtimeStamp = 0x66666666;  //replace with fxn
	event->logID = PROFILING_RESULT;
	event->logLength = 32;		//need to change this depending on type
	event->payload = "";  //Does this need to be fxn identifier and result?
	return;
}*/
