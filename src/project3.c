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
#include "timerInit.h"
#include "profilingFxn.h"

CB_t * userbuff;        // define a pointer to our circular buffer structure
uint8_t bufferSize = 16;           //sets circular buffer size
CB_status status;		//define the circular buffer status structure

void project3(void)
{
#ifdef PROFILEBBB
	profile_All_BBB(10);
	profile_All_BBB(100);
	profile_All_BBB(1000);
	profile_All_BBB(5000);
#endif


#ifdef PROFILEKL25Z
	__enable_irq();  //enable global interrupts
	NVIC_EnableIRQ(UART0_IRQn);    //enable uart0 interrupts
	NVIC_EnableIRQ(TPM0_IRQn);
	userbuff = (CB_t*) malloc(sizeof(CB_t));  //allocate space for the circular buffer struct
	status = CB_init(userbuff,bufferSize);    // initialize the circular buffer
	UART_configure();                //configures the UART

	/******Messages For UART*******/
	uint8_t CR = 0x0d;  			       				// carriage return ascii code
	uint8_t testOut[]= "***UART connection established***";
	uint8_t testOutLength =33;
	/*****************************/

	UART_send_n(testOut,testOutLength);  //Sending "UART Connection established"
	UART_send(&CR);

	myTPM_init();
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
	profile_All_KL25Z(5000);

#endif
	return;
}


