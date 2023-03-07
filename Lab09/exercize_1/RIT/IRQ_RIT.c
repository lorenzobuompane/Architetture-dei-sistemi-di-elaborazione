/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"

extern uint8_t old_value, curr_value;
void FIB_up(void);
void FIB_down(void);

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void)
{			
	static int down=0;	
	down++;
	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0 ){	// INT0
		reset_RIT();
		switch(down){
			case 1:
				old_value=0;
				curr_value=1;
				LED_Out(curr_value);
				break;
			default:
				break;
		}
	}
	else if((LPC_GPIO2->FIOPIN & (1<<11)) == 0 ){ //KEY1
		reset_RIT();
		switch(down){
			case 1:
				//INCREMENTO
				if (curr_value<233) {
					FIB_up();
					LED_Out(curr_value);
				}
				break;
			default:
				break;
		}
	}
	else if((LPC_GPIO2->FIOPIN & (1<<12)) == 0 ){ //KEY2
		reset_RIT();
		switch(down){
			case 1:
				//DECREMENTO
				if(old_value>0) {
					FIB_down();
					LED_Out(curr_value);
				} 
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down=0;			
		disable_RIT();
		reset_RIT();
		//INT0
		NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		//KEY1
		NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		//KEY2
		NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
	}
		
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/*----------------------------------------------------------------------------
  Function that increment/decrement Fibonacci
 *----------------------------------------------------------------------------*/
void FIB_up(void) {
	int tmp;
	tmp=old_value;
	old_value=curr_value;
	curr_value=tmp+old_value;	
}

void FIB_down(void){
	int tmp;
	tmp=curr_value;
	curr_value=old_value;
	old_value=tmp-old_value;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
