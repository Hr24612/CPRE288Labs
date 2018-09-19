/*
 * urat.c
 *
 *  Created on: Jun 23, 2018
 *      Author: hr24612
 */

#include "lcd.h"
#include "timer.h"
#include <inc/tm4c123gh6pm.h>
#include <String.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <uart.h>


void uart_init(void) {

     //enable clock to GPIO, R1 = port B
     SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
     //enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx (See DataSheet)
     SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
     //enable alternate functions on port b pins 0 and 1
     GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1);
     //enable Rx and Tx on port B on pins 0 and 1
     GPIO_PORTB_PCTL_R |= 0x00000011;
     //set pin 0 and 1 to digital
     GPIO_PORTB_DEN_R |= (BIT0 | BIT1);
     //set pin 0 to Rx or input
     GPIO_PORTB_DIR_R &= ~BIT0;
     //set pin 1 to Tx or output
     GPIO_PORTB_DIR_R |= BIT1;
     //continued on next slide
     //calculate baudrate
     uint16_t iBRD = 8;    //use equations
     uint16_t fBRD = 44;    //use equations
     //turn off uart1 while we set it up
      UART1_CTL_R &= ~(UART_CTL_UARTEN);
      //set baud rate
      UART1_IBRD_R = iBRD;
      UART1_FBRD_R = fBRD;
      //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
      UART1_LCRH_R = UART_LCRH_WLEN_8 ;
      //use system clock as source
      UART1_CC_R = UART_CC_CS_SYSCLK;
      //re-enable enable RX, TX, and uart1
      UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

 }//END of uart_init()


//Blocking call that sends 1 char over UART 1
void uart_sendChar(char data){

     //wait until there is room to send data
     while(UART1_FR_R & 0x20)
    {

    }
     //send data
     UART1_DR_R = data;

}


//Blocking call to receive over uart1
//returns char with data
char uart_receive2(void){

    char data = 0;
     //wait to receive
     while(UART1_FR_R & UART_FR_RXFE)
    {

    }
    //mask the 4 error bits and grab only 8 data bits
    data = (char)(UART1_DR_R & 0xFF);

    return data;
}

char uart_receive(void){

    int j =0;
    char data = 0;
    uint8_t button = button_getButton();
    uint8_t last_button = button;
    char SW1[] = "Hey buddy!!! What's up? ";
    char SW2[] = "How is it like in Mars?";
    char SW3[] = "Null";
    char SW4[] = "Blue, no green, Ahhhhh!!!";
    char SW5[] = "No";
    char SW6[] = "Yes";

    while(UART1_FR_R & UART_FR_RXFE){
            button = button_getButton();
            if(button == 6 && button != last_button){
                 while (SW6[j] != '\0'){
                      uart_sendChar(SW6[j]);
                      j++;
                 }
                    uart_sendChar('\r');
                    uart_sendChar('\n');
                    j = 0;
            }
            else if(button == 5 && button != last_button){
                    while (SW5[j] != '\0'){
                        uart_sendChar(SW5[j]);
                        j++;
                    }
                    uart_sendChar('\r');
                    uart_sendChar('\n');
                    j=0;
            }
            else if(button == 4 && button != last_button){
                    while (SW4[j] != '\0'){
                        uart_sendChar(SW4[j]);
                        j++;
                    }
                    uart_sendChar('\r');
                    uart_sendChar('\n');
                    j=0;
            }
            else if(button == 3 && button != last_button){
                    while (SW3[j] != '\0'){
                        uart_sendChar(SW3[j]);
                        j++;
                    }
                    uart_sendChar('\r');
                    uart_sendChar('\n');
                    j=0;
            }
            else if(button == 2 && button != last_button){
                    while (SW2[j] != '\0'){
                        uart_sendChar(SW2[j]);
                        j++;
                    }
                    uart_sendChar('\r');
                    uart_sendChar('\n');
                    j=0;
            }
            else if(button == 1 && button != last_button){
                    while (SW1[j] != '\0'){
                        uart_sendChar(SW1[j]);
                        j++;
                    }
                    uart_sendChar('\r');
                    uart_sendChar('\n');
                    j=0;
            }

                last_button = button;

    }

        //mask the 4 error bits and grab only 8 data bits
    data = (char)(UART1_DR_R & 0xFF);
    return data;
}

void uart_sendString(char data[]){
    int i = 0 , j = 0;
    j = strlen(data);

    for (i=0; i<j; i++){
        uart_sendChar(data[i]);
    }
}
