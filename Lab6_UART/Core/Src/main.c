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
UART_HandleTypeDef uartHandler1;
UART_HandleTypeDef uartHandler2;
uint8_t dataBuffer[10];
uint8_t msgUp[] = "UP\r\n";
uint8_t msgDown[] = "DOWN\r\n";
uint8_t msgLeft[] = "LEFT\r\n";
uint8_t msgRight[] = "RIGHT\r\n";
uint8_t msgA[] = "A\r\n";
uint8_t msgB[] = "B\r\n";

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();


  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&uartHandler1, dataBuffer, 1);
  HAL_UART_Transmit(&uartHandler2, (uint8_t*)"Test UART2\r\n", 12, 1000);
  /* USER CODE END 2 */
  //HAL_UART_Transmit(&uartHandler2, Test, sizeof(Test), 1);
  //HAL_UART_Receive_IT(&uartHandler2, dataBuffer, 1);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  	  if (dataBuffer[0] == 'U') {
	          HAL_UART_Transmit(&uartHandler2, msgUp, sizeof(msgUp), 1000);
	      } else if (dataBuffer[0] == 'D') {
	          HAL_UART_Transmit(&uartHandler2, msgDown, sizeof(msgDown), 1000);
	      } else if (dataBuffer[0] == 'L') {
	          HAL_UART_Transmit(&uartHandler2, msgLeft, sizeof(msgLeft), 1000);
	      } else if (dataBuffer[0] == 'R') {
	          HAL_UART_Transmit(&uartHandler2, msgRight, sizeof(msgRight), 1000);
	      } else if (dataBuffer[0] == 'A') {
	          HAL_UART_Transmit(&uartHandler2, msgA, sizeof(msgA), 1000);
	      } else if (dataBuffer[0] == 'B') {
	          HAL_UART_Transmit(&uartHandler2, msgB, sizeof(msgB), 1000);
	      }

	      // Vuelve a recibir el siguiente byte
	      dataBuffer[0] = '1';
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  uartHandler1.Instance = USART1;
  uartHandler1.Init.BaudRate = 9600;
  uartHandler1.Init.WordLength = UART_WORDLENGTH_8B;
  uartHandler1.Init.StopBits = UART_STOPBITS_1;
  uartHandler1.Init.Parity = UART_PARITY_NONE;
  uartHandler1.Init.Mode = UART_MODE_TX_RX;
  uartHandler1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uartHandler1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&uartHandler1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  uartHandler2.Instance = USART2;
  uartHandler2.Init.BaudRate = 115200;
  uartHandler2.Init.WordLength = UART_WORDLENGTH_8B;
  uartHandler2.Init.StopBits = UART_STOPBITS_1;
  uartHandler2.Init.Parity = UART_PARITY_NONE;
  uartHandler2.Init.Mode = UART_MODE_TX_RX;
  uartHandler2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uartHandler2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&uartHandler2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uartHandler)
{
    if(uartHandler->Instance == USART1){
    	HAL_UART_Receive_IT(&uartHandler1, dataBuffer, 1);
    }
}

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
