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

/*#define LOG_ITEM((binLogger_t * data), (CB_t * buffer)) ({#ifdef PROFILEKL25Z \
		log_item(data,data->logLength);\
		log_flush(buffer);\
		#endif});
*/
typedef enum{
	LOGGER_INIT, GPIO_INIT,SYSTEM_INIT, SYSTEM_HALTED,INFO,WARNING, ERROR,
	PROFILING_STARTED, PROFILING_RESULT,PROFILING_COMPLETED, DATA_RECEIVED,
	DATA_ANALYSIS_STARTED, DATA_ALPHA_COUNT, DATA_NUM_COUNT, DATA_PUNT_COUNT,
	DATA_MISC_COUNT,DATA_ANALYSIS_COMPLETE,HEARTBEAT
}logger_status;

typedef struct{
	uint32_t checkSum;		//using 1's counter
	logger_status logID;	//log indicator, enum value
	uint32_t RTCtimeStamp;	//32-bit value
	uint8_t logLength;		//number of bytes in payload
	char * payload;		//data sent for output
}binLogger_t;

binLogger_t * logger_init0;
binLogger_t * gpio_init;
binLogger_t * system_init;
binLogger_t * system_halted;
binLogger_t * info;
binLogger_t * warning;
binLogger_t * error;
binLogger_t * profiling_started;
binLogger_t * profiling_result;
binLogger_t * profiling_completed;
binLogger_t * data_received;
binLogger_t * data_analysis_started;
binLogger_t * data_alpha_count;
binLogger_t * data_num_count;
binLogger_t * data_punt_count;
binLogger_t * data_misc_count;
binLogger_t * data_analysis_complete;
binLogger_t * heartbeat;

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
void loggerEvent_init(binLogger_t * inputEvent,
		logger_status logEvent, uint8_t inputlogLength, char* inputpayload);

/**
 *@brief calculate ones count of binLogger_t struct type
 *@brief must be run after struct initialization
 *
 *@param binLogger_t type struct
 *
 *@return checksum value, number of ones struct
 */
uint32_t checksumOnesCount(binLogger_t inputEvent);

/**
 *@brief calculate ones count of binLogger_t struct type
 *@brief must be run after struct initialization
 *
 *@param binLogger_t type struct
 *
 *@return checksum value, number of ones struct
 */
void logger_init_all();

/**
 *@brief calculate ones count of binLogger_t struct type
 *@brief must be run after struct initialization
 *
 *@param binLogger_t type struct
 *
 *@return checksum value, number of ones struct
 */
void log_item(binLogger_t * logStruct);

#endif /*binaryLogger_h_*/
