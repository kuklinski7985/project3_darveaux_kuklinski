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
 #define LOG_ITEM(data,buffer) log_item_KL25Z(data,buffer)
#endif

#if defined (PROFILEHOST) || defined (PROFILEBBB)
 #define LOG_ITEM(data,buffer) log_item_BBBHOST(data,buffer)
#endif

#include "conversion.h"
#include "memory.h"
#include "profilingFxn.h"
#include "binaryLogger.h"

CB_t * userbuff;        // define a pointer to our circular buffer structure, used in the UART
uint8_t bufferSizeUART = 16;           //sets circular buffer size
CB_status statusUART;		//define the circular buffer status structure
CB_status statusLogger;		//status enum for logger buffer

CB_t * loggerBuffer;		//circular buffer for logger output
uint8_t bufferSizeLogger = 128;  //should be big enough to contain an entire binlogger_t struct


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

/*creating logging event structures for all events*/
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

	/******Messages For UART*******/
	//uint8_t CR = 0x0d;  			  // carriage return ascii code
	//uint8_t testOut[]= "***UART connection established***";
	//uint8_t testOutLength =33;
	/*****************************/

	//UART_send_n(testOut,testOutLength);  //Sending "UART Connection established"
	//UART_send(&CR);

	/*myTPM_init();
	uint8_t tenBytes[] = "********testing 10 bytes*********";
	UART_send_n(tenBytes,testOutLength);
	UART_send(&CR);
	profile_All_KL25Z(10);

	uint8_t hundredBytes[] = "********testing 100 bytes********";
	UART_send_n(hundredBytes,testOutLength);
	UART_send(&CR);
	profile_All_KL25Z(100);

	uint8_t thousandBytes[] = "********testing 1000 bytes*******";
	UART_send_n(thousandBytes,testOutLength);
	UART_send(&CR);
	profile_All_KL25Z(1000);

	uint8_t fiveKBytes[] = "********testing 5000 bytes*******";
	UART_send_n(fiveKBytes,testOutLength);
	UART_send(&CR);
	profile_All_KL25Z(5000);*/

	uint8_t payloadStr[] =  "new test";
	logger_init_ptr->logID = DATA_ANALYSIS_COMPLETE;
	logger_init_ptr->payload = payloadStr;
	logger_init_ptr->logLength = strlen((char*)logger_init_ptr->payload);
	logger_init_ptr->RTCtimeStamp = 60606060;  //need a function
	logger_init_ptr->checkSum = 45689;  //need function

	LOG_ITEM(logger_init_ptr,loggerBuffer);


	for(;;);

#endif
	return;
}


