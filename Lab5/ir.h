/*
 * ir.h
 *
 *  Created on: Jul 12, 2016
 *      Author: Dane Larson
 */

#ifndef IR_H_
#define IR_H_

#include <stdio.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

void ir_init();
void ir_Enable_IRQ();
void ir_enableInterrupt();
int ir_getValue();
float ir_getDistance();



#endif /* IR_H_ */
