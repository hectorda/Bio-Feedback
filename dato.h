#ifndef DATO_H
#define DATO_H


class Dato
{
private:
    double tiempo;
    double AcX;
    double AcY;
    double AcZ;
    double GyX;
    double GyY;
    double GyZ;


public:
    Dato(double tiempo,double AcX,double AcY,double AcZ,double GyX,double GyY,double GyZ);
    double getTiempo();
    double getAcX();
    double getAcY();
    double getAcZ();
    double getGyX();
    double getGyY();
    double getGyZ();

};

#endif // DATO_H
