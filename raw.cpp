#include "raw.h"

Raw::Raw(double Tiempo,double AcX, double AcY, double AcZ, double GyX, double GyY, double GyZ)
{
    this->Tiempo=Tiempo;
    this->AcX=AcX;
    this->AcY=AcY;
    this->AcZ=AcZ;
    this->GyX=GyX;
    this->GyY=GyY;
    this->GyZ=GyZ;
}

double Raw::getAcX()
{
    return this->AcX;
}

double Raw::getAcY()
{
    return this->AcY;
}

double Raw::getAcZ()
{
    return this->AcZ;
}

double Raw::getGyX()
{
    return this->GyX;
}

double Raw::getGyY()
{
    return this->GyY;
}

double Raw::getGyZ()
{
    return this->GyZ;
}

double Raw::getTiempo()
{
    return this->Tiempo;
}
