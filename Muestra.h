#ifndef MUESTRA_H
#define MUESTRA_H


class Muestra
{
private:
    double Tiempo;
    double AcX;
    double AcY;
    double AcZ;
    double GyX;
    double GyY;
    double GyZ;


public:
    Muestra();
    Muestra(double Tiempo,double AcX,double AcY,double AcZ,double GyX,double GyY,double GyZ);
    double getTiempo();
    double getAcX();
    double getAcY();
    double getAcZ();
    double getGyX();
    double getGyY();
    double getGyZ();

};

#endif // MUESTRA_H
