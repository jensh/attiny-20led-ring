/*
 * Copyright (c) 2016,2017 Jens Hauke. All rights reserved.
 *
 * 2016-06-10 Jens Hauke <jens@4k2.de>
 * 2017-05-31 Jens Hauke <jens@4k2.de>
 */
//
// With (PB5 PB4 PB3 PB2 PB1 PB0) (ATTiny)
// build a lo, hi, tri-state matrix.
//
// Powerdown after UPTIME_MAX xxx (~xx sec). Press reset to wake up.
//
// ATMEL ATTINY13a
//
//       +-\/-+
// PB5  1|    |8  Vcc
// PB3  2|    |7  PB2
// PB4  3|    |6  PB1
// GND  4|    |5  PB0
//       +----+

#include "led_map.h"
#include "power_down.h"
#include "delay.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
// #include "bootloader.h"

#define STEP_LENGTH  800
#define UPTIME_SECONDS(s) (s /* s */ * 1000000 / STEP_LENGTH)
#define UPTIME_MAX UPTIME_SECONDS(60)

#define BUTTON_RELEASED(_) (PINB & (1 << 5))


static
uint32_t uptime = 0;

static inline
void setup(void) {
//	MCUSR = 0;	// reset watchdog
	PORTB = _BV(5);			// All input with pull up on PB5
	DDRB = 0x00;			// Input
	delay_ms(10);	// wait for stable PB5
}


static const unsigned char period_length[] PROGMEM = {
	21, 22, 23, 24
};
#define N_ID (sizeof(period_length)/sizeof(period_length[0]))


typedef struct {
	unsigned char period;
	unsigned char led;
} animation_state_t;

animation_state_t animation_state[N_ID];

void animation_step(uint8_t id) {
	animation_state_t *s = animation_state + id;
	s->period++;

	if (s->period < pgm_read_byte(period_length + id)) {
		return;
	}
	s->period = 0;

	led_map_set(s->led, !led_map_get(s->led));

	s->led = (s->led + 1) % LED_COUNT;
}


static inline
void uptime_step(void) {
	if (++uptime > UPTIME_MAX) {
		do {
			power_down();

			// Wait for high
		} while (!BUTTON_RELEASED());

		// Debounce
		delay_ms(50);
		uptime = 0;
	}
}


static inline
void comm_step(void) {
	if (!BUTTON_RELEASED()) { // && (uptime > UPTIME_SECONDS(2))) {
		// power down in 0.2 sec or earlier
		uint32_t newuptime = UPTIME_MAX - UPTIME_SECONDS(0.2);
		if (newuptime > uptime) {
			uptime = newuptime;
		}
	}
}


static inline
void loop() {
	unsigned id;
	led_map_step();
	for (id = 0; id < N_ID; id++) {
		animation_step(id);
	}

	uptime_step();
	comm_step();
	delay_us(STEP_LENGTH);
}


int main(void)
{
	setup();
	while (1) loop();
	return 0;
}

/*
 *  compile-command: "make sflash@led_circle"
 * Local Variables:
 *  compile-command: "make flash@led_circle"
 * End:
 */
