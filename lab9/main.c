/*
 * main.c
 *
 *  Created on: Jun 26, 2018
 *      Author: hr24612
 */

#include "lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include <math.h>
#include "timer.h"
#include <driverlib/interrupt.h>
#include <uart.h>
#include "servo.h"
#include "adc.h"
#include "ping.h"
#include "uart.h"

int main(void)

{
    lcd_init();
    ping_init();
    PWM_init();
    init_ADC();
   uart_init();

    //Struct to save valus for IR, Sonar and Servo.
    typedef struct Data
    {
        double IR;
        float Angle;
        double Sonar;
    }Data;

    while (1)
    {
        int i, j = 0;
        float angle = 0;
        int sonar = 0;
        int ir = 0;
        int objects = 0;
        int is_in_object = 0;
        //  double c = 0;
        // Keeping track of values based on different angles.
        Data data_Array[92];
        for (i = 0; i <= 91; i++)
        {
            data_Array[i].Angle = 2 * i;
            angle = 2 * i;
            turn_servo(angle);
            timer_waitMillis(40);
            data_Array[i].IR = 0;
            data_Array[i].Sonar = 0;
            ir = 74692 * pow(getIRAverage(1), -1.125);
            data_Array[i].IR = ir;
            sonar = ping_read();
            data_Array[i].Sonar = sonar;

            if (is_in_object && data_Array[i].IR > 30) {
                objects++;
            }
            is_in_object = data_Array[i].IR <= 30;





            //Printing values on LCD after every two degrees.
            lcd_printf("Angle:%.1lf\nSonar:%lfcm\nIR %fcm\nObjects:%d",
                       data_Array[i].Angle, data_Array[i].Sonar,
                       data_Array[i].IR, objects);

        }
        uart_sendString("\r");
        uart_sendChar('\n');
        uart_sendString("Degrees\tSonar cm\tIR cm");
        uart_sendString("\r");
        uart_sendChar('\n');
        char temp[100];
        for (j = 0; j < 91; j++)
        {
            sprintf(temp, "%.1lf\t%lf\t%f", data_Array[j].Angle,
                    data_Array[j].Sonar, data_Array[j].IR);
            uart_sendChar('\r');
            uart_sendChar('\n');
            uart_sendString(temp);

    }
}
}

