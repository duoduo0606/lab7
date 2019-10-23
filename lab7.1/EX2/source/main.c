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
#include<avr/interrupt.h>
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
typedef enum States{start, wait, play, stop,sus}States;
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
    static unsigned char cnt;
    static unsigned char flag;
    static unsigned char score;
    
    unsigned char A0 =((~PINA) & 0x01);
    switch (state){
    case start:
      b=1;
      cnt=0;
      flag=0;
      
      score = 5;
      state = wait;
      break;
    case wait:
      state = A0 ? play : wait;
      break;
    case play:
      state = A0 ? stop : play;
      break;
    case stop:
      state = sus;
      break;
    case sus:
      state = A0 ? play : sus;
      break;
    default: 
      state = start;
      break;
    }
    switch(state){
    case start:
  
      break;
    case wait: 
    	LCD_init();
	LCD_WriteData('0'+5);
       break;
    case play:
      
      if(cnt<2 && (flag%2==0)){
	b = b*2;
	cnt++;}
      else  if(cnt<2 && (flag%2)){
	b = b/2;
	cnt++;}
      if(cnt==2){
	flag++;
	cnt=0;}
        break;
    case stop:
      if(b == 2){
	score++;
	
	}
      else{if (score > 0)
	score--;}
      if(score < 9){
	LCD_init();
	LCD_WriteData('0'+score);}
      else{
	LCD_init();
	LCD_DisplayString(1,"victory");}
        break;
    case sus:
      break;
    }
    PORTB = b;
    return state;
  }
      
    
int main(void) {
   DDRC = 0xFF;PORTC = 0x00;
   DDRD = 0xFF;PORTD = 0x00;
   DDRA = 0x00;PORTA = 0xFF;
   DDRB = 0xFF;PORTB = 0x00;
   TimerSet(300);
   TimerOn();
   States state = start;
   while (1) {
     state = exampleTick(state);
     while(!TimerFlag);
     TimerFlag = 0;
    }
    return 1;
}
