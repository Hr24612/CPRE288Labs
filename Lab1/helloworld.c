#include "lcd.h"

/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 */
int main (void) {
    lcd_init();


        char c[] = "                    Microcontrollers are lots of fun!";
        int size = sizeof(c)/sizeof(c[0]);

        char Space[21] = "                    ";

        int i;
        int j;

        for(i = 0; i < size; i++){
           for(j = 0; j < 20; j++){
               Space[j] = c[j+i];
           }
           lcd_printf(Space);

           timer_waitMillis(300);

           if(i == size-1){
               i = 0; // reset
           }
        }

        return 0;
}
