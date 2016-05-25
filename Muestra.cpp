#include "muestra.h"

Muestra::Muestra()
{

}

Muestra::Muestra(double Tiempo,double AcX, double AcY, double AcZ, double GyX, double GyY, double GyZ)
{
    this->Tiempo=Tiempo;
    this->AcX=AcX;
    this->AcY=AcY;
    this->AcZ=AcZ;
    this->GyX=GyX;
    this->GyY=GyY;
    this->GyZ=GyZ;
}

double Muestra::getAcX()
{
    return this->AcX;
}

double Muestra::getAcY()
{
    return this->AcY;
}

double Muestra::getAcZ()
{
    return this->AcZ;
}

double Muestra::getGyX()
{
    return this->GyX;
}

double Muestra::getGyY()
{
    return this->GyY;
}

double Muestra::getGyZ()
{
    return this->GyZ;
}

double Muestra::getTiempo()
{
    return this->Tiempo;
}
