#include "dato.h"

Dato::Dato(double angleX, double angleY, double time)
{
    this->anguloX=angleX;
    this->anguloY=angleY;
    this->tiempo=time;
}

double Dato::getAnguloX()
{
    return this->anguloX;
}

double Dato::getAnguloY()
{
    return this->anguloY;
}

double Dato::getTiempo()
{
    return this->tiempo;
}
