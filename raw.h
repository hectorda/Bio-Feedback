#ifndef RAW_H
#define RAW_H


class Raw
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
    Raw(double Tiempo,double AcX,double AcY,double AcZ,double GyX,double GyY,double GyZ);
    double getTiempo();
    double getAcX();
    double getAcY();
    double getAcZ();
    double getGyX();
    double getGyY();
    double getGyZ();

};

#endif // RAW_H
