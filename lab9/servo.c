/*
 * servo.c
 *
 *  Created on: Jun 24, 2018
 *      Author: hr24612
 */

#include <inc/tm4c123gh6pm.h>
#include "timer.h"
#include "driverlib/interrupt.h"
#include "lcd.h"
#include "servo.h"
#include <stdint.h>
double modifier = 153.083333;
int start = 7112;

void PWM_init()
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; //Port B (1)
    GPIO_PORTB_AFSEL_R |= 0x20; //set pin 5 to alt function
    GPIO_PORTB_PCTL_R |= 0x700000;
    GPIO_PORTB_DIR_R |= 0x20; //set pin 5 to output
    GPIO_PORTB_DEN_R |= 0x20; //set pin 5 to digital
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; //start timer clock
    TIMER1_CTL_R &= 0xFEFF; //disable the timer
    TIMER1_CFG_R |= 0x04;   //configures timer values (16 bit timer)
    TIMER1_TBMR_R |= 0x0000000A;    //pin 3 enables PWM, pin 2 sets capture mode(edge-count), pin 1:0 set the timer mode (one shot timer mode)
    TIMER1_TBPR_R = (320000 >> 16) & 0xFF;  //overflow for where the timer starts
    TIMER1_TBILR_R = 320000 & 0xFFFF;   //first 16 bits for the timer
    TIMER1_TBPMR_R = 0x00;  // most sig bits for the value MATCHR
    TIMER1_TBMATCHR_R = 0x3E80; //value the timer needs to match before doing an event
    TIMER1_CTL_R |= 0x0100; //enable the timer
    return;
}

int turn_servo(double angle)
{
    //calculate the number the timer needs to match
    int result = 320000 - (start + (modifier * angle));
    TIMER1_TBMATCHR_R = result & 0xFFFF;
    TIMER1_TBPMR_R = (result >> 16) & 0xFF;
    return result;
}


