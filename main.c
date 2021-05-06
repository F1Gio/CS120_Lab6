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

static unsigned char led = 0x00;
static unsigned char buttonInc = 0x00;
static unsigned char buttonDec = 0x00;
static unsigned char Tot = 0x00;
enum SM1_States {SM1_Start, SM1_Inc, SM1_Dec} SM1_State;

void IncDec() {
switch(SM1_State) {
		case SM1_Start:

			if (buttonInc) {
				SM1_State = SM1_Inc;
			}

			else if (buttonDec) {
				SM1_State = SM1_Dec;
			}

			else {
				SM1_State = SM1_Start;
			}
			break;

		case SM1_Inc:
			if (buttonInc) {
				SM1_State = SM1_Inc;
			}

			else if (buttonDec){
				SM1_State = SM1_Dec;
			}

			else if (buttonInc && buttonDec) {
				SM1_State = SM1_Start;
			}

			break;

		case SM1_Dec:
			if (buttonDec) {
				SM1_State = SM1_Dec;
			}

			else if (buttonInc){
				SM1_State = SM1_Inc;
			}

			else if (buttonInc && buttonDec) {
				SM1_State = SM1_Start;
			}

			break;

			
	}

	switch (SM1_State) {
		case (SM1_Start):
			Tot = 0;
			led = (led & 0xF8) | 0x00;
			PORTB = led;
			break;

		case (SM1_Inc):

			if (Tot < 9) {
				Tot = Tot + 1;
				led = (led & 0xF0) | Tot;
				PORTB = led;
			}

			break;

		case (SM1_Dec):

			if (Tot > 0) {
				Tot = Tot - 1;
				led = (led & 0xF0) | Tot;
				PORTB = led;
			}
			break;
	}
		
		

}


int main(void) {
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(100);
	TimerOn();
//	unsigned char led = 0x00;
//	unsigned char button = 0x00;
    /* Insert DDR and PORT initializations */

    /* Insert your solution below */
    while (1) {

	buttonInc = ~PINA & 0x01;
	buttonDec = ~PINA & 0x02;
	IncDec();
//	PORTB = led;
	while (!TimerFlag);
	TimerFlag = 0;



    }

}
