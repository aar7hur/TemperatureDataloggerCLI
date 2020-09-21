/*
 * I2Ceeprom.h
 *
 *  Created on: 28 de out de 2019
 *      Author: L04347
 */

#include "main.h"

#ifndef I2CEEPROM_H_
#define I2CEEPROM_H_

#define EEPROM_WR  	 0xA0
#define EEPROM_RD  	 0xA1
#define FLAG_ADDRESS 0xFFFF  //endereço onde sera guardado o flag de power off



HAL_StatusTypeDef eeprom_write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t size);
HAL_StatusTypeDef eeprom_read_page(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,uint16_t MemAddress, uint8_t *pData, uint16_t size);
float convert_value(uint16_t rxData);
void erase_memory(uint16_t MemAddress, uint16_t DeviceAddress, I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef EE_Init(I2C_HandleTypeDef *hi2c,uint16_t EEPROM_ADDRESS);


#endif /* I2CEEPROM_H_ */
