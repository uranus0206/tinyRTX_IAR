//*******************************************************************************
// tinyRTX Filename: uapp.h (User APPlication)
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
//   10Sep14 SHiggins@tinyRTX.com   Converted from PIC18F452-C to MSP430G2553.
//
//*******************************************************************************

extern void UAPP_POR_PhaseA( void );
extern void UAPP_POR_PhaseB( void );
extern void UAPP_TimerA2_Init( void );
extern void UAPP_TimerA2_New100ms( void );
