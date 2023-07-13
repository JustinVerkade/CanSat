/*
 * Goertek-SPL06-007.c
 *
 *  Created on: Nov 2, 2020
 *      Author: J.Verkade
 */


#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "stdlib.h"
#include "Goertek-SPL06-007.h"
#include "fox.h"
#include <math.h>

uint8_t SPL006_Init()
{
	uint8_t p_sampling[] = {0x06, 0b01110010};
	ret = HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, p_sampling, 2, HAL_MAX_DELAY);

	uint8_t t_sampling[] = {0x07, 0b11110010};
	ret = HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, t_sampling, 2, HAL_MAX_DELAY);

	uint8_t config[] = {0x08, 0b0111};
	ret = HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, config, 2, HAL_MAX_DELAY);

	uint8_t fifo[] = {0x09, 0x00};
	ret = HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, fifo, 2, HAL_MAX_DELAY);

	return 0;
}

static int16_t SPL006_getC0();
static int16_t SPL006_getC1();
static int32_t SPL006_getC00();
static int32_t SPL006_getC10();
static int16_t SPL006_getC01();
static int16_t SPL006_getC11();
static int16_t SPL006_getC20();
static int16_t SPL006_getC21();
static int16_t SPL006_getC30();

static int32_t SPL006_getTempRaw();
static int32_t SPL006_getPressureRaw();

float SPL006_ReadTemperature()
{
	int16_t c0 = SPL006_getC0();
	int16_t c1 = SPL006_getC1();
	float traw_sc = (float)SPL006_getTempRaw() / 3670016.0f; // for 4x prescaling at 128 hz
	return (float)c0 * 0.5f + (float)c1 * traw_sc;
}

float SPL006_ReadPressure()
{
	int32_t c00 = SPL006_getC00();
	int32_t c10 = SPL006_getC10();
	int16_t c01 = SPL006_getC01();
	int16_t c11 = SPL006_getC11();
	int16_t c20 = SPL006_getC20();
	int16_t c21 = SPL006_getC21();
	int16_t c30 = SPL006_getC30();
	float traw_sc = (float)SPL006_getTempRaw() / 3670016.0f;
	float praw_sc = (float)SPL006_getPressureRaw() / 3670016.0f;
	return (float)c00 + praw_sc * ((float)c10 + praw_sc * ((float)c20 + praw_sc * (float)c30)) + traw_sc * (float)c01 + traw_sc * praw_sc * ((float)c11 + praw_sc * (float)c21);
}

float SPL006_ReadAltitude()
{
	float pressure = SPL006_ReadPressure();
	float sealevel_hpa = 100800.0f;
	return 44330.0f * (1.0f - powf(pressure / sealevel_hpa, 0.1903f));
}

static int16_t SPL006_getC0()
{
	int16_t tmp;
	uint8_t data[3] = {0x10, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 2, HAL_MAX_DELAY);

	data[2] = data[2] >> 4;

	tmp = ((int16_t)data[1]<<4) | (int16_t)data[2];

	if(tmp & (1<<11))
		tmp |= 0xF000;

	return tmp;
}

static int16_t SPL006_getC1()
{
	int16_t tmp;
	uint8_t data[3] = {0x11, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 2, HAL_MAX_DELAY);

	data[1] = data[1] & 0x0F;

	tmp = ((int16_t)data[1] << 8) | (int16_t)data[2];

	if(tmp & (1<<11))
		tmp = tmp | 0xF000;

	return tmp;
}

static int32_t SPL006_getC00()
{
	int32_t tmp;
	uint8_t data[4] = {0x13, 0x00, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 3, HAL_MAX_DELAY);

	tmp = ((int32_t)data[1] << 12) | ((int32_t)data[2] << 4) | ((int32_t)data[3] >> 4);

	if(tmp & (1<<19))
		tmp = tmp | 0xFFF00000;

	return tmp;
}

static int32_t SPL006_getC10()
{
	int32_t tmp;
	uint8_t data[4] = {0x15, 0x00, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 3, HAL_MAX_DELAY);

	tmp = ((int32_t)(data[1] & 0x0F) << 16) | ((int32_t)data[2] << 8) | (int32_t)data[3];

	if(tmp & (1<<19))
		tmp = tmp | 0xFFF00000;

	return tmp;
}

static int16_t SPL006_getC01()
{
	int16_t tmp;
	uint8_t data[3] = {0x18, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 2, HAL_MAX_DELAY);

	tmp = ((int32_t)data[1] << 8) | (int32_t)data[2];

	return tmp;
}

static int16_t SPL006_getC11()
{
	int16_t tmp;
	uint8_t data[3] = {0x1A, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 2, HAL_MAX_DELAY);

	tmp = ((int32_t)data[1] << 8) | (int32_t)data[2];

	return tmp;
}

static int16_t SPL006_getC20()
{
	int16_t tmp;
	uint8_t data[3] = {0x1C, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 2, HAL_MAX_DELAY);

	tmp = ((int32_t)data[1] << 8) | (int32_t)data[2];

	return tmp;
}

static int16_t SPL006_getC21()
{
	int16_t tmp;
	uint8_t data[3] = {0x1E, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 2, HAL_MAX_DELAY);

	tmp = ((int32_t)data[1] << 8) | (int32_t)data[2];

	return tmp;
}

static int16_t SPL006_getC30()
{
	int16_t tmp;
	uint8_t data[3] = {0x20, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 2, HAL_MAX_DELAY);

	tmp = ((int32_t)data[1] << 8) | (int32_t)data[2];

	return tmp;
}

static int32_t SPL006_getTempRaw()
{
	int32_t tmp;
	uint8_t data[4] = {0x03, 0x00, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 3, HAL_MAX_DELAY);

	tmp = ((int32_t)data[1] << 8) | (int32_t)data[2];
	tmp = (tmp << 8) | (int32_t)data[3];

	if(tmp & (1<<23))
		tmp = tmp | 0xFF000000;

	return tmp;
}

static int32_t SPL006_getPressureRaw()
{
	int32_t tmp;
	uint8_t data[4] = {0x00, 0x00, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, SPL006_ADDR, data, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, SPL006_ADDR, data+1, 3, HAL_MAX_DELAY);

	tmp = ((int32_t)data[1] << 8) | (int32_t)data[2];
	tmp = (tmp << 8) | (int32_t)data[3];

	if(tmp & (1<<23))
		tmp = tmp | 0xFF000000;

	return tmp;
}
