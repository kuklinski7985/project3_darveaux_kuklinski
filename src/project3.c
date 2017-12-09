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
#ifdef PROFILEKL25Z
int32_t Alpha_count=0;  // used to count the number of alphabetical characters in the buffer
int32_t Num_count=0;	// used to count the number of numerical characters in the buffer
int32_t Punct_count=0;	// used to count the number of numerical characters in the buffer
int32_t Misc_count=0;	// used to count the number of miscellaneous characters in the buffer
int32_t error_count=0;	// used to count the number of characters outside of the ascii range

uint8_t dump_flag = 0;

void process_Data()
{
	uint8_t * nullpayloadPtr = NULL;
#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
	logOutputData(data_analysis_started_ptr, nullpayloadPtr, DATA_ANALYSIS_STARTED);
	log_item(data_analysis_started_ptr,loggerBuffer);
		}
#endif

	int i=0;
	uint8_t D;  // intermediate variable used to read bytes from circular buffer
	int buff_count = userbuff->count +1;  // get number of bytes in buffer from userbuff struct

	//reset all of the count variables to 0 every time the buffer is refilled
	Alpha_count=0;
	Num_count=0;
	Punct_count=0;
	Misc_count=0;
	error_count=0;

	for (i=0; i<buff_count;i++)
		{

			CB_buffer_remove_item(userbuff, userbuff->poppedData);  //read a byte from CB into poppedData variable

			D = *userbuff->poppedData;  //store byte of data in intermediate variable

		  if((D>=65 && D<=90) || (D>=97 && D<=122))        //check if received data is an alphabetical character in ASCII
			{
			  Alpha_count++;
			}
		  else if(D>=48 && D<=57)			//check if received data is a number in ASCII
			{
			  Num_count++;
			}
		  else if((D>=32 && D<=47) || (D>=58 && D<=64) || (D>=91 && D<=96) || (D>=123 && D<=126)) // check if received data is punctuation in ASCII

			{
			  Punct_count++;
			}
		  else if((D>=0 && D<=31) || D==127)  //check if received data is a miscellaneous character
			{
			  Misc_count++;
			}
		  else
			{
			  error_count++;  // data is outside of ascii range
			}
		}

	transmit_table(); // call function to transmit data statistics back out to terminal
}


void transmit_table()
{
	uint8_t CR = 0x0d;  			       				// carriage return ascii code
	uint8_t Alpha[] = "# of Alphabetic Characters:  ";
	uint8_t Alpha_Length = 29;
	uint8_t Num[] = "# of Numeric Characters:  ";
	uint8_t Num_Length = 26;
	uint8_t Punct[] = "# of Punctuation Characters:  ";
	uint8_t Punct_Length = 29;
	uint8_t Misc[] = "# of Miscellaneous Characters:  ";
	uint8_t Misc_Length = 32;

	uint8_t Acnt[3] ={0}; 	//define character array to hold itoa results for alpha count
	uint8_t* Alph_Add;		//define pointer to base of Acnt array

	uint8_t Alpha_asc_length =0;

	uint8_t Ncnt[3]={0};	//define character array to hold itoa results for number count
	uint8_t* Num_Add;		//define pointer to base of Ncnt array

	uint8_t Num_asc_length =0;

	uint8_t Pcnt[3]={0};	//define character array to hold itoa results for punctuation count
	uint8_t* Punc_Add;		//define pointer to base of Pcnt array

	uint8_t Punct_asc_length =0;

	uint8_t Mcnt[3]={0};	//define character array to hold itoa results for miscellanous count
	uint8_t* Misc_Add;		//define pointer to base of Mcnt array

	uint8_t Misc_asc_length =0;

	if (Alpha_count <10)
	{
		Alpha_asc_length =1;  			// count value is a single ascii character
	}

	else if (10 <=Alpha_count && Alpha_count <100)
		{
			Alpha_asc_length =2; 		//count value is 2 ascii characters
		}
	else if (Alpha_count>=100)
			{
				Alpha_asc_length =3;  	//count value is 3 ascii characters
			}

	if (Num_count <10)
		{
			Num_asc_length =1;			//count value is a single ascii character
		}

		else if (10 <=Num_count && Num_count <100)
			{
				Num_asc_length =2;		//count value is 2 ascii characters
			}
		else if (Num_count>=100)
				{
					Num_asc_length =3;	//count value is 3 ascii characters
				}

	if (Punct_count <10)
		{
			Punct_asc_length =1;		//count value is a single ascii character
		}

		else if (10 <=Punct_count && Punct_count <100)
			{
				Punct_asc_length =2;	//count value is 2 ascii characters
			}
		else if (Punct_count>=100)
				{
					Punct_asc_length =3;	//count value is 3 ascii characters
				}

	if (Misc_count <10)
		{
			Misc_asc_length =1;			//count value is a single ascii character
		}

		else if (10 <=Misc_count && Misc_count <100)
			{
				Misc_asc_length =2;		//count value is 2 ascii characters
			}
		else if (Misc_count>=100)
				{
					Misc_asc_length =3;	//count value is 3 ascii characters
				}


	//convert count values from integer to ascii in order to transmit

	Alph_Add = my_itoa(Alpha_count, Acnt, 10);
	Num_Add = my_itoa(Num_count, Ncnt, 10);
	Punc_Add = my_itoa(Punct_count, Pcnt, 10);
	Misc_Add = my_itoa(Misc_count, Mcnt, 10);

	//transmit alphabetical statistics
	//UART_send_n(Alpha, Alpha_Length);
	//UART_send_n(Alph_Add, Alpha_asc_length);
	//UART_send(&CR);  //send a carriage return

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
	logOutputData(data_alpha_count_ptr, Alph_Add, DATA_ALPHA_COUNT);
	log_item(data_alpha_count_ptr,loggerBuffer);


	//transmit numeric statistics
	//UART_send_n(Num, Num_Length);
	//UART_send_n(Num_Add, Num_asc_length);
	//UART_send(&CR);  //send a carriage return

	logOutputData(data_num_count_ptr, Num_Add, DATA_NUM_COUNT);
	log_item(data_num_count_ptr,loggerBuffer);


	//transmit punctuation statistics
	//UART_send_n(Punct, Punct_Length);
	//UART_send_n(Punc_Add, Punct_asc_length);
	//UART_send(&CR);  //send a carriage return


	logOutputData(data_punt_count_ptr, Punc_Add, DATA_PUNT_COUNT);
	log_item(data_punt_count_ptr,loggerBuffer);


	//transmit miscellaneous statistics
	//UART_send_n(Misc, Misc_Length);
	//UART_send_n(Misc_Add, Misc_asc_length);
	//UART_send(&CR);  //send a carriage return

	logOutputData(data_misc_count_ptr, Misc_Add, DATA_MISC_COUNT);
	log_item(data_misc_count_ptr,loggerBuffer);

	uint8_t * nullpayloadPtr = NULL;
	logOutputData(data_analysis_complete_ptr, nullpayloadPtr, DATA_ANALYSIS_COMPLETE);
	log_item(data_analysis_complete_ptr,loggerBuffer);
		}
#endif


}
#endif



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

  uint8_t * nullpayloadPtr = NULL;

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

	  UART_configure();                //configures the UART

	  SPI_init();
	  GPIO_nrf_init();
	  rtc_init();
	  DMA_init();
	  myTPM_init();




	NVIC_EnableIRQ(UART0_IRQn);    //enable uart0 interrupts
	NVIC_EnableIRQ(DMA0_IRQn);    //enable DMA0 interrupts
	NVIC_EnableIRQ(RTC_Seconds_IRQn);
	__enable_irq();  //enable global interrupts



	statusUART = CB_init(userbuff,bufferSizeUART);    // initialize the circular buffer
	statusLogger = CB_init(loggerBuffer,bufferSizeLogger);

#ifdef LOG_ENABLE
		if(log_flag ==1)
		{
	logOutputData(system_init_ptr, nullpayloadPtr, SYSTEM_INIT);
	log_item(system_init_ptr,loggerBuffer);
		}
#endif



	profile_All_KL25Z(10);

	profile_All_KL25Z(100);

	profile_All_KL25Z(1000);

	profile_All_KL25Z(5000);




	for(;;)
	{
		if(dump_flag == 1 )
		{
			dump_flag = 0;
			process_Data();
		}
	}

#endif
	return;
}


