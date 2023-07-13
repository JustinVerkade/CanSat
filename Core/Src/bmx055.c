/*
 * bmx055.c
 *
 *  Created on: 8 Jul 2021
 *      Author: J.Verkade
 */

#include "bmx055.h"
#include <string.h>
#include "usbd_cdc_if.h"

#define ACCEL_ENABLE HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
#define ACCEL_DISABLE HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
#define MAGNET_ENABLE HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
#define MAGNET_DISABLE HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
#define GYRO_ENABLE HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
#define GYRO_DISABLE HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

void BMX055_init(SPI_HandleTypeDef* hspi)
{
	ACCEL_DISABLE;
	MAGNET_DISABLE;
	GYRO_DISABLE;

	MAGNET_ENABLE;
	uint8_t data1[2] = {0x4B, 0x01};
	HAL_SPI_Transmit(hspi, data1, 2, 1000);
	MAGNET_DISABLE;

	HAL_Delay(100);
}

uint8_t BMX055_readAccelId(SPI_HandleTypeDef* hspi)
{
	uint8_t id[2] = {0x00, 0x00};
	uint8_t id_addr[1] = {(0x00 | 0x80)};
	ACCEL_ENABLE;
	HAL_SPI_TransmitReceive(hspi, id_addr, id, 2, 1000);
	ACCEL_DISABLE;
	return id[1];
}

uint8_t BMX055_readMagnetId(SPI_HandleTypeDef* hspi)
{
	uint8_t id[2] = {0x00, 0x00};
	uint8_t id_addr[1] = {(0x40 | 0x80)};
	MAGNET_ENABLE;
	HAL_SPI_TransmitReceive(hspi, id_addr, id, 2, 1000);
	MAGNET_DISABLE;
	return id[1];
}

uint8_t BMX055_readGyroId(SPI_HandleTypeDef* hspi)
{
	uint8_t id[2] = {0x00, 0x00};
	uint8_t id_addr[1] = {(0x00 | 0x80)};
	GYRO_ENABLE;
	HAL_SPI_TransmitReceive(hspi, id_addr, id, 2, 1000);
	GYRO_DISABLE;
	return id[1];
}

void BMX055_readAccel(SPI_HandleTypeDef* hspi, int16_t data[3])
{
	uint8_t raw_data[7];
	uint8_t id_addr[1] = {(0x02 | 0x80)};

	ACCEL_ENABLE;
	HAL_SPI_TransmitReceive(hspi, id_addr, raw_data, 7, 1000);
	ACCEL_DISABLE;

	data[0] = raw_data[1] | (raw_data[2] << 8);
	data[1] = raw_data[3] | (raw_data[4] << 8);
	data[2] = raw_data[5] | (raw_data[6] << 8);
}

void BMX055_readMagnet(SPI_HandleTypeDef* hspi, int16_t data[3])
{
	uint8_t raw_data[7];
	uint8_t id_addr[1] = {(0x42 | 0x80)};

	MAGNET_ENABLE;
	HAL_SPI_TransmitReceive(hspi, id_addr, raw_data, 7, 1000);
	MAGNET_DISABLE;

	data[0] = ((raw_data[2] << 8) + (raw_data[1] & 0xF8));
	data[1] = ((raw_data[4] << 8) + (raw_data[3] & 0xF8));
	data[2] = ((raw_data[6] << 8) + (raw_data[5] & 0xFE));
}

void BMX055_readGyro(SPI_HandleTypeDef* hspi, int16_t data[3])
{
	uint8_t raw_data[7];
	uint8_t id_addr[1] = {(0x02 | 0x80)};

	GYRO_ENABLE;
	HAL_SPI_TransmitReceive(hspi, id_addr, raw_data, 7, 1000);
	GYRO_DISABLE;

	data[0] = raw_data[1] | (raw_data[2] << 8);
	data[1] = raw_data[3] | (raw_data[4] << 8);
	data[2] = raw_data[5] | (raw_data[6] << 8);
}

void BMX055_setAccelRange(SPI_HandleTypeDef* hspi, accel_range_t range)
{
	ACCEL_ENABLE;
	uint8_t data[2] = {0x0F, (uint8_t)range};
	HAL_SPI_Transmit(hspi, data, 2, 1000);
	ACCEL_DISABLE;
}

void BMX055_setGyroRange(SPI_HandleTypeDef* hspi, gyro_range_t range)
{
	GYRO_ENABLE;
	uint8_t data[2] = {0x0F, (uint8_t)range};
	HAL_SPI_Transmit(hspi, data, 2, 1000);
	GYRO_DISABLE;
}


