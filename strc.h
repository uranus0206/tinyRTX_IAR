//*******************************************************************************
// tinyRTX Filename: strc.h (System TRaCe facility)
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
//   09Sep14 SHiggins@tinyRTX.com   Converted from PIC18F452-C to MSP430G2553.
//
//*******************************************************************************


#define	STRC_TSK_BEG_1		0x11
#define	STRC_TSK_END_1		0x12
#define	STRC_TSK_BEG_2		0x21
#define	STRC_TSK_END_2		0x22
#define	STRC_TSK_BEG_3		0x31
#define	STRC_TSK_END_3		0x32
#define	STRC_TSK_BEG_ADC	0x61
#define	STRC_TSK_END_ADC	0x62

extern void STRC_Init( void );
extern void STRC_Trace( unsigned char );
