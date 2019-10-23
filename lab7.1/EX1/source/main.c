/*	Author: hwu901
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "io.h"
#include<avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
typedef enum States{start, wait, add, minus, reset, stop}States;
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
static unsigned char b ;

void TimerOn(){
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;
  _avr_timer_cntcurr = _avr_timer_M;
  SREG |= 0x80;
}

void TimerOff(){
  TCCR1B = 0x00;
}

void TimerISR(){
  TimerFlag =1;
}

ISR(TIMER1_COMPA_vect){
  _avr_timer_cntcurr--;
  if(_avr_timer_cntcurr==0){
    TimerISR();
    _avr_timer_cntcurr = _avr_timer_M;
  }
}

void TimerSet(unsigned long M){
  _avr_timer_M = M;
  _avr_timer_cntcurr = _avr_timer_M;
}
  

  int exampleTick(int state){
    unsigned char A0 =((~PINA) & 0x01);
    unsigned char A1= ((~PINA) & 0x02);
    switch (state){
    case start:
      b=0;
      state = stop;
      break;
    case add:
      state = stop;
      break;
    case minus:
      state= stop;
      break;
    case stop:
      if(A0 && !A1){
	state=add;}
     else if(A1 && !A0){
	state=minus;}
     else if(A0 && A1){
       state=reset;}
	break;
     case reset:
       state = stop;
       break;
    default:
      state = start;
      break;
    }
    switch(state){
    case start:
      break;
    case add:
      if(b < 9){
	b++;}
      else if(b == 9){
	b = 9;}
      break;
    case minus:
      if(b >0){
	b--;}
      else if(b == 0){
	b = 0;}
      break;
    case reset:
      b = 0;
      break;
    case stop:
        break;
    }
    LCD_init();
    LCD_WriteData(b+'0');
    return state;
  }
      
    
int main(void) {
   DDRC = 0xFF;PORTC = 0x00;
   DDRA = 0x00;PORTA = 0xFF;
   DDRD = 0xFF;PORTD = 0x00;
   TimerSet(200);
   TimerOn();
   States state = start;
   
  
    while (1) {
     state = exampleTick(state);
	
	
	
     while(!TimerFlag);
     TimerFlag = 0;
    }
    return 1;
}
