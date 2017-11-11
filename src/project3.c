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


int32_t Alpha_count=0;  // used to count the number of alphabetical characters in the buffer
int32_t Num_count=0;	// used to count the number of numerical characters in the buffer
int32_t Punct_count=0;	// used to count the number of numerical characters in the buffer
int32_t Misc_count=0;	// used to count the number of miscellaneous characters in the buffer
int32_t error_count=0;	// used to count the number of characters outside of the ascii range

CB_t * userbuff;        // define a pointer to our circular buffer structure
//uint8_t size = 16;           //sets circular buffer size
CB_status status;		//define the circular buffer status structure

uint8_t dump_flag;		//define a flag to determine when to transmit data statistics
void project3(void)
{
	__enable_irq();  //enable global interrupts
	NVIC_EnableIRQ(UART0_IRQn);    //enable urat0 interrupts
	userbuff = (CB_t*) malloc(sizeof(CB_t));  //allocate space for the circular buffer struct
	status = CB_init(userbuff,16);    // initialize the circular buffer
	UART_configure();                //configures the UART
	for (;;)
	{
		if (dump_flag==1)  // check to see if buffer has been filled and statistics should be transmitted
			{
			dump_flag=0;
			process_Data(); // function to analyze received data
			}
	}
}

void process_Data()
{
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
	UART_send_n(Alpha, Alpha_Length);
	UART_send_n(Alph_Add, Alpha_asc_length);
	UART_send(&CR);  //send a carriage return

	//transmit numeric statistics
	UART_send_n(Num, Num_Length);
	UART_send_n(Num_Add, Num_asc_length);
	UART_send(&CR);  //send a carriage return

	//transmit punctuation statistics
	UART_send_n(Punct, Punct_Length);
	UART_send_n(Punc_Add, Punct_asc_length);
	UART_send(&CR);  //send a carriage return

	//transmit miscellaneous statistics
	UART_send_n(Misc, Misc_Length);
	UART_send_n(Misc_Add, Misc_asc_length);
	UART_send(&CR);  //send a carriage return
}

