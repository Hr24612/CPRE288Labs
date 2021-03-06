/*
 * lcd_stepper.h
 *
 *  Created on: Jun 6, 2018
 *      Author: hr24612
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "timer.h"

//#warning "Comment out the #define IS_STEPPER_BOARD on the next line if you are using Rumba"
//#define IS_STEPPER_BOARD

/// Extra function for the stepper motor board
uint8_t lcd_reverseNibble(uint8_t x);

/// Initialize PORTB0:6 to Communicate with LCD
void lcd_init(void);

///Send Char to LCD
void lcd_putc(char data);

///Send Character array to LCD
void lcd_puts(char data[]);

///Clear LCD Screen
void inline lcd_clear(void);

///Return Cursor to 0,0
void inline lcd_home(void);

///Goto Line on LCD - 0 Indexed
void lcd_gotoLine(uint8_t lineNum);

///Set cursor position - top left is 0,0
void lcd_setCursorPos(uint8_t x, uint8_t y);

void lcd_printf(const char *format, ...);

#endif /* LCD_H_ */
