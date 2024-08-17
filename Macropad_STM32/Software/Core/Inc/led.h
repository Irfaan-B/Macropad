#ifndef LED_H
#define LED_H

#include "stm32f1xx.h"

#define NUM_LEDS 12

void delay_ns(uint32_t ns);
void send_bit(uint8_t bit);
void send_reset(void);
void send_color(uint8_t red, uint8_t green, uint8_t blue);
void set_led_color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
void update_leds(void);


#endif