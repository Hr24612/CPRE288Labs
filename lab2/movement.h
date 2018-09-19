/*
 * movement.h
 *
 *  Created on: May 28, 2018
 *      Author: hr24612
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "open_interface.h"

void move_forward(oi_t *sensor, int centimeters);
void turn_clockwise(oi_t *sensor, int degrees);
void part3(oi_t *sensor, int Distance);
#endif /* MOVEMENT_H_ */
