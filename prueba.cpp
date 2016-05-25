#include "prueba.h"

Prueba::Prueba(QObject *parent) : QObject(parent)
{

}

void Prueba::setNumeroPrueba(const int numero)
{
    this->numeroPrueba=numero;
}

void Prueba::setNumeroObjetivos(const int numeroObjetivos)
{
    this->numeroObjetivos=numeroObjetivos;
}

void Prueba::setAleatorios(const bool aleatorio)
{
    this->aleatorios=aleatorio;
}

void Prueba::setDetenerAlMarcarTodos(const bool detener)
{
    this->detenerAlMarcarTodos=detener;
}

void Prueba::setLimitarGrafico(const bool limitar)
{
    this->limitarGrafico=limitar;
}

void Prueba::setTiempoPrueba(const double tiempo)
{
    this->tiempoPrueba=tiempo;
}

void Prueba::setDivisorFPS()
{
    if(this->frecuenciaMuestreo > this->ajustesGrafico.FPS)//Si la frecuencia de muestreo es menor a 275 se calculan el divisor en base a la frecuencia elegida sino en 275.
        this->divisorFPS= this->frecuenciaMuestreo<275 ? (int)(this->frecuenciaMuestreo/this->ajustesGrafico.FPS):
                                                         (int)(275/this->ajustesGrafico.FPS);
    else//Si la frecuencia de muestreo  es menor a los fps se grafica en base a la frecuencia
        this->divisorFPS=1;
}

void Prueba::setDivisorFPS(const int divisor)
{
    this->divisorFPS=divisor;
}

void Prueba::setListaObjetivos(const QList<QCPItemEllipse *> listaObjetivos)
{
    this->listaObjetivos=listaObjetivos;
}

void Prueba::setAlturaDispositivo(const double altura)
{
    this->alturaDispositivo=altura;
}

void Prueba::setOrientacion(const QString orientacion)
{
    this->orientacion=orientacion;
}

void Prueba::setFrecuenciaMuestreo(const double frecuencia)
{
    this->frecuenciaMuestreo=frecuencia;
}

void Prueba::setUnidad(const QString unidad)
{
    this->unidad=unidad;
}

void Prueba::setCadenaConfiguracion(const QString cadena)
{
    this->cadenaConfiguracion=cadena;
}

void Prueba::setCantidadMuestras(const int cantidad)
{
    this->cantidadMuestras=cantidad;
}

void Prueba::setTiempoTotal(const double tiempo)
{
    this->tiempoTotal=tiempo;
}

void Prueba::setAjustesGrafico(const AjustesGrafico::Ajustes ajustes)
{
    this->ajustesGrafico=ajustes;
}

void Prueba::setAjustesPuertoSerial(const AjustesPuertoSerial::Ajustes ajustes)
{
    this->ajustesPuertoSerial=ajustes;
}

void Prueba::setAjustesSensores(const AjustesSensores::Ajustes ajustes)
{
    this->ajustesSensores=ajustes;
}

int Prueba::getNumeroPrueba()
{
    return this->numeroPrueba;
}

int Prueba::getNumeroObjetivos()
{
    return this->numeroObjetivos;
}

bool Prueba::getAleatorios()
{
    return this->aleatorios;
}

bool Prueba::getDetenerAlMarcarTodos()
{
    return this->detenerAlMarcarTodos;
}

bool Prueba::getLimitarGrafico()
{
    return this->limitarGrafico;
}

double Prueba::getTiempoPrueba()
{
    return this->tiempoPrueba;
}

int Prueba::getDivisorFPS()
{
    return this->divisorFPS;
}

QList<QCPItemEllipse*> Prueba::getListaObjetivos()
{
    return this->listaObjetivos;
}

double Prueba::getAlturaDispositivo()
{
    return this->alturaDispositivo;
}

QString Prueba::getOrientacion()
{
    return this->orientacion;
}

double Prueba::getFrecuenciaMuestreo()
{
    return this->frecuenciaMuestreo;
}

QString Prueba::getUnidad()
{
    return this->unidad;
}

QString Prueba::getCadenaConfiguracion()
{
    return this->cadenaConfiguracion;
}

int Prueba::getCantidadMuestras()
{
    return this->cantidadMuestras;
}

double Prueba::getTiempoTotal()
{
    return this->tiempoTotal;
}

AjustesGrafico::Ajustes Prueba::getAjustesGrafico()
{
    return this->ajustesGrafico;
}

AjustesPuertoSerial::Ajustes Prueba::getAjustesPuertoSerial()
{
    return this->ajustesPuertoSerial;
}

AjustesSensores::Ajustes Prueba::getAjustesSensores()
{
    return this->ajustesSensores;
}



