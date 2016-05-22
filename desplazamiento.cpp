#include "desplazamiento.h"

Desplazamiento::Desplazamiento(double tiempo,double desplazamientoX,double desplazamientoY)
{
    this->Tiempo=tiempo;
    this->DesplazamientoX=desplazamientoX;
    this->DesplazamientoY=desplazamientoY;
}

double Desplazamiento::getTiempo()
{
    return this->Tiempo;
}

double Desplazamiento::getDesplazamientoX()
{
    return this->DesplazamientoX;
}

double Desplazamiento::getDesplazamientoY()
{
    return this->DesplazamientoY;
}
