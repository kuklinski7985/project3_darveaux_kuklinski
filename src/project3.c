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

uint8_t dump_flag;		//define a flag to determine when to transmit data statistics
void project3(void)
{
	__enable_irq();  //enable global interrupts
	NVIC_EnableIRQ(UART0_IRQn);    //enable urat0 interrupts
	userbuff = (CB_t*) malloc(sizeof(CB_t));  //allocate space for the circular buffer struct
	status = CB_init(userbuff,bufferSize);    // initialize the circular buffer
	UART_configure();                //configures the UART

	uint8_t CR = 0x0d;  			       				// carriage return ascii code
	uint8_t testOut[] = "***UART connection established***";
	uint8_t testOutLength =33;

	UART_send_n(testOut,testOutLength);
	UART_send(&CR);  //send a carriage return

	//turning on the counter
	uint8_t *dstPtr, *srcPtr;
	uint16_t n=16;
	uint8_t failedMessage[] = "Malloc failed!";
	uint8_t failedLength = 14;
	//uint16_t * ITOAcounterValue;
	uint32_t CNTValue;

	dstPtr = (uint8_t*)malloc((sizeof(size_t))*n);
	if (dstPtr == NULL)
	{
		UART_send_n(failedMessage,failedLength);
		UART_send(&CR);
	}

	srcPtr = (uint8_t*)malloc((sizeof(size_t))*n);
	if (srcPtr == NULL)
	{
		UART_send_n(failedMessage,failedLength);
		UART_send(&CR);
	}

	myTPM_init();  //initialize timer
	TPM0_SC |= TPM_SC_CMOD_MASK;
	my_memmove(srcPtr,dstPtr,n);
	TPM0_SC &= TPM_SC_CMOD_MASK;  // turning off counter

	CNTValue = TPM0_BASE_PTR->CNT;
	//ITOAcounterValue = my_itoa(CNTValue,ITOAcounterValue,10);
	UART_send_n(failedMessage,failedLength);




	/*for (;;)
	{

		if (dump_flag==1)  // check to see if buffer has been filled and statistics should be transmitted
			{
				dump_flag=0;
			}
	}*/
}

