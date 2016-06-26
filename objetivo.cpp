#include "objetivo.h"

Objetivo::Objetivo()
{

}

Objetivo::Objetivo(QCPItemEllipse *cir, double centerx, double centery, int radioO, QColor sinMarcar)
{
    this->setCirculo(cir);
    this->setRadioObjetivo(radioO);
    this->setColorSinMarcar(sinMarcar);

    timerParpadeo=new QTimer;
    this->Circulo->topLeft->setCoords(centerx-this->radioObjetivo,centery+this->radioObjetivo);
    this->Circulo->bottomRight->setCoords(centerx+this->radioObjetivo,centery-this->radioObjetivo);
    this->Circulo->setBrush(QBrush(this->colorSinMarcar));
    connect(timerParpadeo,QTimer::timeout,[=](){ this->parpadeoCirculo();});
}

Objetivo::~Objetivo()
{
    delete timerParpadeo;
    delete Circulo;
}

QCPItemEllipse *Objetivo::getCirculo() const
{
    return Circulo;
}

void Objetivo::setCirculo(QCPItemEllipse *value)
{
    Circulo = value;
}

int Objetivo::getRadioObjetivo() const
{
    return radioObjetivo;
}

void Objetivo::setRadioObjetivo(int value)
{
    radioObjetivo = value;
}

QColor Objetivo::getColorSinMarcar() const
{
    return colorSinMarcar;
}

void Objetivo::setColorSinMarcar(const QColor &value)
{
    colorSinMarcar = value;
}

/*
* Se verifica si el punto dado pertenece a un circulo usando geometria
* En caso pertenecer cambia el color de esta y ademas retorna True
* En caso contrario retorna False.
*/
bool Objetivo::PertenecePuntoAlObjetivo(const double x, const double y){
    const double perteneceCirc=qSqrt(qPow(( x - ( this->Circulo->topLeft->coords().x()+this->radioObjetivo)),2)+qPow(( y - (this->Circulo->topLeft->coords().y()- this->radioObjetivo)),2));
    if( perteneceCirc < this->radioObjetivo)
        return true;
    return false;
}


void Objetivo::iniciarParpadeo()
{
    if(!timerParpadeo->isActive()){
        timerParpadeo->start(1000);
        this->Circulo->setBrush(QBrush(Qt::white));
    }
}

/*
* Dado un QCPItemEllipse se calcula el tiempo y se hace parpadear
* repintando la elipse entre dos colores
* según el módulo del tiempo transcurrido
*/
void Objetivo::parpadeoCirculo()
{
    if(this->Circulo->brush()==QBrush(this->colorSinMarcar))
        this->Circulo->setBrush(QBrush(Qt::white));
    else
        this->Circulo->setBrush(QBrush(this->colorSinMarcar));
}

void Objetivo::marcarObjetivo(QColor color)
{
    timerParpadeo->stop();
    this->Circulo->setBrush(QBrush(color));
}

void Objetivo::pararTimer()
{
    timerParpadeo->stop();

}

