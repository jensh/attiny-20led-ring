/* (c) 2016-06-10 Jens Hauke <jens.hauke@4k2.de>              -*- linux-c -*- */
#ifndef _LED_MAP_H_
#define _LED_MAP_H_

#include <inttypes.h>

#define LED_MAP_ROWS 5
#define LED_COUNT (LED_MAP_ROWS * (LED_MAP_ROWS - 1))


void led_map_step(void);
void led_map_set(uint8_t led, uint8_t on);
uint8_t led_map_get(uint8_t led);



#endif /* _LED_MAP_H_ */
