#include "desplazamiento.h"
Desplazamiento::Desplazamiento()
{

}

Desplazamiento::Desplazamiento(double Tiempo, double despProy1, double despProy2, double despRec1, double despRec2)
{
    this->Tiempo=Tiempo;
    desplazamientoProyeccion.Desplazamiento1=despProy1;
    desplazamientoProyeccion.Desplazamiento2=despProy2;
    desplazamientoRecorridoCurvo.Desplazamiento1=despRec1;
    desplazamientoRecorridoCurvo.Desplazamiento2=despRec2;
}

double Desplazamiento::getTiempo() const
{
    return Tiempo;
}

void Desplazamiento::setTiempo(double value)
{
    Tiempo = value;
}

Desplazamiento::desplazamientos Desplazamiento::getDesplazamientoProyeccion() const
{
    return desplazamientoProyeccion;
}

void Desplazamiento::setDesplazamientoProyeccion(const desplazamientos &value)
{
    desplazamientoProyeccion = value;
}

Desplazamiento::desplazamientos Desplazamiento::getDesplazamientoRecorridoCurvo() const
{
    return desplazamientoRecorridoCurvo;
}

void Desplazamiento::setDesplazamientoRecorridoCurvo(const desplazamientos &value)
{
    desplazamientoRecorridoCurvo = value;
}

void Desplazamiento::calcularDesplazamiento(Angulo *angulo,double alturaDispositivo)
{
    const double despPro1=qSin(qDegreesToRadians(angulo->getAngulo1()));
    const double despPro2=qSin(qDegreesToRadians(angulo->getAngulo2()));
    this->desplazamientoProyeccion.Desplazamiento1=despPro1*alturaDispositivo;
    this->desplazamientoProyeccion.Desplazamiento2=despPro2*alturaDispositivo;

    const double despReco1=M_PI*(angulo->getAngulo1()/180);
    const double despReco2=M_PI*(angulo->getAngulo2()/180);
    this->Tiempo=angulo->getTiempo();
    this->desplazamientoRecorridoCurvo.Desplazamiento1=despReco1*alturaDispositivo;
    this->desplazamientoRecorridoCurvo.Desplazamiento2=despReco2*alturaDispositivo;
}
