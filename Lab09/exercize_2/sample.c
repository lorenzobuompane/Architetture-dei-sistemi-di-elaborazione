/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: 
 *		to control led11 and led 10 through EINT buttons (similarly to project 03_)
 *		to control leds9 to led4 by the timer handler (1 second - circular cycling)
 * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/

                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"

/* Led external variables from funct_led */
extern unsigned char led_value;					/* defined in lib_led								*/
#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
		
	LED_On(7);
	
	LPC_SC->PCONP |= (1<<22);							/* alimentazione Timer */
	LPC_SC->PCLKSEL1 |= (1<<22);					/* scelgo freq timer 100MHz  */
	init_timer_double_interval(2, 0x0001FBD0, 0x0001FBD0);		// 	100%
	//init_timer_double_interval(2, 0x00017CDC, 0x0001FBD0);		//	75%
	//init_timer_double_interval(2, 0x0000FDE8, 0x0001FBD0);		//	50%
	//init_timer_double_interval(2, 0x00007EF4, 0x0001FBD0);		//	25%
	//init_timer_double_interval(2, 0x000032C8, 0x0001FBD0);		//	10%
	//init_timer_double_interval(2, 0x000000C8, 0x0001FBD0);		//	0.15%
	enable_timer(2);
	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= 0xFFFFFFFFD;						
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
