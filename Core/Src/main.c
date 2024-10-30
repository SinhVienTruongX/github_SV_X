/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define StepPerRev 200
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define IN1_PIN GPIO_PIN_4
#define IN1_PORT GPIOA
#define IN2_PIN GPIO_PIN_3
#define IN2_PORT GPIOA
#define IN3_PIN GPIO_PIN_2
#define IN3_PORT GPIOA
#define IN4_PIN GPIO_PIN_1
#define IN4_PORT GPIOA
const uint8_t STEP_SEQUENCE[] = {0x0A, 0x06, 0x05, 0x09};
uint32_t micro_step_arry_1[8] = {20,38,56,70,83,92,98,100};
														  //95.6   98   99.5   100
uint32_t micro_step_arry_2[8] = {98,92,83,70,56,38,20,0};
															 //56    47    38    29    20    9.8  0 
uint8_t dem = 0;
void delay (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while (__HAL_TIM_GET_COUNTER(&htim2) < us);
}
void control_pwm(TIM_HandleTypeDef *timer, uint32_t channel, 
                uint32_t *pwm_values, float delay_time) {
    for(int i = 0; i < 8; i++) {
        __HAL_TIM_SET_COMPARE(timer, channel, pwm_values[i]);
        __HAL_TIM_SET_COUNTER(&htim1, 0);
        while(__HAL_TIM_GET_COUNTER(&htim1) < delay_time) {}
    }
}
/*-------------------------Set speed for motor--------------------------------------*/
void stepper_set_rpm (int rpm) 
{
	delay(60000000/StepPerRev/rpm);
}
void stepper_half_set_rpm (int rpm) 
{
	delay(60000000/(StepPerRev*2)/rpm);
}
/*-------------------------Set step---------------------------------------*/
void stepper_full_drive_1phaON (int step)
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 100);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100);
	switch (step)
	{
		case 0:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
			break;
		case 1:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
			break;
		case 2:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);   // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
			break;
		case 3:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
			break;
	}
}
void stepper_full_drive_2phaON (int step)
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 100);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100);
	switch (step){
		case 0:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
			break;
		case 1:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
			break;
		case 2:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET); // IN4
			break;
		case 3:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
			break;
	}
}
void stepper_half_drive (int step)
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 100);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100);
	switch (step){
		case 0:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
			break;
		case 1:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
			break;
		case 2:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
			break;
		case 3:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
			break;
		case 4:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
			break;
		case 5:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
			break;
		case 6:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
			break;
		case 7:
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);   // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);   // IN4
			break;
	}
}
void micro_step(float rpm)
{
    float t = 60000000/(StepPerRev*8)/rpm; 
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100);
        
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);  // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);    // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);  // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);    // IN4
									
			for(int i = 0; i < 8; i++) {
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, micro_step_arry_1[i]); //Tang 3
					delay(t);
			}
			
			for(int i = 0; i < 8; i++) {
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, micro_step_arry_2[i]); //Giam 1
					delay(t);
			}
			
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET);  // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);    // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);  // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);    // IN4
					 
			for(int i = 0; i < 8; i++) {
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, micro_step_arry_1[i]); //Tang 2
					delay(t);
			}
		 
			for(int i = 0; i < 8; i++) {
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, micro_step_arry_2[i]); //Giam 3
					delay(t);
			}
           
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET);  // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);    // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);  // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);    // IN4
								 
			for(int i = 0; i < 8; i++) {
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, micro_step_arry_1[i]); //Tang 4
					delay(t);
			}
			
			for(int i = 0; i < 8; i++) {
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, micro_step_arry_2[i]);
					delay(t);
			}
              
			HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);    // IN1
			HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);  // IN2
			HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);  // IN3
			HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);    // IN4
						
			for(int i = 0; i < 8; i++) {
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, micro_step_arry_1[i]);
					delay(t);
			}
			
			for(int i = 0; i < 8; i++) {
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, micro_step_arry_2[i]);
					delay(t);
			}
}
void micro_step_2(int step, float rpm)
{
    float t = 60000000/(StepPerRev*8)/rpm; 
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100);

    switch(step){
      case 0:            
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);  // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);    // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);  // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);    // IN4
										
				for(int i = 0; i < 8; i++) {
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, micro_step_arry_1[i]); //Tang 3
						delay(t);
				}
				
				for(int i = 0; i < 8; i++) {
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, micro_step_arry_2[i]); //Giam 1
						delay(t);
				}
				break;

      case 1:              
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);   // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);   // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
						 
				for(int i = 0; i < 8; i++) {
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, micro_step_arry_1[i]); //Tang 2
						delay(t);
				}
			 
				for(int i = 0; i < 8; i++) {
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, micro_step_arry_2[i]); //Giam 3
						delay(t);
				}
				break;

      case 2:             
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET);  // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);    // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);  // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);    // IN4
									 
				for(int i = 0; i < 8; i++) {
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, micro_step_arry_1[i]); //Tang 4
						delay(t);
				}
				
				for(int i = 0; i < 8; i++) {
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, micro_step_arry_2[i]);
						delay(t);
				}
				break;
						
      case 3:              
				HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);    // IN1
				HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);  // IN2
				HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);  // IN3
				HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);    // IN4
							
				for(int i = 0; i < 8; i++) {
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, micro_step_arry_1[i]);
						delay(t);
				}
				
				for(int i = 0; i < 8; i++) {
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, micro_step_arry_2[i]);
						delay(t);
				}
				break;
    }  
}
void motorOff()
{
    // Switch off the idle current to the motor
    // Otherwise L298N module will heat up
    HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET); // IN1
    HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET); // IN2
    HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET); // IN3
    HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET); // IN4
}
/*------------------------Control motor revolutions-----------------------*/
void stepper_step_angle_full_1phaON(float angle, int direction, int rpm) // direction -> 0 for CW, 1 for CCW
{
    float step_angle = 1.8; 
    int total_steps = (int)(angle / step_angle); 
    
    for (int step = 0; step < total_steps; step++) 
    {
        if (direction == 0)  //CW
        {
            stepper_full_drive_1phaON(step % 4); 
        }
        else if (direction == 1)  // CCW
        {
            stepper_full_drive_1phaON(3 - (step % 4)); 
        }
        
        stepper_set_rpm(rpm);
    }
}
void stepper_step_angle_full_2phaON(float angle, int direction, int rpm) // direction -> 0 for CW, 1 for CCW
{
    float step_angle = 1.8; 
    int total_steps = (int)(angle / step_angle); 
    
    for (int step = 0; step < total_steps; step++) 
    {
        if (direction == 0)  //CW
        {
            stepper_full_drive_2phaON(step % 4); 
        }
        else if (direction == 1)  // CCW
        {
            stepper_full_drive_2phaON(3 - (step % 4)); 
        }
        
        stepper_set_rpm(rpm);
    }
}
void stepper_step_angle_half(float angle, int direction, int rpm) // direction -> 0 for CW, 1 for CCW
{
    float step_angle = 0.9; 
    int total_steps = (int)(angle / step_angle); 
    
    for (int step = 0; step < total_steps; step++) 
    {
        if (direction == 0)  //CW
        {
            stepper_half_drive(step % 8); 
        }
        else if (direction == 1)  // CCW
        {
            stepper_half_drive(7 - (step % 8)); 
        }
        
        stepper_half_set_rpm(rpm);
    }
}
void stepper_step_angle_micro(float angle, int direction, int rpm) // direction -> 0 for CW, 1 for CCW
{
    float step_angle = 1.8; 
    int total_steps = (int)(angle / step_angle); 
    
    for (int step = 0; step < total_steps; step++) 
    {
        if (direction == 0)  //CW
        {
            micro_step_2(step % 4, rpm); 
        }
        else if (direction == 1)  // CCW
        {
            micro_step_2((3 - (step % 4)), rpm); 
        }
        
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_Base_Start(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		micro_step(400);
//		stepper_step_angle_half(180, 0, 100);
//		HAL_Delay(2000);
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 720-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 100 - 1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xffff - 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
