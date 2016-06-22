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
    double Angulo1;
    double Angulo2;

public:
    Angulo();
    Angulo(double Tiempo,double Angulo1,double Angulo2);

    Kalman kalmanX; // Create the Kalman instances
    Kalman kalmanY;

    double getTiempo();
    double getAngulo1();
    double getAngulo2();
    void setTiempo(const double tiempo);
    void setAngulo1(const double angulo);
    void setAngulo2(const double angulo);
    void calcularAngulo(const QString orientacion, Muestra *muestra);
    void calcularAnguloFiltroComplementario(const QString orientacion, Muestra *muestra, Angulo *anguloAnterior, const double alpha);
    void setAnguloInicialKalman(const double anguloX, const double anguloY);
    void calcularAnguloFiltroKalman(const QString orientacion, Muestra *muestra, Angulo *anguloAnterior);
};

#endif // ANGULO_H
