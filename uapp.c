//**********************************************************************************************
// tinyRTX Filename: uapp.c (User APPlication)
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

//*******************************************************************************
//
//	Hardware: MSP-EXP430G2 LaunchPad.
//		TI MSP430G2553 processor (32 kHz external crystal optional).
//
// Functions:
//  1) Run a timer to generate task intervals at 100ms, toggle LED.
//  2) Read on-chip temperature every 1.0s using ADC (tested but value not used).
//  3) Toggle LED in 2.0s task.
//
//*******************************************************************************

// Complete MSP430G2553 (20-pin device) pin assignments for MSP-EXP430G2 LaunchPad.
//
//	1)
//	DVCC: Digital supply voltage
//
//	2)
//	P1.0: General-purpose digital I/O pin
//	TA0CLK: Timer0_A, clock signal TACLK input
//	ACLK: ACLK signal output
//	A0: ADC10 analog input A0(1)
//	CA0: Comparator_A+, CA0 input
//
//	3)
//	P1.1: General-purpose digital I/O pin
//	TA0.0: Timer0_A, capture: CCI0A input, compare: Out0 output / BSL transmit
//	UCA0RXD: USCI_A0 UART mode: receive data input
//	UCA0SOMI: USCI_A0 SPI mode: slave data out/master in
//	A1: ADC10 analog input A1(1)
//	CA1: Comparator_A+, CA1 input
//
//	4)
//	P1.2: General-purpose digital I/O pin
//	TA0.1: Timer0_A, capture: CCI1A input, compare: Out1 output
//	UCA0TXD: USCI_A0 UART mode: transmit data output
//	UCA0SIMO: USCI_A0 SPI mode: slave data in/master out
//	A2: ADC10 analog input A2(1)
//	CA2: Comparator_A+, CA2 input
//
//	5)
//	P1.3: General-purpose digital I/O pin
//	ADC10CLK: ADC10, conversion clock output(1)
//	A3: ADC10 analog input A3(1)
//	VREF-/VEREF-: ADC10 negative reference voltage (1)
//	CA3: Comparator_A+, CA3 input
//	CAOUT: Comparator_A+, output
//
//	6)
//	P1.4: General-purpose digital I/O pin
//	SMCLK: SMCLK signal output
//	UCB0STE: USCI_B0 slave transmit enable
//	UCA0CLK: USCI_A0 clock input/output
//	A4: ADC10 analog input A4(1)
//	VREF+/VEREF+: ADC10 positive reference voltage(1)
//	CA4: Comparator_A+, CA4 input
//	TCK: JTAG test clock, input terminal for device programming and test
//
//	7)
//	P1.5: General-purpose digital I/O pin
//	TA0.0: Timer0_A, compare: Out0 output / BSL receive
//	UCB0CLK: USCI_B0 clock input/output
//	UCA0STE: USCI_A0 slave transmit enable
//	A5: ADC10 analog input A5(1)
//	CA5: Comparator_A+, CA5 input
//	TMS: JTAG test mode select, input terminal for device programming and test
//
//	8)
//	P2.0: General-purpose digital I/O pin
//	TA1.0: Timer1_A, capture: CCI0A input, compare: Out0 output
//
//	9)
//	P2.1: General-purpose digital I/O pin
//	TA1.1: Timer1_A, capture: CCI1A input, compare: Out1 output
//
//	10)
//	P2.2: General-purpose digital I/O pin
//	TA1.1: Timer1_A, capture: CCI1B input, compare: Out1 output
//
//	11)
//	P2.3: General-purpose digital I/O pin
//	TA1.0: Timer1_A, capture: CCI0B input, compare: Out0 output
//
//	12)
//	P2.4: General-purpose digital I/O pin
//	TA1.2: Timer1_A, capture: CCI2A input, compare: Out2 output
//
//	13)
//	P2.5: General-purpose digital I/O pin
//	TA1.2: Timer1_A, capture: CCI2B input, compare: Out2 output
//
//	14)
//	P1.6: General-purpose digital I/O pin
//	TA0.1: Timer0_A, compare: Out1 output
//	A6: ADC10 analog input A6(1)
//	CA6: Comparator_A+, CA6 input
//	UCB0SOMI: USCI_B0 SPI mode: slave out master in
//	UCB0SCL: USCI_B0 I2C mode: SCL I2C clock
//	TDI/TCLK: JTAG test data input or test clock input during programming and test
//
//	15)
//	P1.7: General-purpose digital I/O pin
//	A7: ADC10 analog input A7(1)
//	CA7: Comparator_A+, CA7 input
//	CAOUT: Comparator_A+, output
//	UCB0SIMO: USCI_B0 SPI mode: slave in master out
//	UCB0SDA: USCI_B0 I2C mode: SDA I2C data
//	TDO/TDI: JTAG test data output terminal or test data input during programming and test
//
//	16)
//	RST*: Reset*
//	NMI: Nonmaskable interrupt input
//	SBWTDIO: Spy-Bi-Wire test data input/output during programming and test
//
//	17)
//	TEST: Selects test mode for JTAG pins on Port 1. The device protection fuse is
//	connected to TEST.
//	SBWTCK: Spy-Bi-Wire test clock input during programming and test
//
//	18)
//	XOUT: Output terminal of crystal oscillator(3)
//	P2.7: General-purpose digital I/O pin
//
//	19)
//	XIN: Input terminal of crystal oscillator
//	P2.6: General-purpose digital I/O pin
//	TA0.1: Timer0_A, compare: Out1 output
//
//	20)
//	DVSS: NA Ground reference
//
//*******************************************************************************

#include <msp430g2553.h>
#include "srtx.h"
#include "uapp.h"

//*******************************************************************************
//
//	SUSR: System to User Redirection.
//
//	These routines provide the interface from the SRTX (System Real Time eXecutive)
//	and SISD (Interrupt Service Routine Director) to user application code.
//
//	 User initialization at Power-On Reset Phase A.
//	 Application time-critical initialization, no interrupts.

extern void UAPP_POR_PhaseA(void)
{
	WDTCTL = WDTPW + WDTHOLD;         		// Stop watchdog timer.

	// Configure clocks.

	if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)		// If calibration data erased..
//	if (CALBC1_12MHZ ==0xFF || CALDCO_12MHZ == 0xFF)	// If calibration data erased..
	 SRTX_CauseFault();		                			// ..then cause fault.

  BCSCTL1 = CALBC1_1MHZ; 					// Set DCO range from calibration.
  DCOCTL = CALDCO_1MHZ;  					// Set DCO freq and mod from calibration.
//  CALDCO_12MHZ       = 0x10FA;
//  CALBC1_12MHZ       = 0x10FB;

  BCSCTL3 |= LFXT1S_2;                      // LFXT1 set to VLO.
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag.

  BCSCTL2 |= SELM_0 + DIVM_3 + DIVS_3;		// MCLK = DCO/8 (=125kHz).
  	  	  	  	  	  	  	  	  			// SMCLK = DCO/8 (=125kHz).
}

extern void UAPP_POR_PhaseB(void)
{
	// Configure LEDs.
	P1DIR = BIT6 + BIT0;                    // P1.6 and P1.0 outputs.
											//
	P1OUT = 0;                              // LEDs off
}

void UAPP_TimerA2_Init(void)
{
	// Configure TimerA2.
	CCTL0 = CCIE;						// CCR0 interrupt enabled.
	CCR0 = 1200;						// Initial count for 100ms.
	TACTL = TASSEL_1 + MC_2;			// Select ACLK and start continuous mode.
}

void UAPP_TimerA2_New100ms(void)
{
	CCR0 += 1200;						// Add 100ms to TimerA2 for next int.
}

