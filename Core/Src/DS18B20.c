/*
 * DS18B20.c
 *
 *  Created on: 4 de nov de 2019
 *      Author: Arthur.Amaral
 */




#include "main.h"
#include "stm32l4xx.h"
#include "dwt_stm32_delay.h"




void Gpio_set_input (uint16_t pin, GPIO_TypeDef* GPIOx)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


void Gpio_set_output (uint16_t pin, GPIO_TypeDef* GPIOx)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}




uint8_t Init_ds18b20 (uint16_t pin, GPIO_TypeDef* GPIOx)
{
	Gpio_set_output (pin, GPIOx);   // set the pin as output
	HAL_GPIO_WritePin (GPIOx, pin, 0);  // pull the pin low
	DWT_Delay_us (480);   // delay according to datasheet

	Gpio_set_input (pin, GPIOx);    // set the pin as input
	DWT_Delay_us (80);    // delay according to datasheet

	if (!(HAL_GPIO_ReadPin (GPIOx, pin)))    // if the pin is low i.e the presence pulse is there
	{
		DWT_Delay_us (400);  // wait for 400 us
		return 0;
	}

	else
	{
		DWT_Delay_us (400);
		return 1;
	}
}

void Write_ds18b20 (uint8_t data, uint16_t pin, GPIO_TypeDef* GPIOx)
{
	Gpio_set_output (pin,GPIOx);   // seta como saida

	for (int i=0; i<8; i++)
	{

		if ((data & (1<<i))!=0)  // se o bit for 1
		{
			// escreve 1

			Gpio_set_output (pin,  GPIOx);  // seta como saida
			HAL_GPIO_WritePin (GPIOx, pin, 0);  // coloca o pino em 0
			DWT_Delay_us (1);  // wait for  us

			Gpio_set_input (pin, GPIOx);  // seta entrada
			DWT_Delay_us (60);  // delay de acordo com datasheet
		}

		else  // se o bit é 0
		{
			// escreve 0

			Gpio_set_output (pin, GPIOx);
			HAL_GPIO_WritePin (GPIOx, pin, 0);  // coloca o pino 0
			DWT_Delay_us (60);  // espera 60 us de acordo com datasheet

			Gpio_set_input (pin, GPIOx);
		}
	}
}


uint8_t Read_ds18b20 (uint16_t pin, GPIO_TypeDef* GPIOx)
{
	uint8_t value=0;
	Gpio_set_input (pin, GPIOx);

	for (int i=0;i<8;i++)
	{
		Gpio_set_output (pin, GPIOx);   // set as output

		HAL_GPIO_WritePin (GPIOx, pin, 0);  // pull the data pin LOW
		DWT_Delay_us (2);  // wait for 2 us

		Gpio_set_input (pin, GPIOx);  // set as input
		if (HAL_GPIO_ReadPin (GPIOx, pin))  // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1
		}
		DWT_Delay_us (60);  // wait for 60 us
	}
	return value;
}




























