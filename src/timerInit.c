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

void myTPM_init()
{

	//Sets CMOD to internal clock, SC=status and control register
	//by and'ing, turning off TPM counter
	//TPM0_SC &= TPM_SC_CMOD_MASK;
	//TPM0_BASE_PTR->SC &= TPM_SC_CMOD_MASK;

	//Setting internal reference clock and enabling for use as MCGIRCLK
	MCG_BASE_PTR -> C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK;

	//Setting internal reference clock as fast (vs. slow)
	MCG_BASE_PTR -> C2 = MCG_C2_IRCS_MASK;

	//system clock gating control register, clock gate to TPM0 module
	SIM_BASE_PTR -> SCGC6 |=SIM_SCGC6_TPM0_MASK;

	//Setting clock to be MCGIRCLK into TPM
	SIM_BASE_PTR -> SOPT2 |=SIM_SOPT2_TPMSRC(3);

	//Turns on DMA
	//TPM0_BASE_PTR->SC |= TPM_SC_DMA_MASK;

	//sets prescale to divide by 1
	//TPM0_BASE_PTR->SC = TPM_SC_PS_MASK;
	TPM0_BASE_PTR->SC = TPM_SC_CMOD(1) | TPM_SC_PS(0);

	//setting counter to count up
	TPM0_BASE_PTR->SC &= TPM_SC_CPWMS_MASK;

	//setting counter to input capture
	TPM0_C0SC &=(TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);

	//setting the input capture to count on the rising edge
	TPM0_C0SC &= TPM_CnSC_ELSB_MASK;
	TPM0_C0SC |= TPM_CnSC_ELSA_MASK;

	//need to turn on the counter when ready using this line
	//TPM0_SC |= TPM_SC_CMOD_MASK;
	return;

}
