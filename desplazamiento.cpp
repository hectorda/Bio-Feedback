#include "desplazamiento.h"
Desplazamiento::Desplazamiento()
{

}

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

void Desplazamiento::calcularDesplazamiento(Angulo *angulo,double alturaDispositivo)
{
    const double despX=qSin(qDegreesToRadians(angulo->getAnguloX()));
    const double despY=qSin(qDegreesToRadians(angulo->getAnguloY()));
    this->Tiempo=angulo->getTiempo();
    this->DesplazamientoX=despX*alturaDispositivo;
    this->DesplazamientoY=despY*alturaDispositivo;

}
