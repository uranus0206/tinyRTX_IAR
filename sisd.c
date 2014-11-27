//*******************************************************************************
// tinyRTX Filename: sisd.c (System Interrupt Service Director)
//
// Copyright 2014 Sycamore Software, Inc.  ** www.tinyRTX.com **
// Distributed under the terms of the GNU Lesser General Purpose License v3
//
// This file is part of tinyRTX. tinyRTX is free software: you can redistribute
// it and/or modify it under the terms of the GNU Lesser General Public License
// version 3 as published by the Free Software Foundation.
//
// tinyRTX is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// (filename copying.lesser.txt) and the GNU General Public License (filename
// copying.txt) along with tinyRTX.  If not, see <http://www.gnu.org/licenses/>.
//
// Revision history:
//   11Sep14 SHiggins@tinyRTX.com   Converted from PIC18F452-C to MSP430G2553.
//
//*******************************************************************************

#include	<msp430g2553.h>
#include	"srtx.h"

#pragma vector=TIMER0_A0_VECTOR
__interrupt void SISD_ISR_TimerA( void )
{
	// ULP Advisor doesn't care for this call out of an ISR.
	// Might be remedied with some macros, which would also
	//   preserve using variables within their own modules.

	SRTX_Scheduler();  						// Schedule tasks on 100ms int.
	__bic_SR_register_on_exit(CPUOFF);  	// Return from LPM to active mode.
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void SISD_ISR_ADC10( void )
{
	if( ++SRTX_Sched_Cnt_TaskADC == 0)  	// Schedule ADC task, and..
		--SRTX_Sched_Cnt_TaskADC;       	// ..max it at 0xFF if it rolls over.
	__bic_SR_register_on_exit(CPUOFF);  	// Return from LPM to active mode.
}

// MSP430G2553 Interrupt Vector Table.
//	Unused vectors fall into common routine for breakpoint location.
//  Used vectors commented out because implemented below.
//
//#pragma vector=RESET_VECTOR		/* 0xFFFE Reset [Highest Priority] */
#pragma vector=NMI_VECTOR			/* 0xFFFC Non-maskable */
#pragma vector=TIMER1_A0_VECTOR		/* 0xFFFA Timer1_A CC0 */
#pragma vector=TIMER1_A1_VECTOR		/* 0xFFF8 Timer1_A CC1-4, TA1 */
#pragma vector=COMPARATORA_VECTOR	/* 0xFFF6 Comparator A */
#pragma vector=WDT_VECTOR			/* 0xFFF4 Watchdog Timer */
//#pragma vector=TIMER0_A0_VECTOR	/* 0xFFF2 Timer0_A CC0 */
#pragma vector=TIMER0_A1_VECTOR		/* 0xFFF0 Timer0_A CC1, TA0 */
#pragma vector=USCIAB0RX_VECTOR		/* 0xFFEE USCI A0/B0 Receive */
#pragma vector=USCIAB0TX_VECTOR		/* 0xFFEC USCI A0/B0 Transmit */
//#pragma vector=ADC10_VECTOR		/* 0xFFEA ADC10 */
// Not implemented on MSP430G2553	/* 0xFFE8 */
#pragma vector=PORT2_VECTOR			/* 0xFFE6 Port 2 */
#pragma vector=PORT1_VECTOR			/* 0xFFE4 Port 1 */
// Not implemented on MSP430G2553	/* 0xFFE2 */
// Not implemented on MSP430G2553	/* 0xFFE0 */
// Bootstrap Loader Security Key    /* 0xFFDE */
// Not implemented on MSP430G2553   /* 0xFFDC through 0xFFCD */
__interrupt void SISD_ISR_Unimplemented( void )
{
	// Place breakpoint on following line to catch unimplemented interrupts.
	__bic_SR_register_on_exit(LPM3_bits);  	// Return from LPM to active mode.
}

