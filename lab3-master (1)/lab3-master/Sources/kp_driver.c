#include "kp_driver.h"

int latched = 0;
int latch_ready = 0;
int event_ready = 0;

int latch_x = 0;
int latch_y = 0;
int latch_count = 0;
int event_value = 0;

int cols[3] = {0,0,0};
int rows[4] = {0,0,0,0};

int state[4][3] = {
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0}
};

int values[4][3] = {
	{1, 2, 3},
	{4, 5, 6},
	{7, 8, 9},
	{10, 0, 11}
};

void update_not_latched(void);
void update_latched(void);
void latch_count_update(void);

/**
* Scanning the keypad and keeping track of the button state
*/
void KP_update(void){
	

	cols[0] = HAL_GPIO_ReadPin(KP_PORT, COL_0_PIN);
	cols[1] = HAL_GPIO_ReadPin(KP_PORT, COL_1_PIN);
	cols[2] = HAL_GPIO_ReadPin(KP_PORT, COL_2_PIN);

	rows[0] = HAL_GPIO_ReadPin(KP_PORT, ROW_0_PIN);
	rows[1] = HAL_GPIO_ReadPin(KP_PORT, ROW_1_PIN);
	rows[2] = HAL_GPIO_ReadPin(KP_PORT, ROW_2_PIN);
	rows[3] = HAL_GPIO_ReadPin(KP_PORT, ROW_3_PIN);

	if (latched) update_latched();
	else update_not_latched();

}

// When not currently devouncing a key, check for a new keypress
void update_not_latched(void){
	int x = 0;
	int y = 0;

	while (x < 3){
		if (cols[x]){
			while (y < 4){
				if (rows[y]) {
					latched = 1;
					latch_ready = 0;
					latch_x = x;
					latch_y = y;
					latch_count = 0;
				}
				y++;
			}
		}
		x++;
	}
}

//When deboncing keys, check if stable and released
void update_latched(void){
	latch_count_update();

	//Stable high signal
	if (latch_count > LATCH_CONFIRM_COUNT) {
		latch_ready = 1;
	}

	//Sable low before propper latch (not registered as keypress)
	if (latch_count < -1 * LATCH_CONFIRM_COUNT) {
		latched = 0;
	}

	//Stable high detected, waiting for key release
	if (latch_ready) {
		if (!cols[latch_x]){
			if (!rows[latch_y]){
				latched = 0;
				event_ready = 1;
				event_value = values[latch_y][latch_x];
			}
		}
	}
}
//Update the latch count (add if hig, decrease if low)
void latch_count_update(void){
	if (cols[latch_x]) {
		if (rows[latch_y]){
			latch_count ++;
			return;
		}
	} else {
		latch_count--;
	}
}

/**
* Get previously latched key 
* Returns the last button that was clicked (pressed, stabilized and released)
*/

int KP_getEvent(void){
	if (event_ready){
		event_ready = 0;
		return event_value;
	}

	return -1;
}

/**
* Get value of key pressed down
*/
int KP_getValueDown(void){
	int i = 0;
	int x = -1;
	int y = -1;

	while (i < 3){
		if (cols[i]){
			x = i;
			break;
		}
		i++;
	}

	if (x == -1) return -1;

	i = 0;

	while (i < 4) {
		if (rows[i]) {
			y = i;
			break;
		}
		i++;
	}

	if (y == -1) return -1;

	return values[y][x];
}
