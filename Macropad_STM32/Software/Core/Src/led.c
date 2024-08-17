#include "led.h"

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} LED_Color;

LED_Color led_colors[NUM_LEDS];

void delay_ns(uint32_t cycles) {
    while (cycles--) {
        __NOP();
    }
}

void send_bit(uint8_t bit) {
    if (bit) {
        // Send '1' code
        GPIOC->BSRR = GPIO_PIN_13; // Set pin high
        delay_ns(3); // High for 500ns - 416ns = 84ns (adjusted to 120ns for safety)
        GPIOC->BRR = GPIO_PIN_13; // Set pin low
        delay_ns(1); // Low for 500ns
    } else {
        // Send '0' code
        GPIOC->BSRR = GPIO_PIN_13; // Set pin high
        delay_ns(1); // High for 500ns
        GPIOC->BRR = GPIO_PIN_13; // Set pin low
        delay_ns(3); // Low for 800ns
    }
}

void send_reset() {
    GPIOC->BRR = GPIO_PIN_13; // Set pin low
    delay_ns(80000); // Low for min 80us
}

void send_color(uint8_t red, uint8_t green, uint8_t blue) {
    for (int i = 7; i >= 0; i--) {
        send_bit((green >> i) & 1);
    }
    for (int i = 7; i >= 0; i--) {
        send_bit((red >> i) & 1);
    }
    for (int i = 7; i >= 0; i--) {
        send_bit((blue >> i) & 1);
    }
}

void set_led_color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) {
    if (index < NUM_LEDS) {
        led_colors[index].red = red;
        led_colors[index].green = green;
        led_colors[index].blue = blue;
    }
}

void update_leds(void) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        send_color(led_colors[i].red, led_colors[i].green, led_colors[i].blue);
    }
    send_reset();
}