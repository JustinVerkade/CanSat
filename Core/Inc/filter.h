/*
 * filter.c
 *
 *  Created on: Apr 6, 2022
 *      Author: J.Verkade
 */

#ifndef _FILTER_H_
#define _FILTER_H_

typedef struct
{
	float filter_coefficient_x;
	float filter_coefficient_y;
	float previous_value;
} filter_t;

void Filter_init(filter_t *me, float hz, float dt);
float Filter_run(filter_t *me, float current_value);

#endif // _FILTER_H_


