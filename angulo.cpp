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

void Angulo::calcularAngulo(const QString orientacion,Muestra *muestra){
    double anguloX=0,anguloY=0;
    if(orientacion.contains("vertical"))
    {
        if(orientacion.contains("atras"))
        {
            //Se calculan los angulos con la IMU vertical.
            anguloX = qRadiansToDegrees(qAtan(-muestra->getAcX()/qSqrt(qPow(muestra->getAcZ(),2) + qPow(muestra->getAcY(),2))));
            anguloY = qRadiansToDegrees(qAtan(muestra->getAcZ()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcY(),2))));
        }
        if(orientacion.contains("frente"))
        {
            //Se calculan los angulos con la IMU vertical.
            anguloX = qRadiansToDegrees(qAtan(muestra->getAcX()/qSqrt(qPow(muestra->getAcZ(),2) + qPow(muestra->getAcY(),2))));
            anguloY = qRadiansToDegrees(qAtan(-muestra->getAcZ()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcY(),2))));
        }
        if(orientacion.contains("derecha"))
        {
            //Se calculan los angulos con la IMU vertical.
            anguloX = qRadiansToDegrees(qAtan(-muestra->getAcZ()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcY(),2))));
            anguloY = qRadiansToDegrees(qAtan(-muestra->getAcX()/qSqrt(qPow(muestra->getAcZ(),2) + qPow(muestra->getAcY(),2))));
        }
        if(orientacion.contains("izquierda"))
        {
            //Se calculan los angulos con la IMU vertical.
            anguloX = qRadiansToDegrees(qAtan(muestra->getAcZ()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcY(),2))));
            anguloY = qRadiansToDegrees(qAtan(muestra->getAcX()/qSqrt(qPow(muestra->getAcZ(),2) + qPow(muestra->getAcY(),2))));
        }
    }
    if(orientacion.contains("horizontal"))
    {
        if(orientacion.contains("arriba"))
        {
            //Se calculan los angulos con la IMU horizontal.
            anguloX = qRadiansToDegrees(qAtan(muestra->getAcY()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcZ(),2))));
            anguloY = qRadiansToDegrees(qAtan(-muestra->getAcX()/qSqrt(qPow(muestra->getAcY(),2) + qPow(muestra->getAcZ(),2))));
        }
        if(orientacion.contains("abajo"))
        {
            //Se calculan los angulos con la IMU horizontal.
            anguloX = qRadiansToDegrees(qAtan(muestra->getAcY()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcZ(),2))));
            anguloY = qRadiansToDegrees(qAtan(muestra->getAcX()/qSqrt(qPow(muestra->getAcY(),2) + qPow(muestra->getAcZ(),2))));
        }
    }
    this->setTiempo(muestra->getTiempo());
    this->setAnguloX(anguloX);
    this->setAnguloY(anguloY);
}

void Angulo::calcularAnguloFiltroComplementario(const QString orientacion, Muestra *muestra,Angulo *anguloAnterior,const double alpha)
{
    double anguloComplementario1=0,anguloComplementario2=0;
    const double dt=(muestra->getTiempo() - anguloAnterior->getTiempo());
    Angulo *anguloSinFiltro = new Angulo();
    anguloSinFiltro->calcularAngulo(orientacion,muestra);//Se obtiene el Angulo Usando Euler

    if(orientacion.contains("vertical"))
    {
        if(orientacion.contains("atras"))
        {
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()-muestra->getGyZ()*dt) + alpha*anguloSinFiltro->getAnguloX();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()-muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloY();;
        }
        if(orientacion.contains("frente"))
        {
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()+muestra->getGyZ()*dt) + alpha*anguloSinFiltro->getAnguloX();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()-muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloY();

        }
        if(orientacion.contains("izquierda"))
        {
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()-muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloX();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()+muestra->getGyZ()*dt) + alpha*anguloSinFiltro->getAnguloY();
        }
        if(orientacion.contains("derecha"))
        {
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()+muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloX();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()-muestra->getGyZ()*dt) + alpha*anguloSinFiltro->getAnguloY();
        }
    }
    if(orientacion.contains("horizontal"))
    {
        if(orientacion.contains("arriba"))
        {
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()+muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloX();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()+muestra->getGyY()*dt) + alpha*anguloSinFiltro->getAnguloY();

        }
        if(orientacion.contains("abajo")){
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAnguloX()-muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAnguloX();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAnguloY()+muestra->getGyY()*dt) + alpha*anguloSinFiltro->getAnguloY();
        }
    }
    this->setTiempo(muestra->getTiempo());
    this->setAnguloX(anguloComplementario1);
    this->setAnguloY(anguloComplementario2);
}

void Angulo::setAnguloInicialKalman(const double anguloX,const double anguloY){
    kalmanX.setAngle(anguloX);
    kalmanY.setAngle(anguloY);
}

void Angulo::calcularAnguloFiltroKalman(const QString orientacion, Muestra *muestra, Angulo *anguloAnterior)
{
    double anguloKalmanX=0,anguloKalmanY=0;
    Angulo *anguloSinFiltro = new Angulo();
    anguloSinFiltro->calcularAngulo(orientacion,muestra);
    const double dt=(muestra->getTiempo() - anguloAnterior->getTiempo());
    if(orientacion.contains("vertical"))
    {
        if(orientacion.contains("atras"))
        {
            anguloKalmanX = kalmanX.getAngle(anguloSinFiltro->getAnguloX(), -muestra->getGyZ(), dt);
            anguloKalmanY = kalmanY.getAngle(anguloSinFiltro->getAnguloY(), -muestra->getGyX(), dt);
        }
        if(orientacion.contains("frente"))
        {
            anguloKalmanX = kalmanX.getAngle(anguloSinFiltro->getAnguloX(), +muestra->getGyZ(), dt);
            anguloKalmanY = kalmanY.getAngle(anguloSinFiltro->getAnguloY(), -muestra->getGyX(), dt);
        }
        if(orientacion.contains("izquierda"))
        {
            anguloKalmanX = kalmanX.getAngle(anguloSinFiltro->getAnguloX(), -muestra->getGyX(), dt);
            anguloKalmanY = kalmanY.getAngle(anguloSinFiltro->getAnguloY(), +muestra->getGyZ(), dt);
        }
        if(orientacion.contains("derecha"))
        {
            anguloKalmanX = kalmanX.getAngle(anguloSinFiltro->getAnguloX(), +muestra->getGyX(), dt);
            anguloKalmanY = kalmanY.getAngle(anguloSinFiltro->getAnguloY(), -muestra->getGyZ(), dt);
        }
    }
    if(orientacion.contains("horizontal"))
    {
        if(orientacion.contains("arriba"))
        {
            anguloKalmanX = kalmanX.getAngle(anguloSinFiltro->getAnguloX(), muestra->getGyX(), dt);
            anguloKalmanY = kalmanY.getAngle(anguloSinFiltro->getAnguloY(), muestra->getGyY(), dt);
        }
        if(orientacion.contains("abajo")){
            anguloKalmanX = kalmanX.getAngle(anguloSinFiltro->getAnguloX(), -muestra->getGyX(), dt);
            anguloKalmanY = kalmanY.getAngle(anguloSinFiltro->getAnguloY(), muestra->getGyY(), dt);
        }

    }
    this->setTiempo(muestra->getTiempo());
    this->setAnguloX(anguloKalmanX);
    this->setAnguloY(anguloKalmanY);

}
