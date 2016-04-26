#include "dato.h"

Dato::Dato(double tiempo,double AcX, double AcY, double AcZ, double GyX, double GyY, double GyZ)
{
    this->tiempo=tiempo;
    this->AcX=AcX;
    this->AcY=AcY;
    this->AcZ=AcZ;
    this->GyX=GyX;
    this->GyY=GyY;
    this->GyZ=GyZ;
}

double Dato::getAcX()
{
    return this->AcX;
}

double Dato::getAcY()
{
    return this->AcY;
}

double Dato::getAcZ()
{
    return this->AcZ;
}

double Dato::getGyX()
{
    return this->GyX;
}

double Dato::getGyY()
{
    return this->GyY;
}

double Dato::getGyZ()
{
    return this->GyZ;
}

double Dato::getTiempo()
{
    return this->tiempo;
}
