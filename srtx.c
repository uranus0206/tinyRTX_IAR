//*******************************************************************************
// tinyRTX Filename: srtx.c (System Real Time eXecutive)
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

#include	<msp430g2553.h>
#include	"srtxuser.h"
#include	"susr.h"
#include	"strc.h"

extern void SRTX_Scheduler( void );	// Needed to allow SISD to call SRTX_Scheduler().
void SRTX_CauseFault( void );

//*******************************************************************************
//
// SRTX service variables.
//
// Task timer counters track remaining SRTX-timer events before each task is scheduled.

volatile unsigned short SRTX_Timer_Cnt_Task1;
volatile unsigned short SRTX_Timer_Cnt_Task2;
volatile unsigned short SRTX_Timer_Cnt_Task3;

// Task schedule counters track pending task schedule counts.
//	When any count > 0 then task has been scheduled but not completed.
//	When any count > 1 then task has been scheduled at least twice before compleleting,
//	therefore technically this is a task overflow.  However, this may be an acceptable
//	occasional condition in high-throughput sheduling, and SRTX will recover gracefully.

volatile unsigned short SRTX_Sched_Cnt_Task1;
volatile unsigned short SRTX_Sched_Cnt_Task2;
volatile unsigned short SRTX_Sched_Cnt_Task3;
volatile unsigned int SRTX_Sched_Cnt_TaskADC;

//*******************************************************************************
//
// SRTX Services.
//
//*******************************************************************************
//
// SRTX_Init: tinyRTX System Real Time eXecutive  Initialization
//
// First, perform any application time-critical initialization which simply must be
// done as soon as power is applied or reset occurs.  Application should not initialize
// any hardware to generate interrupts during Phase A.  
//   Call USER_POR_InitPhaseA.
//
// Second, perform SRTX non-time critical initialization:
//   Initialize SRTX counters.
//   Call SRTX_Scheduler to initially schedule all timebase tasks.
//
// Third, perform any application non-time critical initialization which may
// generate interrupts.  Application should do anything necessary before initial
// execution of timebased tasks. Start timebase timer.
//   Call USER_POR_InitPhaseB.
//
// Fourth, Begin SRTX background loop.  This will immediately dispatch all
//  timebase tasks which have been scheduled with an initial load value of 1.
//   Goto SRTX_Dispatcher.
//
//*******************************************************************************

void main (void)
{
	SUSR_POR_PhaseA();         // Application time-critical init, no interrupts.
    STRC_Init();               // Trace buffer init.

// Init all the task timebase counters.

    SRTX_Timer_Cnt_Task1 = SRTX_CNT_INIT_TASK1;
    SRTX_Timer_Cnt_Task2 = SRTX_CNT_INIT_TASK2;
    SRTX_Timer_Cnt_Task3 = SRTX_CNT_INIT_TASK3;

// Clear all the task schedule counters.

    SRTX_Sched_Cnt_Task1 = 0;
    SRTX_Sched_Cnt_Task2 = 0;
    SRTX_Sched_Cnt_Task3 = 0;
    SRTX_Sched_Cnt_TaskADC = 0;

	SRTX_Scheduler();		// Schedule all timebase tasks.
    SUSR_POR_PhaseB();		// Application non-time critical init.
    						// Starts the timebase and enables those interrupts.

    __bis_SR_register(GIE);	// Enable interrupts and drop into SRTX Dispatcher.

//*******************************************************************************
//
// SRTX Dispatcher:
//   Background task starts after all initialization complete.
//   Checks all tasks in priority order to see which are scheduled.
//   If multiple tasks are scheduled, then highest priority task executes.
//   No return from this routine.
//
// Priority (1 = highest):
//	1: ADC
//	2: Task1
//	3: Task2
//	4: Task3
//
//*******************************************************************************

	while(1)
	{
		if( SRTX_Sched_Cnt_TaskADC > 0 ) 		// If task has been scheduled..
		{
			SUSR_TaskADC();						// ..then invoke the task.
			SRTX_Sched_Cnt_TaskADC--;			// Decrement task schedule count.
			if( SRTX_Sched_Cnt_TaskADC > 0 )	// If decremented task schedule count not 0..
				SRTX_CauseFault();				// ..then provide a breakpoint location to trap it.
		} // if( SRTX_Sched_Cnt_TaskADC > 0 ).
		else
		{
			if( SRTX_Sched_Cnt_Task1 > 0 ) 			// If task has been scheduled..
			{
				SUSR_Task1();						// ..then invoke the task.
				SRTX_Sched_Cnt_Task1--;				// Decrement task schedule count.
				if( SRTX_Sched_Cnt_Task1 > 0 )		// If decremented task schedule count not 0..
					SRTX_CauseFault();				// ..then provide a breakpoint location to trap it.
			} // if( SRTX_Sched_Cnt_Task1 > 0 ).
			else
			{
				if( SRTX_Sched_Cnt_Task2 > 0 ) 			// If task has been scheduled..
				{
					SUSR_Task2();						// ..then invoke the task.
					SRTX_Sched_Cnt_Task2--;				// Decrement task schedule count.
					if( SRTX_Sched_Cnt_Task2 > 0 )		// If decremented task schedule count not 0..
						SRTX_CauseFault();				// ..then provide a breakpoint location to trap it.
				} // if( SRTX_Sched_Cnt_Task2 > 0 ).
				else
				{
					if( SRTX_Sched_Cnt_Task3 > 0 ) 			// If task has been scheduled..
					{
						SUSR_Task3();						// ..then invoke the task.
						SRTX_Sched_Cnt_Task3--;				// Decrement task schedule count.
						if( SRTX_Sched_Cnt_Task3 > 0 )		// If decremented task schedule count not 0..
							SRTX_CauseFault();				// ..then provide a breakpoint location to trap it.
					} // if( SRTX_Sched_Cnt_Task3 > 0 ).
				}
			}
		}

		// Check all scheduled task counts as an atomic operation, if all are non-zero..
		// ..then go to sleep.  Else let dispatcher loop around again.

		__bic_SR_register(GIE);  // Disable interrupts so we can check task flags.
		if(
			SRTX_Sched_Cnt_TaskADC == 0 &&
			SRTX_Sched_Cnt_Task1   == 0 &&
			SRTX_Sched_Cnt_Task2   == 0 &&
			SRTX_Sched_Cnt_Task3   == 0
			)
		{
			__bis_SR_register(GIE+LPM3_bits);	// Sleep in LPM3.
		}
		else
		{
			__bis_SR_register(GIE);  			// Stay in Active Mode and check again.
		}

	} // while(1).
}

//*******************************************************************************
//
// SRTX Scheduler:
//   Arrives here with each timebase event.
//   Decrements each task timer.
//   If task timer has expired, reloads timer and schedules task by incrementing
//		task schedule count (no rollover, max count at 0xFF).
//
//*******************************************************************************

void SRTX_Scheduler( void )
{
	if( --SRTX_Timer_Cnt_Task1 == 0)		// Decrement task 1 timer count, if expires then...
	{
    	SRTX_Timer_Cnt_Task1 = SRTX_CNT_RELOAD_TASK1;  	// ..then reload timer count.
		if( ++SRTX_Sched_Cnt_Task1 == 0)				// Increment task schedule count.
			--SRTX_Sched_Cnt_Task1;						// Max it at 0xFF if it rolls over.
	}

	if( --SRTX_Timer_Cnt_Task2 == 0)		// Decrement task 2 timer count, if expires then...
	{
    	SRTX_Timer_Cnt_Task2 = SRTX_CNT_RELOAD_TASK2;  	// ..then reload timer count.
		if( ++SRTX_Sched_Cnt_Task2 == 0)				// Increment task schedule count.
			--SRTX_Sched_Cnt_Task2;						// Max it at 0xFF if it rolls over.
	}

	if( --SRTX_Timer_Cnt_Task3 == 0)		// Decrement task 3 timer count, if expires then...
	{
    	SRTX_Timer_Cnt_Task3 = SRTX_CNT_RELOAD_TASK3;  	// ..then reload timer count.
		if( ++SRTX_Sched_Cnt_Task3 == 0)				// Increment task schedule count.
			--SRTX_Sched_Cnt_Task3;						// Max it at 0xFF if it rolls over.
	}
}

void SRTX_CauseFault(void)
 {
   P1OUT = 0x01;                    // Turn red LED on.
   while(1); 			            // Trap location.
 }
