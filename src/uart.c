/**
* @file uart.c
* @brief defines uart operation and provides functions for uart operation and initialization
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "uart.h"
#include "MKL25Z4.h"
#include "circbuff.h"

CB_t * userbuff;
uint8_t size;               //sets circular buffer size
CB_status status;
uint8_t dump_flag;

void UART_configure()
{


uint16_t sbr = 0;  // variable used to set the SBR field in the UART baud rate register in order to
			// configure the baud rate

uint16_t osr=8;  // set oversampling ratio to option 3

#ifdef U0

SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; //Enable clock gate for ports to enable

SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;  //enable clock signal to UART0 module

// ISF=0, MUX=2
PORTA->PCR[1] |= PORT_PCR_MUX(0x02); //set PTA1 to UART0_RX

//ISF=0 MUX=2
PORTA->PCR[2] |= PORT_PCR_MUX(0x02); //set PTA2 to UART0_TX



UART0->C2 &= ~(UART_C2_TE_MASK); //disables transmitter for UART 0
UART0->C2 &= ~(UART_C2_RE_MASK); //disables receiver for UART 0
UART0->C1 = 0x00U; //sets UART C1 register to default, 8 bit packets, no parity

UART0->C3 = 0x00U; //set C3

UART0->S2 = 0x00U; // set S2

SIM->SOPT2 |= (SIM_SOPT2_PLLFLLSEL_MASK | SIM_SOPT2_UART0SRC(1));  //set clock source to be from PLL


sbr= BUSCLK/((osr+1)*BAUD); // sets 13 bit SBR value based on desired baud rate and system clock frequency

UART0->BDH =(UART0_BDH_SBR_MASK) & (sbr>>8); // masks out upper 5 SBR bits
UART0->BDL = (UART0_BDL_SBR_MASK) & (sbr & 0xFF); // masks out lower 8 SBR bits

UART0->C4 &= (~UART0_C4_OSR_MASK) | osr;

UART0_C2 |= UART_C2_RIE_MASK; //enables interrupt generation for UART0 receiver
//UART0_C2 |= UART_C2_TIE_MASK; //enables interrupt generation for UART0 transmitter

UART0->C2 |= UART_C2_TE_MASK;  //enable transmitter for UART 0
UART0->C2 |= UART_C2_RE_MASK;  //enable receiver for UART 0

#endif

#ifdef U1
SIM_SCGC4 |= UART1_CLK_GATE_MASK;  //enable clock signal to UART module

UART1_C2 &= ~(UART_C2_TE_MASK); //disables transmitter for UART
UART1_C2 &= ~(UART_C2_RE_MASK); //disables receiver for UART
UART1_C1 = 0; //sets UART C1 register to default, 8 bit packets, no parity

sbr_mask = BUSCLK/(16*BAUD); // sets 13 bit SBR value based on desired baud rate and system clock frequency

bdl = (uint8_t)(sbr_mask & 0x00FF); // masks out lower 8 SBR bits 
bdh = (uint8_t)((sbr_mask & 0x1F00)>>8); // masks out upper 5 SBR bits

UART1_BDL = bdl;  //sets lower 8 SBR bits in UART BDL register
UART1_BDH |= bdh; //sets the higher 5 SBR bits in the UART BDH register

UART1_C2 |= UART_C2_RIE_MASK; //enables interrupt generation for UART receiver
UART1_C2 |= UART_C2_TIE_MASK; //enables interrupt generation for UART transmitter

UART1_C2 |= UART_C2_TE_MASK;  //enable transmitter for UART
UART1_C2 |= UART_C2_RE_MASK;  //enable receiver for UART

#endif

#ifdef U2
SIM_SCGC4 |= UART2_CLK_GATE_MASK;  //enable clock signal to UART module

UART2_C2 &= ~(UART_C2_TE_MASK); //disables transmitter for UART
UART2_C2 &= ~(UART_C2_RE_MASK); //disables receiver for UART
UART2_C1 = 0; //sets UART C1 register to default, 8 bit packets, no parity

sbr_mask = BUSCLK/(16*BAUD); // sets 13 bit SBR value based on desired baud rate and system clock frequency

bdl = (uint8_t)(sbr_mask & 0x00FF); // masks out lower 8 SBR bits 
bdh = (uint8_t)((sbr_mask & 0x1F00)>>8); // masks out upper 5 SBR bits

UART2_BDL = bdl;  //sets lower 8 SBR bits in UART BDL register
UART2_BDH |= bdh; //sets the higher 5 SBR bits in the UART BDH register

UART2_C2 |= UART_C2_RIE_MASK; //enables interrupt generation for UART receiver
UART2_C2 |= UART_C2_TIE_MASK; //enables interrupt generation for UART transmitter

UART2_C2 |= UART_C2_TE_MASK;  //enable transmitter for UART
UART2_C2 |= UART_C2_RE_MASK;  //enable receiver for UART

#endif




}


void UART_send(uint8_t * data)
{

#ifdef U0
	while (!(UART0_S1 & UART_S1_TDRE_MASK)); // wait until transmit data register buffer is empty
		UART0_S1 = 0;

	UART0_D = *data;  // transmit the byte by setting the UART data register
#endif

#ifdef U1
	while (!(UART1_S1 & UART_S1_TDRE_MASK)); // wait until transmit data register buffer is empty

	UART1_D = *data;  // transmit the byte by setting the UART data register
#endif

#ifdef U2
	while (!(UART2_S1 & UART_S1_TDRE_MASK)); // wait until transmit data register buffer is empty

	UART2_D = *data;  // transmit the byte by setting the UART data register
#endif

}



void UART_send_n(uint8_t * data, uint8_t length)
{
	int i;	//index variable


	for (i=0; i<length; i++)
	{
	#ifdef U0
		while (!(UART0_S1 & UART_S1_TDRE_MASK)); // wait until transmit data register buffer is empty

		UART0_D = *data;  // transmit the byte by setting the UART data register
	#endif

	#ifdef U1
		while (!(UART1_S1 & UART_S1_TDRE_MASK)); // wait until transmit data register buffer is empty

		UART1_D = *data;  // transmit the byte by setting the UART data register
	#endif

	#ifdef U2
		while (!(UART2_S1 & UART_S1_TDRE_MASK)); // wait until transmit data register buffer is empty

		UART2_D = *data;  // transmit the byte by setting the UART data register
	#endif

		data++; // increment the pointer to the array of data

	}

	data -= length;  //reset the the data pointer to its original value



}


uint8_t UART_receive(uint8_t* data)
{
	char rx;

#ifdef U0

	while (!(UART0_S1 & UART_S1_RDRF_MASK));	// wait until receive data buffer is full
	UART0_S1 =0;

	rx = UART0_D;  // read data from UART data register into input variable

	return rx;  // return the byte of the data read from the UART receive data buffer

#endif

#ifdef U1
	while (!(UART1_S1 & UART_S1_RDRF_MASK));	// wait until receive data buffer is full

	*data = UART1_D;  // read data from UART data register into input variable

	return *data;  // return the byte of the data read from the UART receive data buffer

#endif


#ifdef U2
	while (!(UART2_S1 & UART_S1_RDRF_MASK));	// wait until receive data buffer is full

	*data = UART2_D;  // read data from UART data register into input variable

	return *data;  // return the byte of the data read from the UART receive data buffer

#endif


}


uint8_t * UART_receive_n(uint8_t * data, uint8_t length)
{

	int i; // index variable

	for (i=0; i<length; i++)
	{
	
	#ifdef U0
		while (!(UART0_S1 & UART_S1_RDRF_MASK));	// wait until receive data buffer is full

		*data = UART0_D; 	// read data from UART data register into input variable
	#endif

	#ifdef U1
		while (!(UART1_S1 & UART_S1_RDRF_MASK));	// wait until receive data buffer is full

		*data = UART1_D; 	// read data from UART data register into input variable
	#endif

	#ifdef U2
		while (!(UART2_S1 & UART_S1_RDRF_MASK));	// wait until receive data buffer is full

		*data = UART2_D; 	// read data from UART data register into input variable
	#endif
	
	data++;  //increment data pointer to point to next element of the array

	}

	data -= length; // resets the data pointer to its original value

	return data;  // return the pointer to the received array of byters

}

void UART0_IRQHandler()
{

	PORTA_ISFR = 0xFFFFFFFF;
	uint8_t tx_packet = 0xF0;  //packet to transmit on if TDRE flag is set
	uint8_t rx_packet = 0;	   //variable to store received packet if RDRF flag is set
	if(UART0_S1 & UART_S1_RDRF_MASK) 
	{
		rx_packet = UART0_D; // read data UART data register into input variable
		status = CB_buffer_add_item (userbuff,rx_packet);  //get status of circular buffer

		if (status==0) //buffer is full
		{
		dump_flag=1;
		}
	}




}

