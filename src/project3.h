
/**
* @file project3.h
* @brief Contains function prototypes for the implention of project3
* @author Andrew Kuklinski and Mason Darveaux
* @date 12/08/2017
**/

#include <stdio.h>
#include <stdint.h>

#ifndef project3_h_
#define project3_h_

#include "circbuff.h"
#include "binaryLogger.h"

extern binLogger_t logger_init;
extern binLogger_t gpio_init;
extern binLogger_t system_init;
extern binLogger_t system_halted;
extern binLogger_t info;
extern binLogger_t warning;
extern binLogger_t error;
extern binLogger_t profiling_started;
extern binLogger_t profiling_result;
extern binLogger_t profiling_completed;
extern binLogger_t data_received;
extern binLogger_t data_analysis_started;
extern binLogger_t data_alpha_count;
extern binLogger_t data_num_count;
extern binLogger_t data_punt_count;
extern binLogger_t data_misc_count;
extern binLogger_t data_analysis_complete;
extern binLogger_t heartbeat;

extern binLogger_t * logger_init_ptr;
extern binLogger_t * gpio_init_ptr;
extern binLogger_t * system_init_ptr;
extern binLogger_t * system_halted_ptr;
extern binLogger_t * info_ptr;
extern binLogger_t * warning_ptr;
extern binLogger_t * error_ptr;
extern binLogger_t * profiling_started_ptr;
extern binLogger_t * profiling_result_ptr;
extern binLogger_t * profiling_completed_ptr;
extern binLogger_t * data_received_ptr;
extern binLogger_t * data_analysis_started_ptr;
extern binLogger_t * data_alpha_count_ptr;
extern binLogger_t * data_num_count_ptr;
extern binLogger_t * data_punt_count_ptr;
extern binLogger_t * data_misc_count_ptr;
extern binLogger_t * data_analysis_complete_ptr;
extern binLogger_t * heartbeat_ptr;
extern CB_t * loggerBuffer;		//circular buffer for logger output

/**
 *@brief contains all functions and operations necessary to solve project3 assignments
 *
 *@param none
 *
 *@return VOID
 */
void project3(void);

/**
 *@brief processes received data
 *
 *@param none
 *
 *@return VOID
 */

void process_Data();

/**
 *@brief transmits data statistics to terminal
 *
 *@param none
 *
 *@return VOID
 */

void transmit_table();

#endif /*__project3_h__*/
