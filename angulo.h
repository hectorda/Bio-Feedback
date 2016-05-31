#ifndef ANGULO_H
#define ANGULO_H

#include <QString>
#include <QtMath>
#include <muestra.h>
#include <Kalman.h>

class Angulo
{
private:
    double Tiempo;
    double AnguloX;
    double AnguloY;

public:
    Angulo();
    Angulo(double Tiempo,double AnguloX,double AnguloY);

    Kalman kalmanX; // Create the Kalman instances
    Kalman kalmanY;

    double getTiempo();
    double getAnguloX();
    double getAnguloY();
    void setTiempo(const double tiempo);
    void setAnguloX(const double angulo);
    void setAnguloY(const double angulo);
    void calcularAngulo(const QString orientacion, Muestra *muestra);
    void calcularAnguloFiltroComplementario(const QString orientacion, Muestra *muestra, Angulo *anguloAnterior, const double alpha);
    void setAnguloInicialKalman(const double anguloX, const double anguloY);
    void calcularAnguloFiltroKalman(const QString orientacion, Muestra *muestra, Angulo *anguloAnterior);
};

#endif // ANGULO_H
