/*
 * movement.c
 *
 *  Created on: May 28, 2018
 *      Author: hr24612
 */

#include "movement.h"
uart_init();

void move_forward(oi_t *sensor, int centimeters)
{

    int sum = 0;
    if (centimeters > 0)
    {
        oi_setWheels(300, 300); // move forward;
        while (sum < centimeters * 10)
        {
            oi_update(sensor);
            sum += sensor->distance;
        }
        oi_setWheels(0, 0); // stop
    }
}

void turn_clockwise(oi_t *sensor, int degrees)
{
    double totalAngle = 0;
    if (degrees > 0)
    {
        oi_setWheels(-100, 100); // rotate clockwise
        while (totalAngle < degrees)
        {
            oi_update(sensor);
            totalAngle -= (double) sensor->angle * 1.08; // add with calibration
        }
        oi_setWheels(0, 0); // stop

    }
    else
    {
        degrees *= -1;
        oi_setWheels(100, -100);
        while (totalAngle < degrees)
        {
            oi_update(sensor);
            totalAngle += (double) sensor->angle * 1.08; // add with calibration
        }
        oi_setWheels(0, 0);
    }

}

void part3(oi_t *sensor, int Distance)
{
    Distance = Distance -20;
    int sum = 0;
    int travel = Distance;
    int c = 0;
    if (Distance > 0)
    {
        oi_setWheels(350, 350); // move forward;
        while (sum < Distance * 10)
        {
            oi_update(sensor);
            sum += sensor->distance;
            if (sensor->bumpLeft == 1 && sensor->bumpRight == 0
                    || sensor->bumpLeft == 0 && sensor->bumpRight == 1
                    || sensor->bumpLeft == 1 && sensor->bumpRight == 1)
            {
                Distance =0;
                int Sum = 0;

                oi_setWheels(-350, -350); // move backward;
                while (Sum < 150)
                {
                    oi_update(sensor);
                    Sum -= sensor->distance;

                }
                sum -= Sum;
                c = travel-(sum/10);
                turn_clockwise(sensor, 90);
                move_forward(sensor, 25);
                turn_clockwise(sensor, -90);
                move_forward(sensor, c);
                oi_setWheels(0,0);
            }
        }
        oi_setWheels(0, 0);
    }

}


