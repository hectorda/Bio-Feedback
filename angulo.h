#ifndef ANGULO_H
#define ANGULO_H

#include <muestra.h>
#include <QString>
#include <QtMath>

class Angulo
{
private:
    double Tiempo;
    double AnguloX;
    double AnguloY;

public:
    Angulo();
    Angulo(double Tiempo,double AnguloX,double AnguloY);
    double getTiempo();
    double getAnguloX();
    double getAnguloY();
    void setTiempo(const double tiempo);
    void setAnguloX(const double angulo);
    void setAnguloY(const double angulo);
    void calcularAngulo(const QString orientacion, Muestra *muestra);
    void calcularAnguloFiltroComplementario(const QString orientacion, Muestra *muestra, Angulo *anguloAnterior);
};

#endif // ANGULO_H
