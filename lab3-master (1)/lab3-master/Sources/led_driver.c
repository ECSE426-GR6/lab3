/**
 * Functions to control and display the led 4 digit 7 segment display as well as the 4 led alarm
 */

#include "led_driver.h"

int digit_mask(int digit);
int led_segments(int value, int* segments);
int write_digit(int value, int digit);
int write_float(float value);
void LED_setPins(void);

//Clock display variables
float current_value = 000.0;

int current_digit = 0;
int digits[4] = {cathode0, cathode1, cathode2, cathode3};
int pins[7] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG};

int digit_segments[4][7] = {
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0}
	};



//CLOCK DISPLAY FUNCTIONS


void LED_update(void){
	HAL_GPIO_WritePin(LED_PORT, digits[current_digit], GPIO_PIN_RESET);
	current_digit = (current_digit + 1) % 4;
	LED_setPins();
	HAL_GPIO_WritePin(LED_PORT, digits[current_digit], GPIO_PIN_SET);
}

void LED_setPins(){
		HAL_GPIO_WritePin(LED_PORT, pinA, digit_segments[current_digit][0] == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, pinB, digit_segments[current_digit][1] == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, pinC, digit_segments[current_digit][2] == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, pinD, digit_segments[current_digit][3] == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, pinE, digit_segments[current_digit][4] == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, pinF, digit_segments[current_digit][5] == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, pinG, digit_segments[current_digit][6] == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);

		if (current_digit == 1)
		{
			HAL_GPIO_WritePin(LED_PORT, pinDP, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(LED_PORT, pinDP, GPIO_PIN_RESET);
		}
}

/**
 * Set the private temp value to be displayed on next refresh
 * @param new_value new value to set temp to
 */
void LED_set_value(float new_value){
	int base_int = (int)(new_value * 10);
	current_value = new_value;

	led_segments(base_int % 10, digit_segments[0]);
	led_segments((base_int / 10) % 10, digit_segments[1]);
	led_segments((base_int / 100) % 10, digit_segments[2]);
	led_segments((base_int / 1000) % 10, digit_segments[3]);
}

/**
 * Get current private temp value
 * @return  current temp
 */
float LED_get_value(void){
	return current_value;
}

/**
 * Returns an array of the segments to switch on ([0] is A, [6] is G)
 * @param  value    int value to convert
 * @param  segments array to store result in
 * @return          error code (0 if no errors)
 */
int led_segments(int value, int* segments){
	int mask = digit_mask(value);
	segments[0] = mask & segA;
	segments[1] = mask & segB;
	segments[2] = mask & segC;
	segments[3] = mask & segD;
	segments[4] = mask & segE;
	segments[5] = mask & segF;
	segments[6] = mask & segG;

	return mask;
}

int digit_mask(int value) {
	if (value == 0) return dig0;
	if (value == 1) return dig1;
	if (value == 2) return dig2;
	if (value == 3) return dig3;
	if (value == 4) return dig4;
	if (value == 5) return dig5;
	if (value == 6) return dig6;
	if (value == 7) return dig7;
	if (value == 8) return dig8;
	if (value == 9) return dig9;

	return -1;
}
