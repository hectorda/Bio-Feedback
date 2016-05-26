#include "prueba.h"

Prueba::Prueba(QObject *parent) : QObject(parent)
{

}

void Prueba::setNumeroPrueba(const int numero)
{
    this->numeroPrueba=numero;
}

void Prueba::setCantidadObjetivos(const int numeroObjetivos)
{
    this->cantidadObjetivos=numeroObjetivos;
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

void Prueba::setObjetivosEnOrden(const bool enorden)
{
    this->objetivosEnOrden=enorden;
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

void Prueba::setListaObjetivos(const QVector<QCPItemEllipse *> listaObjetivos)
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

int Prueba::getNumeroPrueba()
{
    return this->numeroPrueba;
}

int Prueba::getCantidadObjetivos()
{
    return this->cantidadObjetivos;
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

bool Prueba::getObjetivosEnOrden()
{
    return this->objetivosEnOrden;
}

double Prueba::getTiempoPrueba()
{
    return this->tiempoPrueba;
}

int Prueba::getDivisorFPS()
{
    return this->divisorFPS;
}

QVector<QCPItemEllipse*> Prueba::getListaObjetivos()
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

void Prueba::limpiarListas()
{
    this->listaMuestras.clear();
    this->listaAngulos.clear();
    this->listaDesplazamientos.clear();
    this->listaObjetivos.clear();
}

void Prueba::exportar()
{
    if(listaAngulos.isEmpty() || listaDesplazamientos.isEmpty())
        QMessageBox::critical(0,"Aun no se realiza prueba","Se debe realizar una prueba antes de Exportar.");
    else{
        QString selectedFilter;
        QString filters(tr("Formato BioFeed-Back (*.bioh)"));
        QString fileName = QFileDialog::getSaveFileName(0, tr("Guardar el Archivo"),"",filters,&selectedFilter);

        if (fileName != ""){
            QFile file(fileName);
            file.remove();
            if (file.open(QIODevice::Append)){
                QTextStream stream(&file);
                stream <<"PruebaNumero: "<<this->getNumeroPrueba()<<endl;
                stream <<"OrientacionSensor: "<<this->getOrientacion()<<endl;
                stream <<"AlturaPuestaSensor: "<<this->getAlturaDispositivo()<<endl;
                stream <<"TiempoMediciones: "<<this->getTiempoTotal()<<endl;
                stream <<"MuestrasObtenidas: "<<this->getCantidadMuestras()<<endl;
                for (int var = 0; var < this->getCantidadMuestras(); ++var){
                    Angulo *ang=listaAngulos.at(var);
                    Desplazamiento *desp=listaDesplazamientos.at(var);
                    Muestra *raw=listaMuestras.at(var);
                    stream << ang->getTiempo()<<" "<<ang->getAnguloX()<<" "<<ang->getAnguloY()<<" "<<desp->getDesplazamientoX()<<" "<<desp->getDesplazamientoY()<<" "<<
                              raw->getAcX()<<" "<<raw->getAcY()<<" "<<raw->getAcZ()<<" "<<raw->getGyX()<<" "<<raw->getGyY()<<" "<<raw->getGyZ()<<endl;
                }
                file.flush();
                file.close();
            }
            else {
                QMessageBox::critical(0, tr("Error"), tr("No se pudo guardar el archivo"));
                return;
            }
        }
    }
}


