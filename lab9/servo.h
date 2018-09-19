/*
 * servo.h
 *
 *  Created on: Jun 24, 2018
 *      Author: hr24612
 */

#ifndef SERVO_H_
#define SERVO_H_


#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "driverlib/interrupt.h"
#include <stdint.h>

void PWM_init(void);
int turn_servo(double angle);


#endif /* SERVO_H_ */
