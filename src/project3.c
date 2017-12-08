/**
* @file project3.c
* @brief used for project3 assignment in ECEN5813.  Project includes
* profiling self written and standard library versions of functions including
* DMA transfers, circular buffer enhancements, SPI driver interface for a Nordic
* wireless transceiver, and binary logger.
*
* Code can be used for both BeagleBone Black, Linux host machine, and KL25Z. Use
* appropriate compile time flags for target processors.
*
* @author Andrew Kuklinski and Mason Darveaux
* @date 12/08/2017
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "circbuff.h"
#include "debug.h"
#include "project3.h"

#ifdef PROFILEKL25Z
 #include "MKL25Z4.h"
 #include "uart.h"
 #include "timerInit.h"
#endif

#include "conversion.h"
#include "memory.h"
#include "profilingFxn.h"
#include "binaryLogger.h"
#include "loggerQueue.h"
#include "SPI.h"
#include "gpio.h"
#include "rtc.h"



CB_t * userbuff;        // define a pointer to our circular buffer structure, used in the UART
uint8_t bufferSizeUART = 16;           //sets circular buffer size
CB_status statusUART;		//define the circular buffer status structure

CB_t * loggerBuffer;		//circular buffer for logger output
uint8_t bufferSizeLogger = 128;  //should be big enough to contain an entire binlogger_t struct
CB_status statusLogger;		//status enum for logger buffer

/*creating logging event structures and pointers for all events*/
/*
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

extern binLogger_t * logger_init_ptr = &logger_init;
extern binLogger_t * gpio_init_ptr = &gpio_init;
extern binLogger_t * system_init_ptr = &system_init;
extern binLogger_t * system_halted_ptr = &system_halted;
extern binLogger_t * info_ptr = &info;
extern binLogger_t * warning_ptr = &warning;
extern binLogger_t * error_ptr = &error;
extern binLogger_t * profiling_started_ptr = &profiling_started;
extern binLogger_t * profiling_result_ptr = &profiling_result;
extern binLogger_t * profiling_completed_ptr = &profiling_completed;
extern binLogger_t * data_received_ptr = &data_received;
extern binLogger_t * data_analysis_started_ptr = &data_analysis_started;
extern binLogger_t * data_alpha_count_ptr = &data_alpha_count ;
extern binLogger_t * data_num_count_ptr = &data_num_count;
extern binLogger_t * data_punt_count_ptr = &data_punt_count;
extern binLogger_t * data_misc_count_ptr = &data_misc_count;
extern binLogger_t * data_analysis_complete_ptr = &data_analysis_complete;
extern binLogger_t * heartbeat_ptr = &heartbeat;
*/

void project3(void)
{
/* profile functions for host and BeagleBone Black profiling*/
#if defined (PROFILEHOST) || defined (PROFILEBBB)
  printf("****Profile for 10 Bytes****\n");
  profile_All_BBB(10);
  
  printf("***Profile for 100 Bytes****\n");
  profile_All_BBB(100);
  
  printf("***Profile for 1000 Bytes***\n");
  profile_All_BBB(1000);
  
  printf("***Profile for 5000 Bytes***\n");
  profile_All_BBB(5000);
#endif

  /*Code specific to KL25Z. See notes in Main for compile time flag and
   * timer setup using IDE*/
#ifdef PROFILEKL25Z

  	  SPI_init();
  	  GPIO_nrf_init();
  	  rtc_init();
	__enable_irq();  //enable global interrupts
	NVIC_EnableIRQ(UART0_IRQn);    //enable uart0 interrupts
	NVIC_EnableIRQ(TPM0_IRQn);
	userbuff = (CB_t*) malloc(sizeof(CB_t));  //allocate space for the circular buffer struct

	if(userbuff == NULL)
	{
		return;
	}

	loggerBuffer = (CB_t*)malloc(sizeof(CB_t));
	if(loggerBuffer == NULL)
	{
		return;
	}

	statusUART = CB_init(userbuff,bufferSizeUART);    // initialize the circular buffer
	statusLogger = CB_init(loggerBuffer,bufferSizeLogger);
	UART_configure();                //configures the UART

	DMA_init();

	myTPM_init();

	profile_All_KL25Z(10);

	profile_All_KL25Z(100);

	profile_All_KL25Z(1000);

	profile_All_KL25Z(5000);

	/*
	//uint8_t testVar = 't';

	//uint8_t payloadStr[] = {0};
	uint8_t payloadStr[] = "testing, something longer than 1 digit";
	//payloadStr[0] = testVar;
	//my_itoa(testVar, &payloadStr[0], 10);
	logOutputData(logger_init_ptr, payloadStr, PROFILING_STARTED);
	//LOG_ITEM(logger_init_ptr,loggerBuffer);
	LOG_ITEM(logger_init_ptr,loggerBuffer);*/


	for(;;);

#endif
	return;
}


