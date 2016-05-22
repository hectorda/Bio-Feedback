#ifndef DESPLAZAMIENTO_H
#define DESPLAZAMIENTO_H

class Desplazamiento
{
private:
    double Tiempo;
    double DesplazamientoX;
    double DesplazamientoY;

public:
    Desplazamiento(double tiempo, double desplazamientoX, double desplazamientoY);
    double getTiempo();
    double getDesplazamientoX();
    double getDesplazamientoY();
};

#endif // DESPLAZAMIENTO_H
