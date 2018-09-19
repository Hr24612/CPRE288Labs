/**
 * author: Amir Hamza.
 * date: 06/14/2018.
 */
#include "lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include <math.h>
#include "timer.h"
#include <driverlib/interrupt.h>
#include "servo.h"
#include "adc.h"

volatile enum {LOW, HIGH, DONE} state;
unsigned long overflow = 0;
volatile unsigned last_time = 0;
volatile unsigned current_time = 0;
volatile int update_flag = 0;
volatile unsigned rising_time; // start time of the return pulse
volatile unsigned falling_time; // end time of the return pulse

/* start and read the ping sensor once, return distance in mm */
void TIMER3B_Handler(void)
{
    TIMER3_ICR_R = TIMER_ICR_CBECINT;
    if (state == LOW)
    {
        rising_time = TIMER3_TBR_R;
        state = HIGH;
        return;
    }
    else if (state == HIGH)
    {
        falling_time = TIMER3_TBR_R;
        state = DONE;
        TIMER3_CTL_R &= 0xFEFF;
        return;
    }
    else
    {
        return;
    }
}

void ping_init()
{
    // Enable Port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    // Enable digital circuitry for pin 3
    GPIO_PORTB_DEN_R |= 0x08;
    //set pin 3 to alt function
    GPIO_PORTB_AFSEL_R |= 0x08;

    GPIO_PORTB_PCTL_R |= 0x7000;
    //set pin 3 to output
    GPIO_PORTB_DIR_R |= 0x08;
    // Enable input capture mode
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;
    TIMER3_CTL_R &= 0xFEFF; // Enable, Edge Select.
    TIMER3_CTL_R |= 0x0C00;
    TIMER3_CFG_R |= 0x04; //16-bit mode.
    TIMER3_TBMR_R |= 0b010111; //Capture Mode, Edge-Time Mode, Count up.
    TIMER3_TBPR_R = 0xFF;
    TIMER3_TBILR_R = 0xFFFF; //Set upper bound.
    TIMER3_IMR_R |= 0x00000400; // Enable capture interrupt
    TIMER3_ICR_R |= 0x00000400;
    //intialize global interrupts
    NVIC_EN1_R = 0x010;
    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();
}

// ISR: Record the current event time
void TIMER1A_Handler(void)
{
    last_time = current_time;
    current_time = TIMER1_TAR_R;
    update_flag = 1;
}



/* send out a pulse on PB3 */
void send_pulse()
{
    GPIO_PORTB_AFSEL_R &= 0xF7;
    GPIO_PORTB_DIR_R |= 0x08; // set PB3 as output
    GPIO_PORTB_DATA_R |= 0x08; // set PB3 to high

    // wait at least 5 microseconds based on data sheet
    timer_waitMillis(5);

    GPIO_PORTB_DATA_R &= 0xF7; // set PB3 to low
    GPIO_PORTB_DIR_R &= 0xF7; // set PB3 as input
    GPIO_PORTB_AFSEL_R |= 0x08;
    state = LOW;
    TIMER3_CTL_R |= 0b0100000000;
}

/* convert time in clock counts to single-trip distance in mm */
unsigned ping_read()
{
    send_pulse(); // send the starting pulse to PING

    unsigned diff;
    diff = falling_time - rising_time;
    if (rising_time > falling_time)
    {
        overflow++;
        diff += 0xFFFFFF;
    }

    double distance = time2dist(diff);
   // lcd_printf("dist:%lf cm\noverflow:%lu\ndiff:%lu", distance, overflow, diff);
    timer_waitMillis(100);
    return distance;
}

unsigned time2dist(unsigned time)
{
    return ((time / 16000000.0) * 34300.0)/2;
}

