/*
 * adc.c
 *
 *  Created on: Jun 23, 2018
 *      Author: hr24612
 */

/**
 * Initializes ADC
 */

#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"

void init_ADC(void){
    //enable ADC 0 module on port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //enable clock for ADC
    SYSCTL_RCGCADC_R |= 0x1;
    //enable port B pin 4 to work as alternate functions
    GPIO_PORTB_AFSEL_R |= 0x01;
    //set pin to input - 0
    GPIO_PORTB_DEN_R &= 0b11111110;
    //disable analog isolation for the pin
    GPIO_PORTB_AMSEL_R |= 0x01;
    //initialize the port trigger source as processor (default)
    GPIO_PORTB_ADCCTL_R = 0x00;
    //disable SS0 sample sequencer to configure it
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
    //initialize the ADC trigger source as processor (default)
    ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
    //set 1st sample to use the AIN10 ADC pin
    ADC0_SSMUX0_R |= 0x000A;
    //enable raw interrupt
    ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);
    //enable oversampling to average
    ADC0_SAC_R |= ADC_SAC_AVG_64X;
    //re-enable ADC0 SS0
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
}

///function to get a reading
/*
 * @return returns the uninterpreted reading from the sensor
 */
unsigned int ADC_read(void){
    //initiate SS1 conversion
    ADC0_PSSI_R=ADC_PSSI_SS0;
    //wait for ADC conversion to be complete
    while((ADC0_RIS_R & ADC_RIS_INR0) == 0){
    //wait
    }
    //grab result
    int value = ADC0_SSFIFO0_R;
    return value;
}

///gets an average of IR samples
/*
 * @param number of samples to take
*/
int getIRAverage(int num){
    int toAverage=0;

    //average num reads
    int i;
    for(i=0; i<num; i++){
        //get a reading
        toAverage+=ADC_read();
    }
    int average = toAverage/num;
    return average;
}
