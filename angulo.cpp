#include "angulo.h"

Angulo::Angulo(double Tiempo,double AnguloX,double AnguloY)
{
    this->Tiempo=Tiempo;
    this->AnguloX=AnguloX;
    this->AnguloY=AnguloY;
}

double Angulo::getTiempo()
{
    return this->Tiempo;
}

double Angulo::getAnguloX()
{
    return this->AnguloX;
}

double Angulo::getAnguloY()
{
    return this->AnguloY;
}

