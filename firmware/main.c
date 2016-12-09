/*
	Brushless motor electronic speed control
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

#include <avr/interrupt.h>
#include <avr/io.h>
#include "avr-toolbox/atb_bldc.h"

void mcu_initialize() {

    /* Enable interruptions globally */

    sei();

} /* function mcu_initialize */

ISR(TIMER0_OVF_vect) {
    atb_bldc_no_reverse_emf_timer_ovf();
} /* interruption TIMER0_OVF_vect */

ISR(ANA_COMP_vect) {
    atb_bldc_reverse_emf_ac_ovf();
} /* interruption ANA_COMP_vect */

void main() {

    mcu_initialize();
    atb_bldc_initialize();

} /* function main */
