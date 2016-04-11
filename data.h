#ifndef DATA_H
#define DATA_H


class Data
{
private:
    double angleX;
    double angleY;
    double time;

public:
    Data(double angleX,double angleY,double time);
    double getAngleX();
    double getAngleY();
    double getTime();
};

#endif // DATA_H
