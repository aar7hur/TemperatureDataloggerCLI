/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "DS18B20.h"
#include "dwt_stm32_delay.h"
#include "stm32l4xx_hal.h"
#include <string.h>
#include "ssd1306.h"
#include "fonts.h"
#include "stdio.h"
#include "I2Ceeprom.h"
#include "stm32l4xx_it.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SIZEBUF			sizeof(buffer_sensor1)/sizeof(buffer_sensor1[0])

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
float ReadData_ds18b20(uint16_t pin, GPIO_TypeDef* GPIOx);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void i2c_scan(void);
int findLastSavePosition(I2C_HandleTypeDef *hi2c,  uint16_t DevAddress);
uint8_t validValue(uint8_t *data);
void eeprom_read_all_memory(uint16_t MemAddress);
void sendTestValue(void);

//void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp);


#ifndef __UUID_H
#define __UUID_H
//#define STM32_UUID ((uint32_t *)0x1FF0F420)
#define STM32_UUID ((uint32_t *)UID_BASE)
#endif //__UUID_H
#define True 1
#define False 0
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


uint8_t temp_lsb = 0, temp_msb = 0, check,
		counter_pagesize,  data[256], rx_eeprom_buffer[256], flag = 1, counter = 0;


uint16_t temperature_pin1, temperature_pin0, temperature_pin8, temperature_media, temperature;

char str[8], sendToDisplay[20], byte;

float temperature_float;

int init_address = 0;

unsigned char  stringUart[8];

enum Boolean
  {   false,
      true,
  };

typedef enum Boolean voltageDivider;
typedef enum Boolean lastMemAdd;
typedef enum Boolean flag_power_off;

voltageDivider dropvoltage;
lastMemAdd position_returned;
flag_power_off power_off = false;







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
  MX_USART2_UART_Init();

  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  DWT_Delay_Init();

  check = SSD1306_Init();
  print_OK("Medindo...");
  HAL_Delay(1000);
  //sendTestValue();
  //erase_memory(0x0000, EEPROM_WR, &hi2c3);
  //eeprom_read_all_memory(0x0000);

  __HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);

  init_address = findLastSavePosition(&hi2c3, EEPROM_WR);


  HAL_TIM_Base_Start_IT(&htim2);
  //i2c_scan();

  while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
  while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY);

  HAL_UART_Receive_IT(&huart1, (uint8_t*)&byte,4);

  HAL_I2C_IsDeviceReady(&hi2c3, EEPROM_WR, 2,100);

  counter_pagesize = 0;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	  if ((HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1) == RESET) && power_off == false)
	  {
		  power_off = true;
		  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

		  eeprom_write(&hi2c3, EEPROM_WR,FLAG_ADDRESS, flag, 1);
		  //eeprom_read_all_memory(0x0000);

	  }
	  else if(HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1) == SET && power_off == true)
	  {
		  power_off = false;
	  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C3;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration 
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief COMP1 Initialization Function
  * @param None
  * @retval None
  */

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00300F38;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x00707CBB;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

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
  htim2.Init.Prescaler = 100;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000;
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
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
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
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
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
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 LD3_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a characte to the USART2 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

float ReadData_ds18b20(uint16_t pin, GPIO_TypeDef* GPIOx)
{
	 check = Init_ds18b20 (pin, GPIOx);
	 Write_ds18b20 (SKIP_ROM, pin, GPIOx);  // skip ROM
	 Write_ds18b20 (CONVERT_TEMPERATURE,pin, GPIOx);  // convert t
	 DWT_Delay_us(750000);

	 Init_ds18b20 (pin, GPIOx);
	 Write_ds18b20 (SKIP_ROM, pin, GPIOx);  // skip ROM
	 Write_ds18b20 (READ_SCRATCHPAD, pin, GPIOx);  // Read Scratchpad

	 temp_lsb = Read_ds18b20(pin, GPIOx);
	 temp_msb = Read_ds18b20(pin, GPIOx);
	 temperature = (temp_msb<<8)|temp_lsb;


	 return temperature;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(counter == 100)
	{
		counter = 0;

		temperature_pin1 = ReadData_ds18b20(GPIO_PIN_1, GPIOB);
		temperature_pin0 = ReadData_ds18b20(GPIO_PIN_0, GPIOB);
		temperature_pin8 = ReadData_ds18b20(GPIO_PIN_8, GPIOA);

		temperature_float = ((float)(temperature_pin8)/16);

		if((temperature_pin1 == 0xFFFF) && (temperature_pin8 != 0xFFFF))
		{
			temperature_media = temperature_pin1;
		}
		else if((temperature_pin8 == 0xFFFF) && (temperature_pin1 != 0xFFFF))
		{
			temperature_media = temperature_pin8;
		}
		else
		{
			temperature_media = (uint16_t)((temperature_pin8 + temperature_pin1)/2);
		}

		if(temperature_pin0 == 0xFFFF)
		{
			for(uint8_t i = 0; i <2; i++)
			{
				temperature_pin0 = ReadData_ds18b20(GPIO_PIN_0, GPIOB);
				if(temperature_pin0 != 0xFFFF)
				{
					break;
				}
				else
				{
					continue;
				}
			}
			if(temperature_pin0 == 0xFFFF)
			{
				temperature_pin0 = 0xFEE0;
			}

		}

		sprintf(str, "%.1f C ", temperature_float);
		print_temperature(str);

		data[counter_pagesize] = (uint8_t)  (temperature_pin0 & 0xFF);
		data[counter_pagesize+1] = (uint8_t) ((temperature_pin0 & 0xFF00) >> 8);
		data[counter_pagesize+2] = (uint8_t) (temperature_media & 0xFF);
		data[counter_pagesize+3] = (uint8_t) ((temperature_media & 0xFF00) >> 8);


		if(counter_pagesize >= 252)
		{
			if((EE_Init(&hi2c3, EEPROM_WR) == HAL_OK))
			{

				eeprom_write(&hi2c3, EEPROM_WR,init_address, data, 256);
				counter_pagesize = 0;

				if(init_address == 0xFF00)
				{

					SSD1306_Fill (0);
					SSD1306_UpdateScreen ();

					SSD1306_GotoXY(0,10);
					SSD1306_Puts("MEMORIA", &Font_11x18,1);
					SSD1306_GotoXY (00,35);
					SSD1306_Puts("CHEIA!", &Font_11x18,1);

					SSD1306_UpdateScreen();
					HAL_TIM_Base_Stop_IT(&htim2);

				}
				else
				{
					init_address += 0x100;

				}
			}
		}
		else
			{
				counter_pagesize += 4;
			}

		}
	else
	{
		counter++;
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	}

	}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	  if (huart->Instance == USART1)
	  {

		char ReadData[4] = "READ";
		char EraseData[4] = "ERAS";
		uint8_t ret, ret2;

		ret  = strncmp(ReadData,&byte,4);
		ret2 = strncmp(EraseData,&byte,4);

		if(ret == 0)
		{
			SSD1306_Fill (0);
			SSD1306_UpdateScreen ();

			SSD1306_GotoXY(0,10);
			SSD1306_Puts("Reading", &Font_11x18,1);
			SSD1306_GotoXY (00,35);
			SSD1306_Puts("Data!", &Font_11x18,1);

			SSD1306_UpdateScreen();
			eeprom_read_all_memory(0x0000);

			SSD1306_Fill (0);
			SSD1306_GotoXY(0,10);
			SSD1306_Puts("Data ok!", &Font_11x18,1);
			SSD1306_UpdateScreen();

		}


		if(ret2 == 0)
		{

			erase_memory(0x0000, EEPROM_WR, &hi2c3);
		}
	    //HAL_UART_Transmit(&huart2, (uint8_t*)&byte, 4, 100);

	    HAL_UART_Receive_IT(&huart1, (uint8_t*)&byte,4);
	}
}
void i2c_scan(void){


	printf("Conecte ao dispositivo I2C!");
	printf("\r\n");

	HAL_StatusTypeDef result;
	uint8_t i;
	for (i=1; i<128; i++)
	 	{

	 	  result = HAL_I2C_IsDeviceReady(&hi2c3, (uint16_t)(i<<1), 2, 100000);
	 	  if (result != HAL_OK) // HAL_ERROR or HAL_BUSY or HAL_TIMEOUT
	 	  {
	 		  printf("Dispositivo nao encontrado\r\n"); // No ACK received at that address

	 	  }
	 	  if (result == HAL_OK)
	 	  {
	 		  printf("Dispositivo encontrado. Endereco: 0x0%X\r\n", i<<1);
	 		  break;// Received an ACK at that address
	 	  }

	 	}

}
int findLastSavePosition(I2C_HandleTypeDef *hi2c,  uint16_t DevAddress)
{
    init_address = 0x0000;
    while(init_address <= 0xFF00)
    {
        eeprom_read_page(&hi2c3, EEPROM_WR, init_address,rx_eeprom_buffer, 256);

        if (validValue(rx_eeprom_buffer) == true)
        {
        	if(init_address == 0xFF00)
        	{
        		return init_address;
        		break;
        	}
        	else
        	{
        		init_address += 0x100;
        	}
        }
        else
        {
        	return init_address;


        }
    }
}

uint8_t validValue(uint8_t *data)
{
    uint8_t i =0;
    while(i<256)
    {
        uint16_t temp_value;
        float temp_float;
        temp_value = data[i+1]<<8 | data[i];
        temp_float = (float)temp_value/16;

        if((temp_value != 0xFFFF)) //&& (temp_float < 0x46) && (temp_float > 0xA)) //menor que 70 e maior que 10
        {

            if(i == 254)
            {
                position_returned = true;
                return position_returned;
            }
            i+=2;

        }
        else
        {
            position_returned = false;
            return position_returned;
        }

    }
}
void eeprom_read_all_memory(uint16_t MemAddress)
{

	uint16_t aux = 0;
	uint8_t print_data[256], i =0;
	float temp;

	while(MemAddress != 0xFFFF)

	{
		eeprom_read_page(&hi2c3, EEPROM_WR, MemAddress, print_data, 256);

		while( i < 254)
		{
			aux =  (print_data[i+1] << 8 | print_data[i]);
			temp = (float)aux/16;

			printf("%.1f\r\n", temp);

			i+=2;
		}
		i =0;
		if(MemAddress == 0xFF00)
		{
			MemAddress += 0xFF;
		}
		else
		{
			MemAddress += 0x100;
		}

	}
}

void sendTestValue(void)
{
	uint8_t teste[256];
	uint16_t j = 0;

	for(uint16_t i = 0; i < 256; i++)
	{
		teste[i] = 12;
	}
	while (j <= 0xFE00)
	{
		eeprom_write(&hi2c3, EEPROM_WR,j, teste, 256);
		j+= 0x100;
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
