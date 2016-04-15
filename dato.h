#ifndef DATO_H
#define DATO_H


class Dato
{
private:
    double anguloX;
    double anguloY;
    double tiempo;

public:
    Dato(double anguloX,double anguloY,double tiempo);
    double getAnguloX();
    double getAnguloY();
    double getTiempo();
};

#endif // DATO_H
