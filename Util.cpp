#include "Util.h"

ev3::Vector2c turnLeft(ev3::Vector2c direction)
{
    int8_t newX = -direction.y;
    direction.y = direction.x;
    direction.x = newX;
    return direction;
}

ev3::Vector2c turnRight(ev3::Vector2c direction)
{
    int8_t newX = direction.y;
    direction.y = -direction.x;
    direction.x = newX;
    return direction;
}
