/*
 * Hazards.h
 *
 *  Created on: Apr 9, 2017
 *      Author: bigisler
 */

#ifndef HAZARDS_H_
#define HAZARDS_H_

#include "uart.h"
#include "math.h"
#include "ir.h"
#include "ping.h"
#include "timer.h"
#include "lcd.h"

void scan_Hazard(int MODE);//move_by is for deciding incramentation. MODE: 0 manual, 1 automatic.
void timer1_init();
void move_servo(int degree);
void move_servo_C(int degree);


#endif /* HAZARDS_H_ */
