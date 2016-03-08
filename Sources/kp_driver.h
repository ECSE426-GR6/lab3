#include "stm32f4xx_hal.h"

#define COL_0_PIN GPIO_PIN_4
#define COL_1_PIN GPIO_PIN_5
#define COL_2_PIN GPIO_PIN_7

#define ROW_0_PIN GPIO_PIN_0
#define ROW_1_PIN GPIO_PIN_1
#define ROW_2_PIN GPIO_PIN_2
#define ROW_3_PIN GPIO_PIN_3

void KP_update(void);
int KP_getValue(void);
