#include "angulo.h"

Angulo::Angulo()
{

}

Angulo::Angulo(double Tiempo,double AnguloX,double AnguloY)
{
    this->Tiempo=Tiempo;
    this->Angulo1=AnguloX;
    this->Angulo2=AnguloY;
}

double Angulo::getTiempo()
{
    return this->Tiempo;
}

double Angulo::getAngulo1()
{
    return this->Angulo1;
}

double Angulo::getAngulo2()
{
    return this->Angulo2;
}

void Angulo::setTiempo(const double tiempo)
{
    this->Tiempo=tiempo;
}

void Angulo::setAngulo1(const double angulo)
{
    this->Angulo1=angulo;
}

void Angulo::setAngulo2(const double angulo)
{
    this->Angulo2=angulo;
}

void Angulo::calcularAngulo(const QString orientacion,Muestra *muestra){
    double angulo1=0,angulo2=0;
    if(orientacion.contains("vertical"))
    {
        if(orientacion.contains("atras"))
        {
            //Se calculan los angulos con la IMU vertical.
            angulo1 = qRadiansToDegrees(qAtan(-muestra->getAcX()/qSqrt(qPow(muestra->getAcZ(),2) + qPow(muestra->getAcY(),2))));
            angulo2 = qRadiansToDegrees(qAtan(muestra->getAcZ()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcY(),2))));
        }
        if(orientacion.contains("frente"))
        {
            //Se calculan los angulos con la IMU vertical.
            angulo1 = qRadiansToDegrees(qAtan(muestra->getAcX()/qSqrt(qPow(muestra->getAcZ(),2) + qPow(muestra->getAcY(),2))));
            angulo2 = qRadiansToDegrees(qAtan(-muestra->getAcZ()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcY(),2))));
        }
        if(orientacion.contains("derecha"))
        {
            //Se calculan los angulos con la IMU vertical.
            angulo1 = qRadiansToDegrees(qAtan(-muestra->getAcZ()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcY(),2))));
            angulo2 = qRadiansToDegrees(qAtan(-muestra->getAcX()/qSqrt(qPow(muestra->getAcZ(),2) + qPow(muestra->getAcY(),2))));
        }
        if(orientacion.contains("izquierda"))
        {
            //Se calculan los angulos con la IMU vertical.
            angulo1 = qRadiansToDegrees(qAtan(muestra->getAcZ()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcY(),2))));
            angulo2 = qRadiansToDegrees(qAtan(muestra->getAcX()/qSqrt(qPow(muestra->getAcZ(),2) + qPow(muestra->getAcY(),2))));
        }
    }
    if(orientacion.contains("horizontal"))
    {
        if(orientacion.contains("arriba"))
        {
            //Se calculan los angulos con la IMU horizontal.
            angulo1 = qRadiansToDegrees(qAtan(muestra->getAcY()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcZ(),2))));
            angulo2 = qRadiansToDegrees(qAtan(-muestra->getAcX()/qSqrt(qPow(muestra->getAcY(),2) + qPow(muestra->getAcZ(),2))));
        }
        if(orientacion.contains("abajo"))
        {
            //Se calculan los angulos con la IMU horizontal.
            angulo1 = qRadiansToDegrees(qAtan(muestra->getAcY()/qSqrt(qPow(muestra->getAcX(),2) + qPow(muestra->getAcZ(),2))));
            angulo2 = qRadiansToDegrees(qAtan(muestra->getAcX()/qSqrt(qPow(muestra->getAcY(),2) + qPow(muestra->getAcZ(),2))));
        }
    }
    this->setTiempo(muestra->getTiempo());
    this->setAngulo1(angulo1);
    this->setAngulo2(angulo2);
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
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAngulo1()-muestra->getGyZ()*dt) + alpha*anguloSinFiltro->getAngulo1();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAngulo2()-muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAngulo2();;
        }
        if(orientacion.contains("frente"))
        {
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAngulo1()+muestra->getGyZ()*dt) + alpha*anguloSinFiltro->getAngulo1();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAngulo2()+muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAngulo2();

        }
        if(orientacion.contains("izquierda"))
        {
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAngulo1()-muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAngulo1();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAngulo2()+muestra->getGyZ()*dt) + alpha*anguloSinFiltro->getAngulo2();
        }
        if(orientacion.contains("derecha"))
        {
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAngulo1()+muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAngulo1();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAngulo2()-muestra->getGyZ()*dt) + alpha*anguloSinFiltro->getAngulo2();
        }
    }
    if(orientacion.contains("horizontal"))
    {
        if(orientacion.contains("arriba"))
        {
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAngulo1()+muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAngulo1();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAngulo2()+muestra->getGyY()*dt) + alpha*anguloSinFiltro->getAngulo2();

        }
        if(orientacion.contains("abajo")){
            anguloComplementario1 = (1-alpha) *(anguloAnterior->getAngulo1()-muestra->getGyX()*dt) + alpha*anguloSinFiltro->getAngulo1();
            anguloComplementario2 = (1-alpha) *(anguloAnterior->getAngulo2()+muestra->getGyY()*dt) + alpha*anguloSinFiltro->getAngulo2();
        }
    }
    this->setTiempo(muestra->getTiempo());
    this->setAngulo1(anguloComplementario1);
    this->setAngulo2(anguloComplementario2);
}

void Angulo::setAnguloInicialKalman(const double angulo1,const double angulo2){
    kalmanX.setAngle(angulo1);
    kalmanY.setAngle(angulo2);
}

void Angulo::calcularAnguloFiltroKalman(const QString orientacion, Muestra *muestra, Angulo *anguloAnterior)
{
    double anguloKalman1=0,anguloKalman2=0;
    Angulo *anguloSinFiltro = new Angulo();
    anguloSinFiltro->calcularAngulo(orientacion,muestra);
    const double dt=(muestra->getTiempo() - anguloAnterior->getTiempo());
    if(orientacion.contains("vertical"))
    {
        if(orientacion.contains("atras"))
        {
            anguloKalman1 = kalmanX.getAngle(anguloSinFiltro->getAngulo1(), -muestra->getGyZ(), dt);
            anguloKalman2 = kalmanY.getAngle(anguloSinFiltro->getAngulo2(), -muestra->getGyX(), dt);
        }
        if(orientacion.contains("frente"))
        {
            anguloKalman1 = kalmanX.getAngle(anguloSinFiltro->getAngulo1(), +muestra->getGyZ(), dt);
            anguloKalman2 = kalmanY.getAngle(anguloSinFiltro->getAngulo2(), +muestra->getGyX(), dt);
        }
        if(orientacion.contains("izquierda"))
        {
            anguloKalman1 = kalmanX.getAngle(anguloSinFiltro->getAngulo1(), -muestra->getGyX(), dt);
            anguloKalman2 = kalmanY.getAngle(anguloSinFiltro->getAngulo2(), +muestra->getGyZ(), dt);
        }
        if(orientacion.contains("derecha"))
        {
            anguloKalman1 = kalmanX.getAngle(anguloSinFiltro->getAngulo1(), +muestra->getGyX(), dt);
            anguloKalman2 = kalmanY.getAngle(anguloSinFiltro->getAngulo2(), -muestra->getGyZ(), dt);
        }
    }
    if(orientacion.contains("horizontal"))
    {
        if(orientacion.contains("arriba"))
        {
            anguloKalman1 = kalmanX.getAngle(anguloSinFiltro->getAngulo1(), muestra->getGyX(), dt);
            anguloKalman2 = kalmanY.getAngle(anguloSinFiltro->getAngulo2(), muestra->getGyY(), dt);
        }
        if(orientacion.contains("abajo")){
            anguloKalman1 = kalmanX.getAngle(anguloSinFiltro->getAngulo1(), -muestra->getGyX(), dt);
            anguloKalman2 = kalmanY.getAngle(anguloSinFiltro->getAngulo2(), muestra->getGyY(), dt);
        }

    }
    this->setTiempo(muestra->getTiempo());
    this->setAngulo1(anguloKalman1);
    this->setAngulo2(anguloKalman2);

}
