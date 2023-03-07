#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"

extern uint8_t old_value, curr_value;

void FIB_up(void);
void FIB_down(void);

void EINT0_IRQHandler (void)	  
{
	//RESET
	old_value=0;
	curr_value=1;
	LED_Out(curr_value);
  LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}

void EINT1_IRQHandler (void)	  
{
	//INCREMENTO
	if (curr_value<233) {
		FIB_up();
		LED_Out(curr_value);
	}	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  
{
	//DECREMENTO
	if(old_value>0) {
		FIB_down();
		LED_Out(curr_value);
	} 
	LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */   
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

