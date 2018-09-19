
/**
 * author: Amir Hamza
 *
 *
 *
 */


#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"

//void ADC0SS1_Handler(void)
//{
//    //initiate SS1 conversion
//    ADC0_PSSI_R = ADC_PSSI_SS1;
//    //wait for ADC conversion to be complete
//    while ((ADC0_RIS_R & ADC_RIS_INR1) == 0)
//    {
//        //wait
////        lcd_printf("waiting...");
////        timer_waitMillis(500);
//    }
//    //grab result
//    int value = (ADC0_SSFIFO1_R & 0x0FFF);
////    lcd_printf("QV %i, Dist",value);
//    //clear interrupt flags
//    ADC0_ISC_R = ADC_ISC_IN1;
//    timer_waitMillis(500);
//
//}

/**
 * Initializes ADC
 */
void init_ADC(void)
{
    //enable clock for port B (PB4, AIN10 for input signal) (1 of 6)
    SYSCTL_RCGCGPIO_R |= 0b000010;

    //enable clock for ADC0 (bit 0 of only 2)
    SYSCTL_RCGCADC_R |= 0x1;

    //enable port B pin 4 to work as alternate functions
    GPIO_PORTB_AFSEL_R |= 0x10;

    //set pin 4 port B to input (0)
    GPIO_PORTB_DIR_R &= 0b11101111;

    //disable analog isolation for the pin
    GPIO_PORTB_AMSEL_R |= 0x10;

    //initialize the port trigger source as processor (default)
    GPIO_PORTB_ADCCTL_R = 0x00;

    //disable SS1 sample sequencer to configure it
    ADC0_ACTSS_R &= ~(0x00000002);

    //initialize the ADC trigger source as processor (default)
    ADC0_EMUX_R &= ~(0x000000F0);

    //set 1st sample to use the AIN10 ADC pin (8 MUX samples, 12 analog pins)
    ADC0_SSMUX1_R |= 0x000A;

    //enable raw interrupt status, 1st sample raw interrupt enable bit 2, 1st sample is end of sequence bit 1
    ADC0_SSCTL1_R |= (0x0004 | 0x0002);

    //enable oversampling to average 0x0 or 16X
    //ADC0_SAC_R |= 0x0;
    ADC0_SAC_R |= 0x4;

    //not using interrupts right now
    //clear interrupt flags
    ADC0_ISC_R |= 0x00000002;
    //enable    ADC0SS1 interrupt
    ADC0_IM_R |= 0x00000002;
    //enable interrupt for IRQ 15 set bit 15
//    NVIC_EN0_R |= 0x00008000;
//    //tell cpu to use ISR handler for ADC0SS1
//    IntRegister(INT_ADC0SS1, ADC0SS1_Handler);
//    //enable global interrupts
//    IntMasterEnable();

    //re-enable ADC0 SS1
    ADC0_ACTSS_R |= 0x00000002;
}


/**
 * polls ADC raw status interrupt and grabs value
 * @return
 *  converted value
 */
int Polling_ADC(void)
{
    //initiate SS1 conversion
    ADC0_PSSI_R = ADC_PSSI_SS1;
    //wait for ADC conversion to be complete
    while ((ADC0_RIS_R & ADC_RIS_INR1) == 0)
    {
        //wait
    }
    //grab result
    int value = (ADC0_SSFIFO1_R & 0x0FFF);
    ADC0_ISC_R = ADC_ISC_IN1;
    return value;
}

/**
 * calculates distance using the ADC result, doesn't use any averaging
 */
/*float getDistance1(int d)
{

    float table[] = { 56.0, 51.5, 47.6, 44.1, 41.0, 38.3, 35.8, 33.6, 31.6,
                      29.8, 28.1, 26.6, 25.2, 24.0, 22.8, 21.7, 20.7, 19.8,
                      18.9, 18.1, 17.4, 16.6, 16.0, 15.4, 14.8, 14.3, 13.7,
                      13.3, 12.8, 12.4, 12.0, 11.6, 11.2, 10.9, 10.5, 10.2, 9.9,
                      9.6, 9.3, 9.1, 8.8, 8.6, 8.4, 8.1, 7.9, 7.7, 7.5, 7.4,
                      7.2, 7.0, 6.8, 6.7, 6.5, 6.4, 6.2, 6.1, 6.0, 5.8, 5.7,
                      5.6, 5.5, 5.4, 5.3 };

    //binary search for closest value
    int first = 0;
    int last = 62;
    int middle = (last - first) / 2;

    if ((first * 50 + 950) > d)
    {
        return table[first];
    }
    if ((last * 50 + 950) < d)
    {
        return table[last];
    }

    while (last > first)
    {
        if ((middle * 50 + 1000) <= d)
        {
            first = middle + 1;
            middle = (last - first) / 2 + first;
        }
        else if ((middle * 50 + 950) <= d)
        {
            return table[middle];
        }
        else
        {
            last = middle - 1;
            middle = (last - first) / 2 + first;
        }
    }
    return 0;
}*/

/**
 * calculates distance using the ADC result, uses X16 averaging
 */
float getDistance2(int d)
{
    //table goes from digital values 650~2820 bin of 35

    float table[] = { 51.3, 48.2, 45.4, 42.9, 40.7, 38.7, 36.8, 35.1, 33.6,
                      32.1, 30.8, 29.6, 28.4, 27.4, 26.4, 25.5, 24.6, 23.8,
                      23.0, 22.3, 21.6, 21.0, 20.4, 19.8, 19.2, 18.7, 18.2,
                      17.7, 17.3, 16.9, 16.5, 16.1, 15.7, 15.3, 15.0, 14.7,
                      14.3, 14.0, 13.7, 13.5, 13.2, 12.9, 12.7, 12.4, 12.2,
                      12.0, 11.8, 11.5, 11.3, 11.1, 10.9, 10.8, 10.6, 10.4,
                      10.2, 10.1, 9.9, 9.8, 9.6, 9.5, 9.3, 9.2, 9.0};

    //binary search for closest value
    int first = 0;
    int last = 62;
    int middle = (last - first) / 2;

    if ((first * 35 + 650) > d)
    {
        return table[first];
    }
    if ((last * 35 + 650) < d)
    {
        return table[last];
    }

    while (last > first)
    {
        if ((middle * 35 + 685) <= d)
        {
            first = middle + 1;
            middle = (last - first) / 2 + first;
        }
        else if ((middle * 35 + 650) <= d)
        {
            return table[middle];
        }
        else
        {
            last = middle - 1;
            middle = (last - first) / 2 + first;
        }
    }
    return 0;
}

int main(void)
{
    lcd_init();
    init_ADC();

    int digitalValue;
    float distance;

    while (1)
    {
        digitalValue = Polling_ADC();

       if(getDistance2(digitalValue) > 0){
        distance = getDistance2(digitalValue) ;

            lcd_printf("QV %i\n ", digitalValue, distance);
            timer_waitMillis(100);
        }
    }
}
