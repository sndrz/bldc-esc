/*
	Brushless DC motor electronic speed control
	Operating with brushless DC motors with no hardware sensors.

	main.c

    Copyright (C) 2016-2017 Sergei Ivanov <nsndrz@hotmail.org>
*/

/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* INCLUDES */

#include <avr/interrupt.h>
#include <avr/io.h>

/* DEFINES */

/* Transistors control pins */

#define	U_HI_REG    TCCR1A
#define	U_HI_BIT    COM1A1
#define	V_HI_REG    TCCR1A
#define	V_HI_BIT	COM1A1
#define	W_HI_REG    TCCR1A
#define	W_HI_BIT	COM1A1

#define	LO_DDR	    DDRB
#define	LO_PRT	    PORTB
#define	U_LO_BIT	1
#define	V_LO_BIT	2
#define	W_LO_BIT	3

/* PWM input pin */

#define	IN_PWM_DDR	DDRB
#define	IN_PWM_PRT	PORTB
#define	IN_PWM_BIT	1

#define REVEMF_COUNTER_MAX  200
#define IN_PWM_COUNTER_MAX  255

/* Macros */

#define U_HI_ON     ( U_HI_REG |=  _BV(U_HI_BIT) );
#define U_HI_OFF    ( U_HI_REG &= ~_BV(U_HI_BIT) );
#define V_HI_ON     ( V_HI_REG |=  _BV(V_HI_BIT) );
#define V_HI_OFF    ( V_HI_REG &= ~_BV(V_HI_BIT) );
#define W_HI_ON     ( W_HI_REG |=  _BV(W_HI_BIT) );
#define W_HI_OFF    ( W_HI_REG &= ~_BV(W_HI_BIT) );

#define U_LO_ON     ( LO_PRT |=  _BV(U_LO_BIT) );
#define U_LO_OFF    ( LO_PRT &= ~_BV(U_LO_BIT) );
#define V_LO_ON     ( LO_PRT |=  _BV(V_LO_BIT) );
#define V_LO_OFF    ( LO_PRT &= ~_BV(V_LO_BIT) );
#define W_LO_ON     ( LO_PRT |=  _BV(W_LO_BIT) );
#define W_LO_OFF    ( LO_PRT &= ~_BV(W_LO_BIT) );

#define STOP_MOTOR  U_HI_OFF; V_HI_OFF; W_HI_OFF; U_LO_OFF; V_LO_OFF; W_LO_OFF;

/* VARIABLES */

volatile uint8_t commutation_phase;
volatile uint8_t revemf_counter;
volatile uint8_t in_pwm_counter;
volatile uint8_t in_pwm_result;

/* FUNCTIONS */

void bldc_commutation() {

} /* function bldc_commutation */

void set_rotary_speed(uint8_t in_pwm) {

} /* function set_rotary_speed */

void initialize() {

    /* Set up IO ports */

    LO_DDR |= _BV(U_LO_BIT);
    LO_DDR |= _BV(V_LO_BIT);
    LO_DDR |= _BV(W_LO_BIT);

    IN_PWM_DDR &= ~_BV(IN_PWM_BIT);
    IN_PWM_PRT |=  _BV(IN_PWM_BIT);

    /* Shutdown all motor phases */

    STOP_MOTOR;

    /* Set initial values */

    commutation_phase = 0;
    revemf_counter = 0;
    in_pwm_result = 0;
    in_pwm_counter = 0;

    /* Set up AC for sensor circut */

    /* Disable ADC */
    ADCSRA &= ~(1 << ADEN);
    /* Connect AC negative input to ADCx pins */
    ADCSRB |= (1 << ACME);
    /* Enable AC interruptions */
    ACSR |= (1 << ACIE);

    /* Enable interruptions globally */

    sei();

} /* function initialize */

/* INTERRUPTIONS */

ISR(TIMER0_OVF_vect) {

    bldc_commutation();
    revemf_counter = 0;

} /* interruption TIMER0_OVF_vect */

ISR(ANA_COMP_vect) {

    if (revemf_counter < REVEMF_COUNTER_MAX) {
        revemf_counter++;
    } else {
        bldc_commutation();
    } /* else */

} /* interruption ANA_COMP_vect */

/* MAIN LOOP */

void main() {

    initialize();

    while (1) {

        if ( in_pwm_counter <= IN_PWM_COUNTER_MAX ) {
            in_pwm_result += IN_PWM_PRT;
            in_pwm_counter++;
        } else {
            in_pwm_counter = 0;
            set_rotary_speed(in_pwm_result);
            in_pwm_result = 0;
        } /* else */

    } /* while */

} /* function main */
