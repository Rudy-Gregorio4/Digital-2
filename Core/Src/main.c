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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t contador1 = 0;
uint8_t contador2 = 0;
uint8_t estadoInicio = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(HAL_GPIO_ReadPin(BOTON_INICIO_GPIO_Port, BOTON_INICIO_Pin) == 0){
		  estadoInicio = 1;
		  HAL_GPIO_WritePin(LUZ_ROJA_GPIO_Port, LUZ_ROJA_Pin, GPIO_PIN_SET);
		  HAL_Delay(1000);
		  HAL_GPIO_WritePin(LUZ_ROJA_GPIO_Port, LUZ_ROJA_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LUZ_AMARILLA_GPIO_Port, LUZ_AMARILLA_Pin, GPIO_PIN_SET);
		  HAL_Delay(1000);
		  HAL_GPIO_WritePin(LUZ_AMARILLA_GPIO_Port, LUZ_AMARILLA_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LUZ_VERDE_GPIO_Port, LUZ_VERDE_Pin, GPIO_PIN_SET);
		  HAL_Delay(1000);
	  }
	  while(estadoInicio == 1){

		  if(HAL_GPIO_ReadPin(BOTON1_GPIO_Port, BOTON1_Pin) == 0){
			  contador1 = contador1 + 1;
			  HAL_Delay(250);
		  }
		  if(HAL_GPIO_ReadPin(BOTON2_GPIO_Port, BOTON2_Pin) == 0){
			  contador2 = contador2 + 1;
			  HAL_Delay(250);
		  }

		  switch (contador1) {
			case 1:
				HAL_GPIO_WritePin(LED1_A_GPIO_Port, LED1_A_Pin, GPIO_PIN_SET);
				break;
			case 2:
				HAL_GPIO_WritePin(LED1_A_GPIO_Port, LED1_A_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED1_B_GPIO_Port, LED1_B_Pin, GPIO_PIN_SET);
				break;
			case 3:
				HAL_GPIO_WritePin(LED1_B_GPIO_Port, LED1_B_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED1_C_GPIO_Port, LED1_C_Pin, GPIO_PIN_SET);
				break;
			case 4:
				HAL_GPIO_WritePin(LED1_C_GPIO_Port, LED1_C_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED1_D_GPIO_Port, LED1_D_Pin, GPIO_PIN_SET);
				break;
			case 5:
				HAL_GPIO_WritePin(LED1_D_GPIO_Port, LED1_D_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED1_E_GPIO_Port, LED1_E_Pin, GPIO_PIN_SET);
				break;
			case 6:
				HAL_GPIO_WritePin(LED1_E_GPIO_Port, LED1_E_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED1_F_GPIO_Port, LED1_F_Pin, GPIO_PIN_SET);
				break;
			case 7:
				HAL_GPIO_WritePin(LED1_F_GPIO_Port, LED1_F_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED1_G_GPIO_Port, LED1_G_Pin, GPIO_PIN_SET);
				break;
			case 8:
				HAL_GPIO_WritePin(LED1_G_GPIO_Port, LED1_G_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED1_H_GPIO_Port, LED1_H_Pin, GPIO_PIN_SET);
				break;
		}

		  switch (contador2) {
		  	 case 1:
		  		HAL_GPIO_WritePin(LED2_A_GPIO_Port, LED2_A_Pin, GPIO_PIN_SET);
		  		break;
		  	 case 2:
		  		HAL_GPIO_WritePin(LED2_A_GPIO_Port, LED2_A_Pin, GPIO_PIN_RESET);
		  		HAL_GPIO_WritePin(LED2_B_GPIO_Port, LED2_B_Pin, GPIO_PIN_SET);
		  		break;
		     case 3:
		    	HAL_GPIO_WritePin(LED2_B_GPIO_Port, LED2_B_Pin, GPIO_PIN_RESET);
		  		HAL_GPIO_WritePin(LED2_C_GPIO_Port, LED2_C_Pin, GPIO_PIN_SET);
		  		break;
		  	 case 4:
		  		HAL_GPIO_WritePin(LED2_C_GPIO_Port, LED2_C_Pin, GPIO_PIN_RESET);
		  		HAL_GPIO_WritePin(LED2_D_GPIO_Port, LED2_D_Pin, GPIO_PIN_SET);
		  		break;
		  	 case 5:
		  		HAL_GPIO_WritePin(LED2_D_GPIO_Port, LED2_D_Pin, GPIO_PIN_RESET);
		  		HAL_GPIO_WritePin(LED2_E_GPIO_Port, LED2_E_Pin, GPIO_PIN_SET);
		  		break;
		  	 case 6:
		  		HAL_GPIO_WritePin(LED2_E_GPIO_Port, LED2_E_Pin, GPIO_PIN_RESET);
		  		HAL_GPIO_WritePin(LED2_F_GPIO_Port, LED2_F_Pin, GPIO_PIN_SET);
		  		break;
		  	 case 7:
		  		HAL_GPIO_WritePin(LED2_F_GPIO_Port, LED2_F_Pin, GPIO_PIN_RESET);
		  		HAL_GPIO_WritePin(LED2_G_GPIO_Port, LED2_G_Pin, GPIO_PIN_SET);
		  		break;
		  	 case 8:
		  		HAL_GPIO_WritePin(LED2_G_GPIO_Port, LED2_G_Pin, GPIO_PIN_RESET);
		  		HAL_GPIO_WritePin(LED2_H_GPIO_Port, LED2_H_Pin, GPIO_PIN_SET);
		  		break;
		  }

		  if (contador1 >= 9) {
			contador2 = 0;
			estadoInicio = 0;
			HAL_GPIO_WritePin(LUZ_GANA2_GPIO_Port, LUZ_GANA2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED2_A_GPIO_Port, LED2_A_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_B_GPIO_Port, LED2_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_C_GPIO_Port, LED2_C_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_D_GPIO_Port, LED2_D_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_E_GPIO_Port, LED2_E_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_F_GPIO_Port, LED2_F_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_G_GPIO_Port, LED2_G_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_H_GPIO_Port, LED2_H_Pin, GPIO_PIN_RESET);
		  }

		  if (contador2 >= 9) {
		 	contador1 = 0;
		 	estadoInicio = 0;
		 	HAL_GPIO_WritePin(LUZ_GANA1_GPIO_Port, LUZ_GANA1_Pin, GPIO_PIN_SET);
		 	HAL_GPIO_WritePin(LED1_A_GPIO_Port, LED1_A_Pin, GPIO_PIN_RESET);
		 	HAL_GPIO_WritePin(LED1_B_GPIO_Port, LED1_B_Pin, GPIO_PIN_RESET);
		 	HAL_GPIO_WritePin(LED1_C_GPIO_Port, LED1_C_Pin, GPIO_PIN_RESET);
		 	HAL_GPIO_WritePin(LED1_D_GPIO_Port, LED1_D_Pin, GPIO_PIN_RESET);
		 	HAL_GPIO_WritePin(LED1_E_GPIO_Port, LED1_E_Pin, GPIO_PIN_RESET);
		 	HAL_GPIO_WritePin(LED1_F_GPIO_Port, LED1_F_Pin, GPIO_PIN_RESET);
		 	HAL_GPIO_WritePin(LED1_G_GPIO_Port, LED1_G_Pin, GPIO_PIN_RESET);
		 	HAL_GPIO_WritePin(LED1_H_GPIO_Port, LED1_H_Pin, GPIO_PIN_RESET);
		  }

	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_LSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_Error_Handler */
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
/* USER CODE END Header_Error_Handler */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add their own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
