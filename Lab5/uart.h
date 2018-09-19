/*
 * uart.h
 *
 *  Created on: Mar 28, 2017
 *      Author: kylem
 */

#ifndef UART_H_
#define UART_H_
#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>

void uart_init(void);
void uart_sendChar(char data);
void uart_sendStr(char data[]);
char uart_receive(void);

#endif /* UART_H_ */
