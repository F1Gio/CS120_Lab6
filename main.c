/*	Author: Giovany Turrubiartes
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;

}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum SM1_States {SM1_Start, SM1_PB0, SM1_PB1, SM1_PB2, SM1_PB11, SM1_PB00, SM1_Wait} SM1_State;


int main(void) {
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(300);
	TimerOn();
	unsigned char led = 0x00;
	unsigned char button = 0x00;
    /* Insert DDR and PORT initializations */

    /* Insert your solution below */
    while (1) {

	button = ~PINA & 0x01;

	switch(SM1_State) {
		case SM1_Start:
			if (button) {
				SM1_State = SM1_Start;
				}

			else {
				SM1_State = SM1_PB0;
			}

			break;

		case SM1_PB0:
			if (button) {
				SM1_State = SM1_Wait;
			}

			else {
				SM1_State = SM1_PB1;
			}

			break;

		case SM1_PB1:
			if (button) {
				SM1_State = SM1_Wait;
			}

			else {
				SM1_State = SM1_PB2;
			}

			break;

		case SM1_PB2:
			if (button) {
				SM1_State = SM1_Wait;
			}

			else {
				SM1_State = SM1_PB11;
			}

			break;

		case SM1_PB11:
			if (button) {
				SM1_State = SM1_Wait;
			}

			else {
				SM1_State = SM1_PB00;
			}

			break;

		case SM1_PB00:
			if (button) {
				SM1_State = SM1_Wait;
			}

			else {
				SM1_State = SM1_PB0;
			}

			break;

		case SM1_Wait:
			if (button) {
				SM1_State = SM1_Wait;
			}

			else {
				SM1_State = SM1_Start;
			}

			break;
	}

	switch (SM1_State) {
		case (SM1_Start):
			led = (led & 0xF8) | 0x00;
			break;

		case (SM1_PB0):
			led = (led & 0xF8) | 0x01;
			break;

		case (SM1_PB1):
			led = (led & 0xF8) | 0x02;
			break;

		case (SM1_PB2): 
			led = (led & 0xF8) | 0x04;
			break;

		case (SM1_PB11):
			led = (led & 0xF8) | 0x02;
			break;

		case (SM1_PB00):
			led = (led & 0xF8) | 0x01;
			break;

		case (SM1_Wait):
			break;

		}
	
	
	    PORTB = led;
//	    while (!TimerFlag);
//	    TimerFlag = 0;
	    
    

    }
  
}
