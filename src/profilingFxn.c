/**
* @file profilingFxn.c
* @brief defines functions to profile standard library versions, non-DMA,
* non-DMA optimized using -o3 and DMA versions of memmove and memzero/memset.
* Setting compile time flags will choose the target specific code
* @author Andrew Kuklinski and Mason Darveaux
* @date 12/08/2017
**/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "profilingFxn.h"

#ifdef PROFILEKL25Z
#include "MKL25Z4.h"
#endif

#include "uart.h"
#include "memory.h"
#include "conversion.h"
#include <time.h>
#include <sys/time.h>

#include "binaryLogger.h"
#include "circbuff.h"
#include "loggerQueue.h"
#include "project3.h"


#if defined (PROFILEHOST) || defined (PROFILEBBB)
void profile_All_BBB(uint16_t bytesMoved)  //code for host and BBB
{
	/*information on gettimeofday usage was obtained from
	   *man7.org/linux/man-pages/man2/gettimeofday.2.html
	   *
	   *contains a struct timeval with members time_t tv_sec and
	   *suseconds_t tv_usec.
	   *gettimeofday(struct timeval *tv, struct timezone *tz)
	   *#INCLUDE <time.h> and <sys/time.h>
	*/

	  uint8_t dst[bytesMoved];  //creates destination space on stack

	  uint8_t *srcPtr;
	  srcPtr = (uint8_t*) malloc((sizeof(size_t))*bytesMoved);  //creates source space on heap
	  if (srcPtr == NULL)
	  {
	    printf("src malloc failed\n");
	  }

	  struct timeval start, end;
	  uint16_t totaltime;
	  
        struct timeval time;
        gettimeofday(&time, NULL);
	printf("timestamp: %ld\n\n",time.tv_sec);
	  for(uint8_t i=0; i<6; i++)
	  {
		  totaltime = 0;
		  if(i == 0)
		  {
			  gettimeofday(&start,NULL);  //uses time.h functions to get seconds and usec
			  memmove(dst,srcPtr,bytesMoved);
			  gettimeofday(&end,NULL);
			  totaltime = (end.tv_sec-start.tv_sec) +
					  (end.tv_usec - start.tv_usec);
			  printf("memmove        | total time: %d\n",totaltime);
		  }
		  if(i == 1)
		  {
		    	  gettimeofday(&start,NULL);
			  my_memmove(dst,srcPtr,bytesMoved);
			  gettimeofday(&end,NULL);
			  totaltime = (end.tv_sec-start.tv_sec) +
					  (end.tv_usec - start.tv_usec);
			  printf("my_memmove     | total time: %d\n",totaltime);
		  }

		  if(i == 3)
		  {
		      gettimeofday(&start,NULL);
		      memset(srcPtr,'z',bytesMoved);	//function to be tested
			  gettimeofday(&end,NULL);
			  totaltime = (end.tv_sec-start.tv_sec) +
					  (end.tv_usec - start.tv_usec);
			  printf("memset         | total time: %d\n",totaltime);

		  }
		  if(i == 4)
		  {
			  gettimeofday(&start,NULL);
              my_memset(srcPtr,bytesMoved,'z');
			  gettimeofday(&end,NULL);
			  totaltime = (end.tv_sec-start.tv_sec) +
					  (end.tv_usec - start.tv_usec);
			  printf("my_memset      | total time: %d\n",totaltime);

		  }

	  }

	  free(srcPtr);
	  return;

}
#endif

#ifdef PROFILEKL25Z  //code for KL25Z
void profile_All_KL25Z(uint16_t bytesMoved)
{

	uint8_t bytes[2] = {0};
	bytes[0] = bytesMoved && 0x00FF;
	bytes[1] = (bytesMoved && 0xFF00)>>8;
	uint8_t infoBytesMoved[] = "bytes to be moved (next line for number): ";

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(info_ptr, infoBytesMoved, INFO);
			log_item(info_ptr,loggerBuffer);
		}
#endif

	uint16_t bytesMovedpayload[6] = {0};
	uint8_t * bytesPayloadPtr;

	bytesPayloadPtr = my_itoa(bytesMoved, bytes, 10);			//converts values to ASCII
#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
	logOutputData(info_ptr, bytesPayloadPtr, INFO);
	log_item(info_ptr,loggerBuffer);
		}
#endif

	uint8_t failedMessage[] = "Malloc failed!";
	uint8_t failedLength = 14;
	uint8_t arrayTicks[6] = {0};
	uint8_t * ticksPtr;
	uint32_t valueLength = 0;
	uint16_t startVal = 0;
	uint16_t endVal = 0;
	uint16_t totalTime = 0;
	uint8_t CR = 0x0d;

	uint8_t profilStartpayload[] = "profiling analysis started";
	uint8_t profilingResultpayload1[] = "profile result memmove next line";
	uint8_t profilingResultpayload2[] = "profile result my_memmove next line";
	uint8_t profilingResultpayload3[] = "profile result memmove_dma next line";
	uint8_t profilingResultpayload4[] = "profile result memset next line";
	uint8_t profilingResultpayload5[] = "profile result my_memset next line";
	uint8_t profilingResultpayload6[] = "profile result memset_dma next line";
	uint8_t profilingResultpayload7[] = "profiling analysis completed";
	uint8_t *profileResultPtr;

	/***************************************************************
	 * use bytesMoved to change the the number of bytes to be tested
	 **************************************************************/
	uint8_t messageOutLength = 34;

	/*using an array in order to utilize both the heap and the stack when
	 * number of bytes that need to be moved gets high
	 * use of the array is for the stack*/
	uint8_t dst[bytesMoved];

	/*used malloc to utilize the heap*/
	uint8_t *srcPtr;

	for(int i=0;i<6;i++)
	{
		srcPtr = (uint8_t*)malloc((sizeof(uint8_t))*bytesMoved);
		if (srcPtr == NULL)
		{
#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			log_data(failedMessage,failedLength);
			log_data_single(&CR);
		}
#endif
			return;
		}

		if(i == 0)
		{

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_started_ptr, profilStartpayload, PROFILING_STARTED);
			log_item(profiling_started_ptr,loggerBuffer);
		}
#endif

			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			uint8_t payloadArray[] = "profiling started";
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			memmove(dst,srcPtr,bytesMoved);	//function to be tested

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_result_ptr, profilingResultpayload1, PROFILING_RESULT);
			log_item(profiling_result_ptr,loggerBuffer);
		}
#endif


			//uint8_t messageOut[] = "Profile memmove        | # ticks: ";
			//UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

		if(i ==1)
		{
#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_started_ptr, profilStartpayload, PROFILING_STARTED);
			log_item(profiling_started_ptr,loggerBuffer);
		}
#endif

			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			my_memmove(dst,srcPtr,bytesMoved);	//function to be tested

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_result_ptr, profilingResultpayload2, PROFILING_RESULT);
			log_item(profiling_result_ptr,loggerBuffer);
		}
#endif

			//uint8_t messageOut[] = "Profile my_memmove     | # ticks: ";
			//UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

		if(i==2)
		{

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_started_ptr, profilStartpayload, PROFILING_STARTED);
			log_item(profiling_started_ptr,loggerBuffer);
		}
#endif

			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			memmove_dma(srcPtr,dst,bytesMoved);

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_result_ptr, profilingResultpayload3, PROFILING_RESULT);
			log_item(profiling_result_ptr,loggerBuffer);
		}
#endif

			//uint8_t messageOut[] = "Profile DMA_my_memmove | # ticks: ";
			//UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

		if(i ==3)
		{

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_started_ptr, profilStartpayload, PROFILING_STARTED);
			log_item(profiling_started_ptr,loggerBuffer);
		}
#endif

			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			memset(srcPtr,'z',bytesMoved);	//function to be tested

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_result_ptr, profilingResultpayload4, PROFILING_RESULT);
			log_item(profiling_result_ptr,loggerBuffer);
		}
#endif


			//uint8_t messageOut[] = "Profile memset         | # ticks: ";
			//UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

		if(i==4)
		{
#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_started_ptr, profilStartpayload, PROFILING_STARTED);
			log_item(profiling_started_ptr,loggerBuffer);
		}
#endif

			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			my_memset(srcPtr,bytesMoved,'z');	//function to be tested

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_result_ptr, profilingResultpayload5, PROFILING_RESULT);
			log_item(profiling_result_ptr,loggerBuffer);
		}
#endif

			//uint8_t messageOut[] = "Profile my_memset      | # ticks: ";
			//UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

	/*
		if(i==5)
		{

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_started_ptr, profilStartpayload, PROFILING_STARTED);
			log_item(profiling_started_ptr,loggerBuffer);
		}
#endif
			uint8_t
			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			//PUT DMA my_memset FUNCTION CALL HERE

			memset_dma(srcPtr,bytesMoved,'z',dst);

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
			logOutputData(profiling_result_ptr, profilingResultpayload1, PROFILING_RESULT);
			log_item(profiling_result_ptr,loggerBuffer);
		}
#endif

			//uint8_t messageOut[] = "Profile DMA_my_memset  | # ticks: ";
			//UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

		*/

		ticksPtr = my_itoa(totalTime, arrayTicks, 10);			//converts values to ASCII
#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
		logOutputData(profiling_result_ptr, ticksPtr, PROFILING_RESULT);
		log_item(profiling_result_ptr,loggerBuffer);
		}
#endif



		//valueLength = getValueLength(totalTime);
		//UART_send_n(ticksPtr, valueLength);						//sends message
		//UART_send(&CR);  										//send a carriage return
	}

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
	logOutputData(profiling_completed_ptr, profilingResultpayload7, PROFILING_COMPLETED);
	log_item(profiling_completed_ptr,loggerBuffer);
		}
#endif
	return;
}

uint32_t getValueLength(uint32_t length)
{
	uint32_t valueLength = 0;
	/*if (length <10)
	{
		valueLength =1;  			// count value is a single ascii character
	}
	else if (10 <=length && length <100)
	{
		valueLength =2; 		//count value is 2 ascii characters
	}
	else if (100 <=length && length <1000)
	{
		valueLength =3;  	//count value is 3 ascii characters
	}
	else if (1000 <=length && length <10000)
	{
		valueLength =4; 		//count value is 4 ascii characters
	}
	else if (10000 <=length && length <100000)
	{
		valueLength =5; 		//count value is 5 ascii characters
	}
	else if (100000 <=length && length <1000000)
	{
		valueLength =6; 		//count value is 6 ascii characters
	}*/
	while (length != 0)
	{
		valueLength++;
		length /= 10;
	}

	return valueLength;
}
#endif

