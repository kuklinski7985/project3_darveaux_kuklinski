/**
* @file binaryLogger.h
* @brief functions for creating a binary logger, using log ID and payload to log data to serial output
* @author Andrew Kuklinski and Mason Darveaux
* @date 12/08/2017
**/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef binaryLogger_h_
#define binaryLogger_h_

typedef enum{
	LOGGER_INIT, GPIO_INIT,SYSTEM_INIT, SYSTEM_HALTED,INFO,WARNING, ERROR,
	PROFILING_STARTED, PROFILING_RESULT,PROFILING_COMPLETED, DATA_RECEIVED,
	DATA_ANALYSIS_STARTED, DATA_ALPHA_COUNT, DATA_NUM_COUNT, DATA_PUNT_COUNT,
	DATA_MISC_COUNT,DATA_ANALYSIS_COMPLETE,HEARTBEAT
}logger_status;

typedef struct{
	logger_status logID;
	uint32_t RTCtimeStamp;
	uint8_t logLength;
	uint32_t payload;
	uint32_t checkSum;
}binLogger_t;


/**
 *@brief UART transmit wrap function
 *
 *@param data: pointer to a sequence of bytes, length: length of bytes to log
 *
 *@return VOID
 */
void log_data (uint8_t * data, uint8_t length);

/**
 *@brief UART function that takes a string and outputs to terminal
 *
 *@param input: c-string, example "this is a test"
 *
 *@return VOID
 */
void log_string (uint8_t *input);

/**
 *@brief accepts an integer and outputs to terminal over UART
 *
 *@param integerInput: number value to output
 *
 *@return VOID
 */
void log_integer(uint32_t integerInput);

/**
 *@brief blocks until the current logger buffer is empty
 *
 *@param circBuff: buffer to be used to flush
 *
 *@return VOID
 */
void log_flush(CB_t * circBuff);

/**
 *@brief
 *
 *@param
 *
 *@return VOID
 */
void loggerEvent_init(binLogger_t * inputEvent);

void getCheckSumValue();
void getRTCValue();

#endif /*binaryLogger_h_*/
