//**********************************************************************************************
// tinyRTX Filename: uadc.c (User Analog to Digital Conversion)
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
//  10Sep14 SHiggins@tinyRTX.com   Converted from PIC18F452-C to MSP430G2553.
//
//*******************************************************************************
//
#include <msp430g2553.h>

extern void UADC_Init( void );
extern void UADC_Trigger( void );
extern void UADC_Process( void );

extern void UADC_Init(void)
{
	// Configure ADC.
	ADC10CTL1 = INCH_10 + ADC10DIV_0;	// INCH_10: temperature sensor.
										// ADC10DIV_0: clock / 1.
										// ADC10SSEL_0(default): ADC10OSC.
}

void UADC_Trigger(void)
{
	ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
										// SREF_1: range from Vss to VREF+.
										// ADC10SHT_3: 64 × ADC10CLKs (max) sample-and-hold time.
										// REFON: reference generator on.
										// ADC10ON: turns on the ADC10.
										// ADC10IE: ADC10 interrupt enabled.

	__delay_cycles(4);					// Wait for ADC Ref to settle

	ADC10CTL0 |= ENC + ADC10SC;       	// ENC: Enable conversion.
										// ADC10SC: Start sampling and conversion.
}

void UADC_Process(void)
{
	volatile int tempRaw;

	ADC10CTL0 &= ~ENC;				   		// ~ENC: Enable conversion OFF.
											// (MUST be done before ~REFON.)

	ADC10CTL0 &= ~(REFON + ADC10ON);        // ~REFON: reference OFF.
											// ~ADC10ON: ADC10 OFF.

	tempRaw = ADC10MEM;						// Read conversion value.

	// At this point we can do cool stuff with ADC value.
}
