#include "angulo.h"

Angulo::Angulo()
{

}

Angulo::Angulo(double Tiempo,double AnguloX,double AnguloY)
{
    this->Tiempo=Tiempo;
    this->AnguloX=AnguloX;
    this->AnguloY=AnguloY;
}

double Angulo::getTiempo()
{
    return this->Tiempo;
}

double Angulo::getAnguloX()
{
    return this->AnguloX;
}

double Angulo::getAnguloY()
{
    return this->AnguloY;
}

void Angulo::setTiempo(const double tiempo)
{
    this->Tiempo=tiempo;
}

void Angulo::setAnguloX(const double angulo)
{
    this->AnguloX=angulo;
}

void Angulo::setAnguloY(const double angulo)
{
    this->AnguloY=angulo;
}

void Angulo::calcularAngulo(const QString orientacion,Raw *raw){
    double angulo1=0,angulo2=0;
    if(orientacion.contains("vertical"))
    {
        if(orientacion.contains("atras"))
        {
            //Se calculan los angulos con la IMU vertical.
            angulo1 = qRadiansToDegrees(qAtan(-raw->getAcX()/qSqrt(qPow(raw->getAcZ(),2) + qPow(raw->getAcY(),2))));
            angulo2 = qRadiansToDegrees(qAtan(raw->getAcZ()/qSqrt(qPow(raw->getAcX(),2) + qPow(raw->getAcY(),2))));
        }
        if(orientacion.contains("frente"))
        {
            //Se calculan los angulos con la IMU vertical.
            angulo1 = qRadiansToDegrees(qAtan(raw->getAcX()/qSqrt(qPow(raw->getAcZ(),2) + qPow(raw->getAcY(),2))));
            angulo2 = qRadiansToDegrees(qAtan(-raw->getAcZ()/qSqrt(qPow(raw->getAcX(),2) + qPow(raw->getAcY(),2))));
        }
        if(orientacion.contains("derecha"))
        {
            //Se calculan los angulos con la IMU vertical.
            angulo1 = qRadiansToDegrees(qAtan(-raw->getAcZ()/qSqrt(qPow(raw->getAcX(),2) + qPow(raw->getAcY(),2))));
            angulo2 = qRadiansToDegrees(qAtan(-raw->getAcX()/qSqrt(qPow(raw->getAcZ(),2) + qPow(raw->getAcY(),2))));
        }
        if(orientacion.contains("izquierda"))
        {
            //Se calculan los angulos con la IMU vertical.
            angulo1 = qRadiansToDegrees(qAtan(raw->getAcZ()/qSqrt(qPow(raw->getAcX(),2) + qPow(raw->getAcY(),2))));
            angulo2 = qRadiansToDegrees(qAtan(raw->getAcX()/qSqrt(qPow(raw->getAcZ(),2) + qPow(raw->getAcY(),2))));
        }
    }
    if(orientacion.contains("horizontal"))
    {
        if(orientacion.contains("arriba"))
        {
            //Se calculan los angulos con la IMU horizontal.
            angulo1 = qRadiansToDegrees(qAtan(raw->getAcY()/qSqrt(qPow(raw->getAcX(),2) + qPow(raw->getAcZ(),2))));
            angulo2 = qRadiansToDegrees(qAtan(-raw->getAcX()/qSqrt(qPow(raw->getAcY(),2) + qPow(raw->getAcZ(),2))));
        }
        if(orientacion.contains("abajo"))
        {
            //Se calculan los angulos con la IMU horizontal.
            angulo1 = qRadiansToDegrees(qAtan(raw->getAcY()/qSqrt(qPow(raw->getAcX(),2) + qPow(raw->getAcZ(),2))));
            angulo2 = qRadiansToDegrees(qAtan(raw->getAcX()/qSqrt(qPow(raw->getAcY(),2) + qPow(raw->getAcZ(),2))));
        }
    }
    this->setTiempo(raw->getTiempo());
    this->setAnguloX(angulo1);
    this->setAnguloY(angulo2);
}

void Angulo::calcularAnguloFiltroComplementario(const QString orientacion, Raw *raw,Angulo *anguloAnterior)
{
    double anguloComplementario1=0,anguloComplementario2=0;
    double alpha=0.02;
    const double dt=(raw->getTiempo() - anguloAnterior->getTiempo())/1000;
    Angulo *anguloSinFiltro = new Angulo();
    anguloSinFiltro->calcularAngulo(orientacion,raw);

    if(orientacion.contains("vertical"))
    {
        if(orientacion.contains("atras"))
        {
            //Aplicar el Filtro Complementario
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()+raw->getGyZ()*dt) + alpha*anguloSinFiltro->getAnguloX();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()+raw->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloY();;
        }
        if(orientacion.contains("frente"))
        {
            //Aplicar el Filtro Complementario
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()+raw->getGyZ()*dt) + alpha*anguloSinFiltro->getAnguloX();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()+raw->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloY();

        }
        if(orientacion.contains("derecha"))
        {
            //Aplicar el Filtro Complementario
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()+raw->getGyZ()*dt) + alpha*anguloSinFiltro->getAnguloX();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()+raw->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloY();
        }
        if(orientacion.contains("izquierda"))
        {
            //Aplicar el Filtro Complementario
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()+raw->getGyZ()*dt) + alpha*anguloSinFiltro->getAnguloX();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()+raw->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloY();
        }
    }
    if(orientacion.contains("horizontal")){
        {
            if(orientacion.contains("arriba"))
            {
                //Aplicar el Filtro Complementario
                anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()+raw->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloX();
                anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()+raw->getGyY()*dt) + alpha*anguloSinFiltro->getAnguloY();

            }
            if(orientacion.contains("abajo")){
            {
                //Aplicar el Filtro Complementario
                anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()+raw->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloX();
                anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()+raw->getGyY()*dt) + alpha*anguloSinFiltro->getAnguloY();
                }
            }
        }
    }
    this->setTiempo(raw->getTiempo());
    this->setAnguloX(anguloComplementario1);
    this->setAnguloY(anguloComplementario2);
}

