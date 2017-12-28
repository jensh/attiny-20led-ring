// (c) 2016-06-10 Jens Hauke <jens.hauke@4k2.de>

#include "led_map.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

static
uint8_t led_map[LED_MAP_ROWS] = {0 , };

static
uint8_t led_crow = 0; // current row


// Set (PB5 PB4 PB3 PB2 PB1 PB0) to lo, hi or tri-state
static
void set_tri(uint8_t lo_hi, uint8_t in_out) {
	uint8_t portb, ddrb;
	// Keep the state of the unused pins
#define USED_BITS ((1 << LED_MAP_ROWS) - 1)
	ddrb = (DDRB & (~USED_BITS)) | (in_out & USED_BITS); // 1: Output, 0: Input
	portb = (PORTB & (~USED_BITS)) | (lo_hi & USED_BITS); // 0: lo, 1: hi

	// Set registers
	PORTB = portb;
	DDRB = ddrb;
}


static
void led_crow_inc(void) {
	led_crow++;
	if (led_crow > LED_MAP_ROWS) led_crow = 0;
}


void led_map_step(void) {
	uint8_t cnt = 0;
	uint8_t lmap;
	uint8_t lopin_mask;
	while (!((lmap = led_map[led_crow]))) {
		if (cnt++ >= LED_MAP_ROWS) {
			 // empty map. Switch off.
			set_tri(0, 0);
			return;
		}
		led_crow_inc();
	}
	lopin_mask = 1 << led_crow;
	set_tri(lmap & ~lopin_mask, lmap | lopin_mask);
	led_crow_inc();
}


//    0  1  2  3  4  5  6  7
// 0  x  0  1  2  3  4  5  6
// 1  7  x  8  9 10 11 12 13
// 2 14 15  x 16 17 18 19 20
// 3 21 22 23  x 24 25 26 27
// 4 28 29 30 31  x 32 33 34
// 5 35 36 37 38 39  x 40 41
// 6 42 43 44 45 46 47  x 48
// 7 49 50 51 52 53 54 55  x

//    0  1  2  3  4  .  .  .
// 0  x  0  1  2  3  .  .  .
// 1  4  x  5  6  7  .  .  .
// 2  8  9  x 10 11  .  .  .
// 3 12 13 14  x 15  .  .  .
// 4 16 17 18 19  x  .  .  .
// 5  .  .  .  .  .  x  .  .
// 6  .  .  .  .  .  .  x  .
// 7  .  .  .  .  .  .  .  x


static
uint8_t trans_led(uint8_t led) {
	static const unsigned char led_mapping[20] PROGMEM = {
		2, 16,  7,  9, 14, 19, 11,  1,  4, 13,
		6,  0,  8, 18, 15, 10,  5, 17,  3, 12
	};
	return pgm_read_byte(led_mapping + led);
}


void led_map_set(uint8_t led, uint8_t on) {
	led = trans_led(led);
	uint8_t lo_bit = (led / (LED_MAP_ROWS - 1)) % LED_MAP_ROWS;
	uint8_t hi_bit = (led) % (LED_MAP_ROWS - 1);
	if (hi_bit >= lo_bit) hi_bit++;

	// bit pos to bitmask:
	hi_bit = 1 << hi_bit;

	if (on) {
		led_map[lo_bit] |= hi_bit;
	} else {
		led_map[lo_bit] &= ~(hi_bit);
	}
}


uint8_t led_map_get(uint8_t led) {
	led = trans_led(led);

	uint8_t lo_bit = (led / (LED_MAP_ROWS - 1)) % LED_MAP_ROWS;
	uint8_t hi_bit = (led) % (LED_MAP_ROWS - 1);
	if (hi_bit >= lo_bit) hi_bit++;

	// bit pos to bitmask:
	hi_bit = 1 << hi_bit;
	return led_map[lo_bit] & hi_bit;
}


void led_map_clear(void) {
	uint8_t i;
	for (i = 0; i < LED_MAP_ROWS; i++) led_map[i] = 0;
}
