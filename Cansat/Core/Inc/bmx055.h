/*
 * bmx055.h
 *
 *  Created on: 8 Jul 2021
 *      Author: J.Verkade
 */

#ifndef INC_BMX055_H_
#define INC_BMX055_H_

#include "stm32f4xx_hal.h"

void BMX055_init(SPI_HandleTypeDef* hspi);

uint8_t BMX055_readAccelId(SPI_HandleTypeDef* hspi);
uint8_t BMX055_readMagnetId(SPI_HandleTypeDef* hspi);
uint8_t BMX055_readGyroId(SPI_HandleTypeDef* hspi);

void BMX055_readAccel(SPI_HandleTypeDef* hspi, int16_t data[3]);
void BMX055_readMagnet(SPI_HandleTypeDef* hspi, int16_t data[3]);
void BMX055_readGyro(SPI_HandleTypeDef* hspi, int16_t data[3]);

typedef enum
{
	a2g = 0b0011,
	a4g = 0b0101,
	a8g = 0b1000,
	a16g = 0b1100,
} accel_range_t;

typedef enum
{
	g2000g = 0b000,
	g1000g = 0b001,
	g500g = 0b010,
	g250g = 0b011,
	g125g = 0b100
} gyro_range_t;

void BMX055_setAccelRange(SPI_HandleTypeDef* hspi, accel_range_t range);
void BMX055_setGyroRange(SPI_HandleTypeDef* hspi, gyro_range_t range);

#endif /* INC_BMX055_H_ */
