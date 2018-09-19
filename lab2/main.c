#include "open_interface.h"
#include "movement.h"

void moveforward()
{
    oi_t *sensor = oi_alloc();
    oi_init(sensor);

    move_forward(sensor, 50);

    oi_free(sensor);
}

void turn()
{
    oi_t *sensor = oi_alloc();
    oi_init(sensor);
    turn_clockwise(sensor, 90);
}

void counterTurn()
{
    oi_t *sensor = oi_alloc();
    oi_init(sensor);
    turn_clockwise(sensor, -90);
}

void squareMovement()
{
    moveforward();
    turn();
    moveforward();
    turn();
    moveforward();
    turn();
    moveforward();
}

void collisionTest()
{
    oi_t *sensor = oi_alloc();
    oi_init(sensor);

    part3(sensor, 200);

}

void PART3()
{
    collisionTest();


}

void main()
{
    PART3();
}
