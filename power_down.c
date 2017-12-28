// (c) 2016-06-10 Jens Hauke <jens.hauke@4k2.de>
#include "power_down.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>


void power_down(void)
{
	PORTB = _BV(5);			// All input with pull up on PB5
	DDRB = 0x00;			// Input

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//	wdt_disable();	// No wakeup from watchdog

#if 1
	// Wakeup from Pin changes
	PCMSK = _BV(5);			// Pin Change Enable Mask 5:0
	GIMSK |= _BV(PCIE);		// Enable PC interrupts
	GIFR |= _BV(PCIF);		// Clear PC Interrupt Flag
#endif
//	uint8_t adcsra = ADCSRA;	//save ADCSRA
	ADCSRA &= ~_BV(ADEN);		//disable ADC. Important!

	// Sleep with BOD disabled (see avr/sleep.h)
	cli();
	if (1) {
		sleep_enable();
		sleep_bod_disable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
	sei();

//	ADCSRA = adcsra;		//restore ADCSRA
//	wdt_enable(WDTO_4S);		// enable watchdog 4s
}


ISR(INT0_vect)
{
}

ISR_ALIAS(PCINT0_vect, INT0_vect);
ISR_ALIAS(WDT_vect, INT0_vect);
