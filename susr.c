//**********************************************************************************************
// tinyRTX Filename: susr.c (System to USeR interface)
//
// Copyright 2014 Sycamore Software, Inc.  ** www.tinyRTX.com **
// Distributed under the terms of the GNU Lesser General Purpose License v3
//
// This file is part of tinyRTX. tinyRTX is free software: you can redistribute
// it and/or modify it under the terms of the GNU Lesser General Public License
// version 3 as published by the Free Software Foundation.
//
// tinyRTX is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY// without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// (filename copying.lesser.txt) and the GNU General Public License (filename
// copying.txt) along with tinyRTX.  If not, see <http://www.gnu.org/licenses/>.
//
// Revision history:
//   09Sep14 SHiggins@tinyRTX.com   Converted from PIC18F452-C to MSP430G2553.
//
//*******************************************************************************

#include <msp430g2553.h>
#include "strc.h"
#include "uapp.h"
#include "uadc.h"

//*******************************************************************************
//
//	SUSR: System to User Redirection.
//
//	These routines provide the interface from the SRTX (System Real Time eXecutive)
//	and SISD (Interrupt Service Routine Director) to user application code.
//
//	 User initialization at Power-On Reset Phase A.
//	 Application time-critical initialization, no interrupts.

extern void SUSR_POR_PhaseA(void)
{
	UAPP_POR_PhaseA();
}

extern void SUSR_POR_PhaseB(void)
{
	UAPP_POR_PhaseB();
	UAPP_TimerA2_Init();		// Configure TimerA2, enable timer ints.
	UADC_Init();				// Configure ADC.
}

//
// User interface to reset timebase timer for new interrupt.
//
extern void SUSR_Timebase(void)
{
	UAPP_TimerA2_New100ms();		// Re-init timer so new int in 100ms.
}

//
// User interface to Task1.
//
extern void SUSR_Task1(void)
{
		STRC_Trace(STRC_TSK_BEG_1);
		P1OUT ^= BIT0;				// Toggle LED.
		SUSR_Timebase();			// Update CCR0 so new timer interrupt in 100ms.
		STRC_Trace(STRC_TSK_END_1);
}
//
// User interface to Task2.
//
extern void SUSR_Task2(void)
{
		STRC_Trace(STRC_TSK_BEG_2);
		UADC_Trigger();				// Trigger ADC conversion.
		STRC_Trace(STRC_TSK_END_2);
}
//
// User interface to Task3.
//
extern void SUSR_Task3(void)
{
		STRC_Trace(STRC_TSK_BEG_3);
		P1OUT ^= BIT6;				// Toggle LED.
		STRC_Trace(STRC_TSK_END_3);
}

//
// User interface to TaskADC.
//
extern void SUSR_TaskADC(void)
{
		STRC_Trace(STRC_TSK_BEG_ADC);
		UADC_Process();					// Process ADC value.
		STRC_Trace(STRC_TSK_END_ADC);
}
