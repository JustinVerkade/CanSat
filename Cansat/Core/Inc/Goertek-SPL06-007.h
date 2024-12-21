/*
 * Goertek-SPL06-007.h
 *
 *  Created on: Nov 2, 2020
 *      Author: J.Verkade
 */

#ifndef INC_GOERTEK_SPL06_007_H_
#define INC_GOERTEK_SPL06_007_H_

HAL_StatusTypeDef ret;
I2C_HandleTypeDef hi2c1;
static const uint8_t SPL006_ADDR = 0x76<<1 | 0<<0;

uint8_t SPL006_Init();
float SPL006_ReadTemperature();
float SPL006_ReadPressure();
float SPL006_ReadAltitude();

#endif /* INC_GOERTEK_SPL06_007_H_ */
