/**
 * clock-intr.c: timer- and interrupt-based clock program
 * This is skeleton code. You need to make it complete.
 *
 * @author James Talbert, Zach Glanz, Zhao Zhang
 * @date 05/24/2018
 */

#include <stdint.h>
#include "timer.h"
#include "lcd_stepper.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "button.h"
#define TIMER4_PRESCALER 250



 //This is the interval for incrementing the clock
#define TIMER4A_COUNTS 64000 // Starting value of Timer to countdown to zero in 1 second


// This is the interval for checking the buttons
#define TIMER4B_COUNTS 12800 // Starting value of Timer to countdown to zero in 200 milliseconds

#define SW1 1
#define SW2 2
#define SW3 3
#define SW4 4
#define SW5 5
#define SW6 6

volatile int hour = 0;
volatile int min = 0;
volatile int sec = 0;
int count = 0;
/**
 * Timer interrupt source 1: the function will be called every one second
 * (you need define TIMER4A_COUNTS value correctly and enable the interrupts for TIMER4A
 */
void TIMER4A_Handler(void)
{
	TIMER4_ICR_R = TIMER_ICR_TATOCINT;
	sec = (sec+1) % 60;
	    if(sec == 0)
	    {
	        min = (min+1) % 60;
	    }
	    if(sec == 0 && min == 0)
	    {
	        hour = (hour+1) % 24;
	    }
}


void TIMER4B_Handler(void)
{
	TIMER4_ICR_R = TIMER_ICR_TBTOCINT;
	uint8_t button = button_getButton();
	if(button == SW6)
	    {
	        hour = (hour+1) % 25;
	    }

	    if(button == SW5)
	    {
	        if(hour == 0)
	        {
	            hour = 25;
	        }
	        hour--;
	    }
	    if(button == SW4)
	    {
	        min = (min+1) % 60;
	    }
	    if(button == SW3)
	    {
	        if(min == 0)
	        {
	            min = 60;
	        }
	        min--;
	    }

	    if(button == SW2)
	    {
	        sec = (sec+1) % 60;
	    }

	    if(button == SW1)
	    {
	        if(sec == 0)
	        {
	            sec = 60;
	        }
	        sec--;
	    }
}

/// Initialize the timers
void clock_timer_init(void) {
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R4; // Turn on clock to TIMER4

	//Configure the timer for input capture mode
	TIMER4_CTL_R &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN); //disable timerB to allow us to change settings
	TIMER4_CFG_R |= TIMER_CFG_16_BIT; //set to 16 bit timer

	TIMER4_TAMR_R = TIMER_TAMR_TAMR_PERIOD; //set for periodic mode, down count
	TIMER4_TBMR_R = TIMER_TBMR_TBMR_PERIOD; //set for periodic mode, down count

	TIMER4_TAPR_R = TIMER4_PRESCALER - 1; // 1 sec clock
	TIMER4_TBPR_R = TIMER4_PRESCALER - 1;  // 200 ms clock

	TIMER4_TAILR_R = TIMER4A_COUNTS - 1; // set the load value for the 1 second clock (with your prescaler, how many clock cycles happen in 1 second?)
	TIMER4_TBILR_R = TIMER4B_COUNTS - 1; // set the load value for the 0.2 second clock


	//clear TIMER3B interrupt flags
	TIMER4_ICR_R = (TIMER_ICR_TATOCINT | TIMER_ICR_TBTOCINT); //clears TIMER4 time-out interrupt flags
	TIMER4_IMR_R |= (TIMER_IMR_TATOIM | TIMER_IMR_TBTOIM); //enable TIMER4(A&B) time-out interrupts

	//initialize local interrupts
	//  n = 0, 1, 2, 3, or 4
	NVIC_EN2_R |= 0xC0;
	//go to page 105 and find the corresponding interrupt numbers for TIMER4 A&B
	//then set those bits in the correct interrupt set EN register (p. 142)
	
	IntRegister(INT_TIMER4A, TIMER4A_Handler); //register TIMER4A interrupt handler
	IntRegister(INT_TIMER4B, TIMER4B_Handler); //register TIMER4B interrupt handler

	IntMasterEnable(); //Initialize global interrupts

	TIMER4_CTL_R |= (TIMER_CTL_TAEN | TIMER_CTL_TBEN); //Enable TIMER4A & TIMER4B
}

TimerPartOne(void)
{
    while(1)
    {

        uint8_t button = 0;
        button = button_getButton();
        if(button == SW6)
        {
            hour = (hour+1) % 25;
            lcd_printf("%02d:%02d:%02d", hour, min, sec);
        }

        if(button == SW5)
        {
             if(hour == 0)
            {
                hour = 25;
            }
            hour--;
            lcd_printf("%02d:%02d:%02d", hour, min, sec);
        }
        if(button == SW4)
        {
            min = (min+1) % 60;
            lcd_printf("%02d:%02d:%02d", hour, min, sec);
        }
        if(button == SW3)
        {
            if(min == 0)
            {
                min = 60;
            }
            min--;
            lcd_printf("%02d:%02d:%02d", hour, min, sec);
        }
        if(button == SW2)
        {
            sec = (sec+1) % 60;
            lcd_printf("%02d:%02d:%02d", hour, min, sec);
        }
        if(button == SW1)
        {
            if(sec == 0)
            {
                sec = 60;
            }
            sec--;
            lcd_printf("%02d:%02d:%02d", hour, min, sec);
        }
        if(count > 4)
        {
            sec = (sec+1) % 60;
            if(sec == 0)
            {
                min = (min+1) % 60;
            }
            if(sec == 0 && min == 0)
            {
                hour = (hour+1) % 24;
            }
            count = 0;
            lcd_printf("%02d:%02d:%02d", hour, min, sec);
        }

        else
        {
            count++;
            lcd_printf("%02d:%02d:%02d", hour, min, sec);
            timer_waitMillis(200);
        }
    }
}
/// Displays the time of day on the LCD
/**
 * Displays the time of day on the LCD in the format "HH:MM:SS".  Allows the user to adjust the time using
 * the push buttons.  The push buttons will be checked every 200ms, and will update the screen every time
 * the time is adjusted.
 *
 * SW6 increment hours
 * SW5 decrement hours
 * SW4 increment minutes
 * SW3 decrement minutes
 * SW2 increment seconds
 * SW1 decrement seconds
 */

void main()
{
    lcd_init();
        button_init();
        clock_timer_init();


    // TimerPartOne();
    while (1)
    {
        lcd_printf("%02d:%02d:%02d", hour, min, sec);
    }

}

