#include "stm32f4xx_hal.h"

#define LATCH_CONFIRM_COUNT 100

#define KP_PORT GPIOE

#define COL_0_PIN GPIO_PIN_13
#define COL_1_PIN GPIO_PIN_14
#define COL_2_PIN GPIO_PIN_15

#define ROW_0_PIN GPIO_PIN_9
#define ROW_1_PIN GPIO_PIN_10
#define ROW_2_PIN GPIO_PIN_11
#define ROW_3_PIN GPIO_PIN_12

void KP_update(void);
int KP_getValueDown(void);
int KP_getEvent(void);
