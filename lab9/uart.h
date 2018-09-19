/*
 * urat.h
 *
 *  Created on: Jun 23, 2018
 *      Author: hr24612
 */

#ifndef UART_H_
#define UART_H_


#include "Timer.h"
#include <inc/tm4c123gh6pm.h>

void uart_init(void) ;

void uart_sendChar(char data);

char uart_receive(void);

char uart_receive2(void);

void uart_sendString(char data[]);


#endif /* UART_H_ */
