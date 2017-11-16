/**
* @file uart.c
* @brief defines uart operation and provides functions for uart operation and initialization
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "timerInit.h"
#include "MKL25Z4.h"
#include "core_cm0plus.h"

void myTPM_init()
{
	/**********clock to TPM setting**************/
	//Setting internal reference clock and enabling for use as MCGIRCLK
	//MCG-> C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK;

	//Setting internal reference clock as fast (vs. slow)
	//MCG -> C2 |= MCG_C2_IRCS_MASK;

	//system clock gating control register, clock gate to TPM0 module
	SIM -> SCGC6 |=SIM_SCGC6_TPM0_MASK;

	//Setting clock to be MCGIRCLK into TPM
	SIM-> SOPT2 |= SIM_SOPT2_TPMSRC(1);

	/******TPM configuration settings********/
	//Turns on DMA
	//TPM0->SC |= TPM_SC_DMA_MASK;

	//setting counter to count up, should be 0
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);

	/*value at which TPM gets to and then resets
	 *should read to max extent of counter
	 */
	TPM0->MOD = 0xFFFF;

	//sets prescale to divide by 1
	TPM0->SC |= TPM_SC_PS_MASK;

	/*LPTPM counter increments on every LPTPM counter clock
	use this to turn on timer before fxn to profile*/
	//TPM0->SC |= TPM_SC_CMOD(0b01);

	//enable TOF interrupts
	TPM0->SC |= TPM_SC_TOIE_MASK;

	//setting counter to input capture MSB:MSA = 0b00
	TPM0_C0SC &= (TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);

	//setting the input capture (MS0B:MS0A) to count on the rising edge
	//ELSB:ELSA = 0b01
	TPM0_C0SC &= TPM_CnSC_ELSB_MASK;
	TPM0_C0SC |= TPM_CnSC_ELSA_MASK;

	//enable channel interrupts
	TPM0_C0SC |= TPM_CnSC_CHIE_MASK;

	//enable DMA
	TPM0_C0SC |= TPM_CnSC_DMA_MASK;

	return;

}
