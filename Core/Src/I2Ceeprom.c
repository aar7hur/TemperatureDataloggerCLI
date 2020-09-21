/*
 * I2Ceeprom.c
 *
 *  Created on: 28 de out de 2019
 *      Author: L04347
 */


#include <string.h>
#include <stdlib.h>
#include "stdio.h"
#include "stm32l4xx_hal.h"
#include "I2Ceeprom.h"
#include "stm32l4xx_hal_i2c.h"
#include "ssd1306.h"

I2C_HandleTypeDef hi2c1;
extern uint8_t  rxBuffer[2];
extern unsigned int timeout;


HAL_StatusTypeDef eeprom_write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t size)
{
	HAL_StatusTypeDef returnValue;
	uint8_t *data;

	data = (uint8_t*)malloc(sizeof(uint8_t)*(size+2));


	//data[0] = (uint8_t) ((MemAddress & 0xFF0000) >> 16);  /// comentar
	data[0] = (uint8_t)	((MemAddress & 0xFF00) >>8);
	data[1] = (uint8_t) (MemAddress & 0xFF);

	memcpy(data+2, pData, size);

	returnValue = HAL_I2C_Master_Transmit(hi2c, DevAddress, data, size+2, HAL_MAX_DELAY);
	if(returnValue != HAL_OK)
		return returnValue;


	free(data);

	while(HAL_I2C_IsDeviceReady(hi2c, DevAddress,1,HAL_MAX_DELAY) != HAL_OK);

	return HAL_OK;
}

HAL_StatusTypeDef eeprom_read_page(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t size) {



	HAL_StatusTypeDef returnValue;
	uint8_t addr[2];

	addr[0] = (uint8_t)	((MemAddress & 0xFF00) >> 8);
	addr[1] = (uint8_t) (MemAddress & 0xFF);


	returnValue = HAL_I2C_Master_Transmit(hi2c, DevAddress, addr, 2, HAL_MAX_DELAY);
	if(returnValue != HAL_OK)
		return returnValue;

  /* Next we can retrieve the data from EEPROM */
	returnValue = HAL_I2C_Master_Receive(hi2c, DevAddress, pData, size, HAL_MAX_DELAY);

	return returnValue;
}

float convert_value(uint16_t rxData)
{

	float returnValueFloat;
	uint16_t resolution = 4095;
	float constant = 3.3;


	returnValueFloat = ((float)rxData/resolution)*constant;

	return returnValueFloat;

}

void erase_memory(uint16_t MemAddress, uint16_t DeviceAddress, I2C_HandleTypeDef *hi2c)
{

    uint8_t data[256];
    print_OK("Apagando Mem..");

    for(uint8_t i = 0; i< 255; i++)
        {
            data[i] = 0xFF;
        }

    while( MemAddress != 0xFFFF)
    {
    	 eeprom_write(hi2c,DeviceAddress,MemAddress, data, 256);

    	 if(MemAddress == 0xFF00)
    	    {
         		MemAddress += 0xFF;
           	}
 	   	else
           	{
         		MemAddress += 0x100;
           	}

    }

    MemAddress = 0x00;




}

HAL_StatusTypeDef EE_Init(I2C_HandleTypeDef *hi2c,uint16_t EEPROM_ADDRESS)
{
	HAL_StatusTypeDef EepromStatus;

	EepromStatus = HAL_I2C_IsDeviceReady(hi2c,EEPROM_ADDRESS, 2, 100);

	return EepromStatus;
}
















