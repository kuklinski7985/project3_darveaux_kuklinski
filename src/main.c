
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "circbuff.h"
#include "uart.h"
#include "debug.h"

#ifdef PROFILEKL25Z
#include "MKL25Z4.h"
#endif

#include "project3.h"
#include "binaryLogger.h"
#include "loggerQueue.h"



extern CB_t * userbuff;        //define a pointer to our circular buffer structure
//extern uint8_t size;         //sets circular buffer size
extern CB_status status;       //define the circular buffer status structure

extern uint8_t dump_flag;      //define a flag to determine when to transmit data statistics

binLogger_t logger_init;
binLogger_t gpio_init;
binLogger_t system_init;
binLogger_t system_halted;
binLogger_t info;
binLogger_t warning;
binLogger_t error;
binLogger_t profiling_started;
binLogger_t profiling_result;
binLogger_t profiling_completed;
binLogger_t data_received;
binLogger_t data_analysis_started;
binLogger_t data_alpha_count;
binLogger_t data_num_count;
binLogger_t data_punt_count;
binLogger_t data_misc_count;
binLogger_t data_analysis_complete;
binLogger_t heartbeat;

binLogger_t * logger_init_ptr = &logger_init;
binLogger_t * gpio_init_ptr = &gpio_init;
binLogger_t * system_init_ptr = &system_init;
binLogger_t * system_halted_ptr = &system_halted;
binLogger_t * info_ptr = &info;
binLogger_t * warning_ptr = &warning;
binLogger_t * error_ptr = &error;
binLogger_t * profiling_started_ptr = &profiling_started;
binLogger_t * profiling_result_ptr = &profiling_result;
binLogger_t * profiling_completed_ptr = &profiling_completed;
binLogger_t * data_received_ptr = &data_received;
binLogger_t * data_analysis_started_ptr = &data_analysis_started;
binLogger_t * data_alpha_count_ptr = &data_alpha_count ;
binLogger_t * data_num_count_ptr = &data_num_count;
binLogger_t * data_punt_count_ptr = &data_punt_count;
binLogger_t * data_misc_count_ptr = &data_misc_count;
binLogger_t * data_analysis_complete_ptr = &data_analysis_complete;
binLogger_t * heartbeat_ptr = &heartbeat;

CB_t * loggerBuffer;



/* 1. when using kinetis IDE, you will have to add the -DPROJECT3 and -DPROFILEKL25Z tags to the complier
 * 2. you will also have to increase the size of the heap to as least 0x3500
 * 	   a. -Xlinker --defsym=__heap_size__=0x3500
 * 3. verify that system_MKL25Z4.h has the following to make UART operate:
 *     a. #define CLOCK_SETUP 1
 *     b. value for SYSTEM_MCG_C5_VALUE 0x41u and not 0x01u
 *     c. #define SYSTEM_SIM_SOPT2_VALUE should equal 0x04010000U not 0x01010000U
 * */

int main()
{

  #ifdef PROJECT3
 project3();
 #endif

  return 0;
}




