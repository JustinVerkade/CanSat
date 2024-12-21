/*
 * filter.c
 *
 *  Created on: Apr 6, 2022
 *      Author: J.Verkade
 */

#include "filter.h"

void Filter_init(filter_t *me, float hz, float dt)
{
	float rc = 1.0f / (2.0f * 3.141592f * hz);
	me->filter_coefficient_x = dt / (dt + rc);
	me->filter_coefficient_y = rc / (dt + rc);
	me->previous_value = 0.0f;
}

float Filter_run(filter_t *me, float current_value)
{
	float filtered_value = current_value * me->filter_coefficient_x + me->previous_value * me->filter_coefficient_y;
	me->previous_value = filtered_value;
	return filtered_value;
}


