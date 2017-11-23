/**
* @file uart.c
* @brief defines uart operation and provides functions for uart operation and initialization
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "profilingFxn.h"
#include "MKL25Z4.h"
#include "uart.h"
#include "memory.h"
#include "conversion.h"
#include <time.h>
#include <sys/time.h>

void profile_All_BBB(uint16_t bytesMoved)
{
	/*information on gettimeofday usage was obtained from
	   *man7.org/linux/man-pages/man2/gettimeofday.2.html
	   *
	   *contains a struct timeval with members time_t tv_sec and
	   *suseconds_t tv_usec.
	   *gettimeofday(struct timeval *tv, struct timezone *tz)
	   *#INCLUDE <time.h> and <sys/time.h>
	*/
	  uint8_t dst[bytesMoved];

	  uint8_t *srcPtr;
	  srcPtr = (uint8_t*) malloc((sizeof(size_t))*bytesMoved);
	  if (srcPtr == NULL)
	  {
	    printf("src malloc failed\n");
	  }

	  struct timeval start, end;
	  uint16_t totaltime;

	  for(uint8_t i=0; i<6; i++)
	  {
		  totaltime = 0;
		  if(i == 0)
		  {
			  gettimeofday(&start,NULL);
			  my_memmove(dst,srcPtr,bytesMoved);
			  gettimeofday(&end,NULL);
			  totaltime = (end.tv_sec-start.tv_sec) +
					  (end.tv_usec - start.tv_usec);
			  printf("my_memmove | total time: %d\n",totaltime);
		  }
		  if(i == 1)
		  {
			  printf("i = 1\n");
		  }
		  if(i == 2)
		  {
			  printf("i = 2\n");
		  }
		  if(i == 3)
		  {
			  printf("i = 3\n");
		  }
		  if(i == 4)
		  {
			  printf("i = 4\n");
		  }
		  if(i == 5)
		  {
			  printf("i = 5\n");
		  }
	  }

	  free(srcPtr);
	  return;

}

void profile_All_KL25Z(uint16_t bytesMoved)
{

	uint8_t failedMessage[] = "Malloc failed!";
	uint8_t failedLength = 14;
	uint8_t arrayTicks[5] = {0};
	uint8_t * ticksPtr;
	uint32_t valueLength = 0;
	uint16_t startVal = 0;
	uint16_t endVal = 0;
	uint16_t totalTime = 0;
	uint8_t CR = 0x0d;

	/***************************************************************
	 * use bytesMoved to change the the number of bytes to be tested
	 **************************************************************/
	uint8_t messageOutLength = 34;

	/*using an array in order to utilize both the heap and the stack when
	 * number of bytes that need to be moved gets high
	 * used of the array is for the stack*/
	uint8_t dst[bytesMoved];

	/*used malloc to utilize the heap*/
	uint8_t *srcPtr;

	for(int i=0;i<6;i++)
	{
		srcPtr = (uint8_t*)malloc((sizeof(uint8_t))*bytesMoved);
		if (srcPtr == NULL)
		{
			UART_send_n(failedMessage,failedLength);
			UART_send(&CR);
		}

		if(i == 0)
		{
			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			memmove(dst,srcPtr,bytesMoved);	//function to be tested

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time
			uint8_t messageOut[] = "Profile memmove        | # ticks: ";
			UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

		if(i ==1)
		{
			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			my_memmove(dst,srcPtr,bytesMoved);	//function to be tested

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time
			uint8_t messageOut[] = "Profile my_memmove     | # ticks: ";
			UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

		if(i==2)
		{
			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			//PUT DMA my_memmove FUNCTION CALL HERE

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time
			uint8_t messageOut[] = "Profile DMA_my_memmove | # ticks: ";
			UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

		if(i ==3)
		{
			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			memset(srcPtr,'z',bytesMoved);	//function to be tested

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time
			uint8_t messageOut[] = "Profile memset         | # ticks: ";
			UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

		if(i==4)
		{
			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			my_memset(srcPtr,bytesMoved,'z');	//function to be tested

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time
			uint8_t messageOut[] = "Profile my_memset      | # ticks: ";
			UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}

		if(i==5)
		{
			TPM0->CNT = TPM_CNT_COUNT(0x0);		//initialize counter register to zero
			startVal = TPM0->CNT;				//read start value of counter
			TPM0->SC |= TPM_SC_CMOD(0b01);		//counter increments on every clock cycle and turns on

			//PUT DMA my_memset FUNCTION CALL HERE

			TPM0->SC |= TPM_SC_CMOD(0b00);		//turns off the counter
			endVal = TPM0->CNT;					//reading the counter value after running test fxn
			totalTime = endVal - startVal;		//getting execution time
			uint8_t messageOut[] = "Profile DMA_my_memset  | # ticks: ";
			UART_send_n(messageOut,messageOutLength);
			free(srcPtr);
		}
		ticksPtr = my_itoa(totalTime, arrayTicks, 10);			//converts values to ASCII
		valueLength = getValueLength(totalTime);
		UART_send_n(ticksPtr, valueLength);						//sends message
		UART_send(&CR);  										//send a carriage return
	}
	return;
}

uint32_t getValueLength(uint16_t length)
{
	uint32_t valueLength = 0;
	if (length <10)
	{
		valueLength =1;  			// count value is a single ascii character
	}
	else if (10 <=length && length <100)
	{
		valueLength =2; 		//count value is 2 ascii characters
	}
	else if (100 <=length && length <999)
	{
		valueLength =3;  	//count value is 3 ascii characters
	}
	else if (1000 <=length && length <9999)
	{
		valueLength =4; 		//count value is 4 ascii characters
	}
	else if (10000 <=length && length <99999)
	{
		valueLength =5; 		//count value is 5 ascii characters
	}
	else if (100000 <=length && length <999999)
	{
		valueLength =6; 		//count value is 6 ascii characters
	}

	return valueLength;
}

