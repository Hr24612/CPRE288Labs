/*
 * WiFi.c
 *
 *  Created on: Sep 25, 2016
 *      Author: ericm
 */
/*
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "timer.h"
#include "uart.h"

#define COMMAND_PIN		0x04

#define COMMAND_START	0
#define COMMAND_STOP	1

#define RETVAL_SUCCESS	0

//void uart_init(void);
//void uart_sendChar(char data);
//char uart_receive(void);
//void uart_sendStr(char data[]);
void WiFi_start(char *psk);
/*
int WiFi_stop() {
	return _sendCommand(COMMAND_STOP, NULL, 0);
}
uint8_t _sendCommand(uint8_t command, uint8_t* param, uint8_t len) {
	//Raise command flag
	GPIO_PORTB_DATA_R |= COMMAND_PIN;

	//Send WiFi command
	uart_sendChar(command);

	//Send parameter (if applicable)
	if(len > 0) {
		uart_sendBuffer(param, len);
	}

	//Wait for response from WiFi microcontroller
	int response = uart_receive();

	//Lower the command flag
	GPIO_PORTB_DATA_R &= ~COMMAND_PIN;

	return response;
}

main(void) {

	button_init();
	int number = 0;
	uart_init();
	lcd_init();
	WiFi_start();
	char chr[21];
	chr[20] = '\0';
	//char SW6[] = "Yes";
	//char SW5[] = "No";
	//char SW4[] = "Blue, no green, Ahhhhh!!!";
	//char SW3[] = "hmmmmm";
	//char SW2[] = "BiuBiu";
	//char SW1[] = "I am number 1";
	//char *check;


	//while (1) {
		//int button = button_checkButtons();

		if(button ==1 && check != SW1)
		{
			check = SW1;
			uart_sendStr(SW1);
			lcd_printf("%s", SW1);
		}
		if(button ==2 && check != SW2)
		{
			check = SW2;
			lcd_printf("%s", SW2);
			uart_sendStr(SW2);
		}
		if(button ==4 && check != SW3)
		{
			check = SW3;
			lcd_printf("%s", SW3);
			uart_sendStr(SW3);
		}
		if(button ==8 && check != SW4)
		{
			check = SW4;
			lcd_printf("%s", SW4);
			uart_sendStr(SW4);
		}
		if(button ==16 && check != SW5)
		{
			check = SW5;
			lcd_printf("%s", SW5);
			uart_sendStr(SW5);
		}
		if(button ==32 && check != SW6)
		{
			check = SW6;
			lcd_printf("%s", SW6);
			uart_sendStr(SW6);
		}

		while (number < 20) {

			chr[number] = uart_receive();

			uart_sendChar(chr[number]);
			if (chr[number] != '\r') {
				lcd_printf("%c \nnumber: %d", chr[number], number + 1);
				number++;
			} else {
				uart_sendChar('\n');
				chr[number] = '\0';
				break;
			}

		}
		lcd_printf("%s \nnumber: %d", chr, number);
		number = 0;

	//}
}
*/
/*
//Initialize USART1 to a given baud rate
void uart_init(void) {
	//enable clock to GPIO, R1 = port B
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	//enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx (See
	//DataSheet)
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
	//enable alternate functions on port b pins 0 and 1
	GPIO_PORTB_AFSEL_R |= (0x3);
	//enable Rx and Tx on port B on pins 0 and 1
	GPIO_PORTB_PCTL_R |= 0x00000011;
	//set pin 0 and 1 to digital
	GPIO_PORTB_DEN_R |= (0x3);
	//set pin 0 to Rx or input
	GPIO_PORTB_DIR_R &= ~0x1;
	//set pin 1 to Tx or output
	GPIO_PORTB_DIR_R |= 0x2;
	uint16_t iBRD = 8; //use equations
	uint16_t fBRD = 44; //use equations
	//turn off uart1 while we set it up
	UART1_CTL_R &= ~(UART_CTL_UARTEN);
	//set baud rate
	UART1_IBRD_R = iBRD;
	UART1_FBRD_R = fBRD;
	//set frame, 8 data bits, 1 stop bit, no parity, no FIFO
	UART1_LCRH_R = UART_LCRH_WLEN_8;
	//use system clock as source
	UART1_CC_R = UART_CC_CS_SYSCLK;
	//re-enable enable RX, TX, and uart1
	UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
}

void uart_sendChar(char data) {
	//wait until there is room to send data
	while ((UART1_FR_R & 0x20) != 0)
		;

	//send data
	UART1_DR_R = data;
}

void uart_sendStr(char data[])
{
	int i=0;

	while(data[i] != 0)
	{
		uart_sendChar(data[i]);
		i++;
	}
}

char uart_receive(void) {
	char data = 0;
	//wait to receive
	while ((UART1_FR_R & UART_FR_RXFE) != 0)
		;
	//mask the 4 error bits and grab only 8
//	data bits
	data = (char) (UART1_DR_R & 0xFF);
	return data;
}
*/

void WiFi_start(char *psk){

	uart_init();
	GPIO_PORTB_DEN_R |= BIT2;
	GPIO_PORTB_DIR_R |= BIT2;
	GPIO_PORTB_DATA_R |= BIT2;

	uart_sendChar(0x00); //Send command
	uart_sendStr(psk); //Send WiFi PSK
	uart_sendChar('\0'); //NULL terminator
	char response = uart_receive(); //Wait for response
	GPIO_PORTB_DIR_R &= ~BIT2;

}
//
//void WiFi_stop(){
//
//	char WIFI_STOP_COMMAND[] = "STOP";
//	int commandPin = 1; //Enter command mode
//	uart_sendChar(WIFI_STOP_COMMAND); //Send command
//	int response = uart_waitForChar(); //Wait for response
//	commandPin = 0; //Leave command mode
//	if(response != 0) {
//		//An error occurred…
//		;
//	}
//
//
//}


/*
#include <string.h>

#include "WiFi.h"

#define COMMAND_PIN		0x04

#define COMMAND_START	0
#define COMMAND_STOP	1

#define RETVAL_SUCCESS	0


void uart_init(void);
uint8_t _sendCommand(uint8_t command, uint8_t* param, uint8_t len);

int WiFi_start(char *psk) {
	//Initialize UART
	//TODO call the correct init function for your uart
	TODO
	uart_init();

	//Initialize command pin for WiFi connection
	SYSCTL_RCGCGPIO_R |= BIT1; //Turn on GPIO for WiFi command mode
	GPIO_PORTB_DEN_R |= COMMAND_PIN; //Enable pin2
	GPIO_PORTB_DIR_R |= COMMAND_PIN;
	GPIO_PORTB_DATA_R &= ~COMMAND_PIN; //Clear command pin

	//Wait for pin to settle
	timer_waitMillis(1);

	uint8_t response = _sendCommand(COMMAND_START, psk, strlen(psk) + 1);

	if(response != RETVAL_SUCCESS) {
		return -1;
	}
	else {
		return 0;
	}
}

int WiFi_stop() {
	return _sendCommand(COMMAND_STOP, NULL, 0);
}

uint8_t _sendCommand(uint8_t command, uint8_t* param, uint8_t len) {
	//Raise command flag
	GPIO_PORTB_DATA_R |= COMMAND_PIN;

	//Send WiFi command
	uart_sendChar(command);

	//Send parameter (if applicable)
	if(len > 0) {
		uart_sendBuffer(param, len);
	}

	//Wait for response from WiFi microcontroller
	int response = uart_receive();

	//Lower the command flag
	GPIO_PORTB_DATA_R &= ~COMMAND_PIN;

	return response;
}
void uart_init(void) {
	//enable clock to GPIO, R1 = port B
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
	//enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx (See
	//DataSheet)
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
	//enable alternate functions on port b pins 0 and 1
	GPIO_PORTB_AFSEL_R |= (0x3);
	//enable Rx and Tx on port B on pins 0 and 1
	GPIO_PORTB_PCTL_R |= 0x00000011;
	//set pin 0 and 1 to digital
	GPIO_PORTB_DEN_R |= (0x3);
	//set pin 0 to Rx or input
	GPIO_PORTB_DIR_R &= ~0x1;
	//set pin 1 to Tx or output
	GPIO_PORTB_DIR_R |= 0x2;
	uint16_t iBRD = 8; //use equations
	uint16_t fBRD = 44; //use equations
	//turn off uart1 while we set it up
	UART1_CTL_R &= ~(UART_CTL_UARTEN);
	//set baud rate
	UART1_IBRD_R = iBRD;
	UART1_FBRD_R = fBRD;
	//set frame, 8 data bits, 1 stop bit, no parity, no FIFO
	UART1_LCRH_R = UART_LCRH_WLEN_8;
	//use system clock as source
	UART1_CC_R = UART_CC_CS_SYSCLK;
	//re-enable enable RX, TX, and uart1
	UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
}
*/


