#include "kp_driver.h"

int cols[3] = {0,0,0};
int rows[4] = {0,0,0,0};

int numbers[4][3] = {
	{1, 2, 3},
	{4, 5, 6},
	{7, 8, 9},
	{-2, 0, -3}
};

void KP_update(void){
	cols[0] = HAL_GPIO_ReadPin(GPIOA, COL_0_PIN);
	cols[1] = HAL_GPIO_ReadPin(GPIOA, COL_1_PIN);
	cols[2] = HAL_GPIO_ReadPin(GPIOA, COL_2_PIN);

	rows[0] = HAL_GPIO_ReadPin(GPIOA, ROW_0_PIN);
	rows[1] = HAL_GPIO_ReadPin(GPIOA, ROW_1_PIN);
	rows[2] = HAL_GPIO_ReadPin(GPIOA, ROW_2_PIN);
	rows[3] = HAL_GPIO_ReadPin(GPIOA, ROW_3_PIN);
}

int KP_getValue(void){
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

	return numbers[y][x];
}
