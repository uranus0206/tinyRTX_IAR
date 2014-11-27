//*******************************************************************************
// tinyRTX Filename: strc.c (System TRaCe facility)
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

extern	void	STRC_Init( void );
extern	void	STRC_Trace( unsigned char );

//*******************************************************************************
//
// STRC Trace Service.
//
// STRC trace buffer and related service variables.

#define	STRC_BUFFER_SIZE 0x80					// Define trace buffer size.

unsigned char STRC_Buffer[ STRC_BUFFER_SIZE ];	// Trace buffer.
unsigned char STRC_Idx;							// Trace buffer current index.
unsigned char* STRC_Ptr;						// Trace buffer current index.

//*******************************************************************************
//
// STRC_Init() - Initialize trace buffer.
//
extern void STRC_Init(void)
{
	for ( STRC_Idx = 0; STRC_Idx < STRC_BUFFER_SIZE; STRC_Idx++ )
	{
		STRC_Buffer[ STRC_Idx ] = 0;
	}
	STRC_Idx = 0;
}

//*******************************************************************************
//
//   STRC_Trace() - Input arg is stored at location pointed to by STRC_Ptr++.
//
extern void STRC_Trace( unsigned char traceToken)
{
	STRC_Buffer[ STRC_Idx++ ] = traceToken;	// Save token in trace buffer.
	if( STRC_Idx >= STRC_BUFFER_SIZE )		// Reset pointer if buffer filled.
		STRC_Idx = 0;
}
