#include "data.h"

Data::Data(double angleX, double angleY, double time)
{
    this->angleX=angleX;
    this->angleY=angleY;
    this->time=time;
}

double Data::getAngleX()
{
    return this->angleX;
}

double Data::getAngleY()
{
    return this->angleY;
}

double Data::getTime()
{
    return this->time;
}
