/* (c) 2016-06-10 Jens Hauke <jens.hauke@4k2.de>              -*- linux-c -*- */
#ifndef _DELAY_H_
#define _DELAY_H_

#include <util/delay.h>


static inline void delay_us(uint16_t us) __attribute__((always_inline));
void delay_us(uint16_t us) {
	uint16_t loops_1 = us * F_CPU / (3 * 1000000);
	if (!__builtin_constant_p(us) || (loops_1 < 255)) {
		_delay_loop_1(loops_1);
	} else {
		_delay_loop_2(us * F_CPU / (4 * 1000000));
	}
}


static inline void delay_ms(uint16_t ms) __attribute__((always_inline));
void delay_ms(uint16_t ms) {
	delay_us(ms * 1000);
}

#endif /* _DELAY_H_ */
