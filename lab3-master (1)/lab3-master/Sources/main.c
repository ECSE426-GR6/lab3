/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include <Math.h>
#include <stdio.h>
#include "lis3dsh.h"
#include "led_driver.h"
#include "kp_driver.h"
#include "kfilter.h"
/* Private variables ---------------------------------------------------------*/
// PI used for angle calculation
#define PI 3.14159265359
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void tim_init(void);
void gpio_init(void);
float change_digit(float number, int digit, int new_val);
void memsinit(void);
float Rangle(void);
float calculateangle(float x, float y, float z);
//Data ready interrupt flag
int data_ready = 0;

int main(void)
{	
  //VARIABLES
  int input = 1; //Input state

  //Input variables
  float num = 0.0f;
  int event = -1;
  int digit = 0;

  //Angle variables
  float angle_value = 0.0f;

  /* MCU Configuration----------------------------------------------------------*/
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
k_filter_init(0.005f, 0.1f, 0.0f, 3.0f, 0.0f);
	tim_init();
	gpio_init();
  memsinit();
	
    NVIC_SetPriority(EXTI0_IRQn, 1); //4. Set Priority to 1
    NVIC_EnableIRQ(EXTI0_IRQn);

	LED_set_value(num);

	while (1){

    if (input) { //Input mode
      KP_update();
      event = KP_getEvent();
      if (event == 11) { //# pressed
        input = 0;
				LED_set_target(num);
      }else if (event > -1 && event < 10) { //Other key pressed
        num = change_digit(num, digit, event);
        LED_set_value(num);
        digit = (digit + 1) % 4;
      }
    } else { //Playing mode
        if (data_ready) {
          //CONVERT ANGLE READING
					//set LED to the filtered angle value
  				LED_set_value(k_filter_value(Rangle()));
          data_ready = 0;
        }
    }
	}
}

//Helper function to change one digit of a floating point number (used in input mode)
float change_digit(float number, int digit, int new_val){
  int base_int = (int)(number * 10);

  int dec = base_int % 10;
  int one = (base_int / 10) % 10;
  int ten = (base_int / 100) % 10;
  int hundred = (base_int / 1000) % 10;

  if (digit == 3) {
    float decimal = ((float)dec) / 10;
    number -= decimal;
    decimal = ((float)new_val) / 10;
    number += decimal;
    return number;
  }
  if (digit == 2) {
    return number - one + new_val;
  }
  if (digit == 1) {
    return number - (ten * 10) + (new_val * 10);
  }
  if (digit == 0) {
    return number - (hundred * 100) + (new_val * 100);
  }

  return -1;
}

void memsinit(void){

  LIS3DSH_InitTypeDef meminit;
  LIS3DSH_DRYInterruptConfigTypeDef intinit;

  meminit.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;
  meminit.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;
  meminit.AA_Filter_BW = LIS3DSH_AA_BW_800;
  meminit.Axes_Enable = LIS3DSH_XYZ_ENABLE;
  meminit.Full_Scale = LIS3DSH_FULLSCALE_2;
  meminit.Self_Test = LIS3DSH_SELFTEST_NORMAL;
  
  LIS3DSH_Init(&meminit);

  intinit.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
  intinit.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
  intinit.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
  
  LIS3DSH_DataReadyInterruptConfig(&intinit);

  
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_pin){
  
	if (GPIO_pin == GPIO_PIN_0){
		data_ready = 1;
	}
}

//Rangle function to read the data from the sensor
float Rangle(){

float xyz[3];
float xangle;
	float yangle;
	float zangle;
	 LIS3DSH_ReadACC(xyz);
	
	// X offset ~= -19
	// Y offset ~=  +12
	// Z offset ~=  -40
	// the offset is added to the read value for calibration
	
	 xangle = xyz[0] -19;
	 yangle = xyz[1] +12;
		zangle = xyz[2] -40 ;
	

	
	
	return calculateangle(xangle,yangle,zangle);
	
}

//the calculate angle function to compute the pitch and roll angles
float calculateangle(float x, float y, float z){
float pitch;
float roll;
float angle;

 pitch = atan(x / (sqrt(y*y+z*z)))*(180/PI) +90 ;
		printf("%f\n", pitch);
// + 90 degrees is added to each angle to demonstrate that the sensor works both directions.
	// cannot display negative
	roll = atan(y / (sqrt(x*x+z*z)))*(180/PI) +90 ;
	
	return angle = pitch;
}
/** System Clock Configuration*/
void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 				= 8;
  RCC_OscInitStruct.PLL.PLLN 				= 336;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts. SysTick is already 
	  configured inside HAL_Init, I don't kow why the CubeMX generates this call again*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


TIM_HandleTypeDef TIM_Handle;
void tim_init(void)
{
    __TIM3_CLK_ENABLE();
		TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4; // 168 MHz / 4 = 42 MHz
    TIM_Handle.Init.Prescaler = 499; // 42 MHz / (419 + 1) = 100 KHz
    TIM_Handle.Init.Period = 499; // 100 KHz / (499 + 1) = 200 Hz
    TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM_Handle.Instance = TIM3;   //Same timer whose clocks we enabled
    HAL_TIM_Base_Init(&TIM_Handle);     // Init timer
    HAL_TIM_Base_Start_IT(&TIM_Handle); // start timer interrupts
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    
}


void TIM3_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&TIM_Handle, TIM_FLAG_UPDATE) != RESET)      //In case other interrupts are also running
    {
        if (__HAL_TIM_GET_ITSTATUS(&TIM_Handle, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_CLEAR_FLAG(&TIM_Handle, TIM_FLAG_UPDATE);
            LED_update();
        }
    }
}


void gpio_init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* GPIO Ports Clock Enable */
  
  __GPIOA_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOE_CLK_ENABLE();
  //__GPIOC_CLK_ENABLE();
  //__GPIOH_CLK_ENABLE();
  //__GPIOB_CLK_ENABLE();

	
	

	
/*Configure GPIOA pins */
  ///3x4 Keypad
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

/*Configure GPIOD pins */
	///7 segment display
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


/*Configure GPIOe pins*/
  ///Accelerometer data ready interrupt
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}


#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line){
}
#endif
