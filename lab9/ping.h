/*
 * ping.h
 *
 *  Created on: Jun 23, 2018
 *      Author: hr24612
 */

#ifndef PING_H_
#define PING_H_

void TIMER3B_Handler(void);
void ping_init();
void TIMER1A_Handler(void);
void send_pulse(void);
unsigned ping_read(void);
unsigned time2dist(unsigned time);

#endif /* PING_H_ */
