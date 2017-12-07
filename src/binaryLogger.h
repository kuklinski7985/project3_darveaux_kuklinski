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
	uint32_t RTCtimeStamp;	//32-bit value
	uint8_t logID;			//log indicator, enum value
	uint8_t logLength;		//number of bytes in payload
	uint8_t * payload;		//data sent for output
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
CB_status log_flush(CB_t * circBuff);

/**
 *@brief calculate ones count of binLogger_t struct type
 *@brief must be run after struct initialization
 *
 *@param binLogger_t type struct
 *
 *@return checksum value, number of ones struct
 */
void log_item_KL25Z(binLogger_t * inputEvent, CB_t *loggerBuffer);


#endif /*binaryLogger_h_*/
