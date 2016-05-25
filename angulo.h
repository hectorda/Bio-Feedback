#ifndef ANGULO_H
#define ANGULO_H

#include <raw.h>
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
    void calcularAngulo(const QString orientacion, Raw *raw);
    void calcularAnguloFiltroComplementario(const QString orientacion, Raw *raw, Angulo *anguloAnterior);
};

#endif // ANGULO_H
