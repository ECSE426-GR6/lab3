#include "stm32f4xx_hal.h"

#define DIGIT_SWITCH_DELAY 1

#define LED_PORT GPIOD

#define pinA GPIO_PIN_0
#define pinB GPIO_PIN_1
#define pinC GPIO_PIN_2
#define pinD GPIO_PIN_3
#define pinE GPIO_PIN_4
#define pinF GPIO_PIN_5
#define pinG GPIO_PIN_6

#define pinDP GPIO_PIN_7

#define cathode0 GPIO_PIN_11
#define cathode1 GPIO_PIN_10
#define cathode2 GPIO_PIN_9
#define cathode3 GPIO_PIN_8

#define segA 0x0001
#define segB 0x0002
#define segC 0x0004
#define segD 0x0008
#define segE 0x0010
#define segF 0x0020
#define segG 0x0040

#define dig0 0x3f
#define dig1 0x06
#define dig2 0x5b
#define dig3 0x4f
#define dig4 0x66
#define dig5 0x6d
#define dig6 0x7d
#define dig7 0x07
#define dig8 0x7f
#define dig9 0x67


void LED_update(void);
void LED_set_value(float);
float LED_get_value(void);
