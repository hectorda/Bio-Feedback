#ifndef DESPLAZAMIENTO_H
#define DESPLAZAMIENTO_H

#include <angulo.h>

class Desplazamiento
{
private:
    double Tiempo;
    double DesplazamientoX;
    double DesplazamientoY;

public:
    Desplazamiento();
    Desplazamiento(double tiempo, double desplazamientoX, double desplazamientoY);
    double getTiempo();
    double getDesplazamientoX();
    double getDesplazamientoY();
    void calcularDesplazamiento(Angulo *angulo, double alturaDispositivo);
};

#endif // DESPLAZAMIENTO_H
