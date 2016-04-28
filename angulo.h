#ifndef ANGULO_H
#define ANGULO_H


class Angulo
{
private:
    double Tiempo;
    double AnguloX;
    double AnguloY;

public:
    Angulo(double Tiempo,double AnguloX,double AnguloY);
    double getTiempo();
    double getAnguloX();
    double getAnguloY();
};

#endif // ANGULO_H
