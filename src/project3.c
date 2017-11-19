/**
* @file project2.c
* @brief Defines the data processing portion of Project2, ECEN5816.  Uses
* UART and circular buffer constructions to track statistics of input
* character values
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "circbuff.h"
#include "uart.h"
#include "debug.h"
#include "project3.h"
#include "MKL25Z4.h"
#include "conversion.h"
#include "memory.h"
#include "timerInit.c"

CB_t * userbuff;        // define a pointer to our circular buffer structure
uint8_t bufferSize = 16;           //sets circular buffer size
CB_status status;		//define the circular buffer status structure

void project3(void)
{
	__enable_irq();  //enable global interrupts
	NVIC_EnableIRQ(UART0_IRQn);    //enable urat0 interrupts
	NVIC_EnableIRQ(TPM0_IRQn);
	userbuff = (CB_t*) malloc(sizeof(CB_t));  //allocate space for the circular buffer struct
	status = CB_init(userbuff,bufferSize);    // initialize the circular buffer
	UART_configure();                //configures the UART

	/******Messages For UART*******/
	uint8_t CR = 0x0d;  			       				// carriage return ascii code
	uint8_t testOut[] = "***UART connection established***";
	uint8_t testOutLength =33;
	uint8_t failedMessage[] = "Malloc failed!";
	uint8_t failedLength = 14;
	uint8_t ticksElap[] = "# Ticks Elapased: ";
	uint8_t ticksLength = 18;

	//uint8_t testMessage[] = "Test Message: ";
	//uint8_t testMessageLength = 14;
	//uint8_t testArray[6] = {0};
	//uint8_t *testPtr;
	/*****************************/


	uint8_t arrayTicks[6] = {0};
	uint8_t * ticksPtr;
	uint32_t valueLength = 0;
	uint16_t startVal = 0;
	uint16_t endVal = 0;
	uint16_t totalTime = 0;

	UART_send_n(testOut,testOutLength);  //Sending "UART Connection established"
	UART_send(&CR);  //send a carriage return

	//uint8_t *dstPtr, *srcPtr;
	uint8_t *srcPtr;
	uint16_t bytesMoved=5000;			//change this value for number of bytes to be moved

	/*dstPtr = (uint8_t*)malloc((sizeof(size_t))*bytesMoved);
	if (dstPtr == NULL)
	{
		UART_send_n(failedMessage,failedLength);
		UART_send(&CR);
	}*/

	/*using an array in order to utilize both the heap and the stack when
	 * number of bytes that need to be moved gets high
	 * used of the arry is for the stack*/
	uint8_t dst[bytesMoved];

	/*used malloc to utilize the heap*/
	srcPtr = (uint8_t*)malloc((sizeof(uint8_t))*bytesMoved);
	if (srcPtr == NULL)
	{
		UART_send_n(failedMessage,failedLength);
		UART_send(&CR);
	}

	myTPM_init();
	TPM0->CNT = TPM_CNT_COUNT(0x0);
	startVal = TPM0->CNT;

	TPM0->SC |= TPM_SC_CMOD(0b01);
	//set value of timer
	my_memmove(dst,srcPtr,bytesMoved);
	//turning off counter
	TPM0->SC |= TPM_SC_CMOD(0b00);
	//set end value of counter
	endVal = TPM0->CNT;
	totalTime = endVal - startVal;

	UART_send_n(ticksElap,ticksLength);


	if (totalTime <10)
	{
		valueLength =1;  			// count value is a single ascii character
	}
	else if (10 <=totalTime && totalTime <100)
	{
		valueLength =2; 		//count value is 2 ascii characters
	}
	else if (100 <=totalTime && totalTime <999)
	{
		valueLength =3;  	//count value is 3 ascii characters
	}
	else if (1000 <=totalTime && totalTime <9999)
	{
		valueLength =4; 		//count value is 4 ascii characters
	}
	else if (10000 <=totalTime && totalTime <99999)
	{
		valueLength =5; 		//count value is 5 ascii characters
	}
	else if (100000 <=totalTime && totalTime <999999)
	{
		valueLength =6; 		//count value is 6 ascii characters
	}

	ticksPtr = my_itoa(totalTime, arrayTicks, 10);

	UART_send_n(ticksPtr, valueLength);
	UART_send(&CR);  //send a carriage return

	//free(dstPtr);
	free(srcPtr);
	return;
}


