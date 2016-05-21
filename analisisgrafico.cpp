#include "analisisgrafico.h"
#include "ui_analisisgrafico.h"
#include <QTextStream>

AnalisisGrafico::AnalisisGrafico(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalisisGrafico)
{
    ui->setupUi(this);
}

AnalisisGrafico::AnalisisGrafico(QWidget *parent,Reportes *reportes):
    QDialog(parent),
    ui(new Ui::AnalisisGrafico)
{
    ui->setupUi(this);
    this->reportes=reportes;
    ui->horizontalSlider->setHandleMovementMode(QxtSpanSlider::NoOverlapping);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void AnalisisGrafico::setListaAngulos(QList<Angulo *> LA)
{
    this->listaAngulos=LA;

    if(this->listaAngulos.isEmpty())
        QTextStream stdout<<"No hay datos de Angulos a analizar"<<endl;

    else
    {
        this->setWindowTitle("Analisis Lista Angulos");
        const int muestras=listaAngulos.size()-1;
        ui->horizontalSlider->setMaximum(muestras);

        connect(ui->horizontalSlider,QxtSpanSlider::lowerValueChanged, [=](const int &newValue){
            const double tiempo=listaAngulos.at(newValue)->getTiempo();
            reportes->moverLineaIzquierdaAngulos(tiempo);
            calcularEstadisticosAngulos(newValue,ui->horizontalSlider->upperValue());
            contarDatosAngulos(newValue,ui->horizontalSlider->upperValue());
            ui->labelRangoInf->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->horizontalSlider,QxtSpanSlider::upperValueChanged, [=](const int &newValue){
            const double tiempo=listaAngulos.at(newValue)->getTiempo();
            reportes->moverLineaDerechaAngulos(tiempo);
            calcularEstadisticosAngulos(ui->horizontalSlider->lowerValue(),newValue);
            contarDatosAngulos(ui->horizontalSlider->lowerValue(),newValue);
            ui->labelRangoSup->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->pushButtonRestaurar,QPushButton::clicked,[=](){
            reportes->moverLineaIzquierdaAngulos(0);
            reportes->moverLineaDerechaAngulos(listaAngulos.last()->getTiempo());
            ui->horizontalSlider->setLowerValue(0);
            ui->horizontalSlider->setUpperPosition(listaAngulos.size()-1);
            ajustarRangosGraficoAngulos(0,listaAngulos.size()-1);
        });

        connect(ui->pushButtonAplicarRango,QPushButton::clicked,[=](){
            const int inicio=ui->horizontalSlider->lowerValue();
            const int fin=ui->horizontalSlider->upperValue();
            ajustarRangosGraficoAngulos(inicio,fin);
        });

        connect(ui->pushButtonRescalar,QPushButton::clicked,[=](){
            reportes->replotGraficoAngulos();
        });

        ui->horizontalSlider->setLowerValue(0);
        ui->horizontalSlider->setLowerPosition(0);
        ui->horizontalSlider->lowerValueChanged(0); //para que se añada automaticamente el slider izquierdo
        ui->horizontalSlider->setUpperValue(muestras);
        ui->horizontalSlider->setUpperPosition(muestras);
    }
}


void AnalisisGrafico::ajustarRangosGraficoAngulos(const int inicio, const int fin)
{
    reportes->vaciarGraficoAngulos();
    for (int var = inicio; var <= fin; ++var)
        reportes->agregarDatosGraficoAngulos(listaAngulos.at(var));

    reportes->replotGraficoAngulos();
}

void AnalisisGrafico::contarDatosAngulos(const int inicio, const int fin){
    const int terminos=fin+1-inicio;
    double frecuenciaMuestreo=(listaAngulos.at(fin)->getTiempo()-listaAngulos.at(inicio)->getTiempo());
    frecuenciaMuestreo/=(fin-inicio);
    ui->labelCantidadDatos->setText(QString("Datos totales: %1, Datos analizados actualmente: %2"
                                            "\nFrecuencia de muestreo obtenida %3 Hz")
                                            .arg(listaAngulos.size()).arg(terminos).arg(1/frecuenciaMuestreo));

}

void AnalisisGrafico::calcularEstadisticosAngulos(const int inicio, const int fin)
{
    double media1=0,media2=0,varian1=0,varian2=0,desvEst1=0,desvEst2=0;
    int terminos=fin+1-inicio;
    for (int var = inicio; var <= fin; ++var) {
        media1+=listaAngulos.at(var)->getAnguloX();
        media2+=listaAngulos.at(var)->getAnguloY();
    }
    media1/=terminos;
    media2/=terminos;

    for (int var = inicio; var <= fin; ++var) {
        varian1+=qPow((listaAngulos.at(var)->getAnguloX()-media1),2);
        varian2+=qPow((listaAngulos.at(var)->getAnguloY()-media2),2);
    }
    varian1/=terminos;
    varian2/=terminos;
    desvEst1=qSqrt(varian1);
    desvEst2=qSqrt(varian2);

    ui->labelMedia->setText(tr("Media Angulo X: %1 , Media Angulo Y: %2").arg(media1).arg(media2));
    ui->labelDesvEst->setText(QString("Desviacion Estandar Angulo X: %1 , Desviación Estándar Angulo Y: %2").arg(desvEst1).arg(desvEst2));
}

AnalisisGrafico::~AnalisisGrafico()
{
    delete ui;
}



void AnalisisGrafico::setListaMuestras(QList<Raw *> LR)
{
    this->listaMuestras=LR;

    if(this->listaMuestras.isEmpty())
        QTextStream stdout<<"No hay datos de Muestras a analizar"<<endl;

    else
    {
        this->setWindowTitle("Analisis Lista Muestras");
        const int muestras=listaMuestras.size()-1;
        ui->horizontalSlider->setMaximum(muestras);

        connect(ui->horizontalSlider,QxtSpanSlider::lowerValueChanged, [=](const int &newValue){
            const double tiempo=listaMuestras.at(newValue)->getTiempo();
            //reportes->moverLineaIzquierdaAngulos(tiempo);
            calcularEstadisticosMuestras(newValue,ui->horizontalSlider->upperValue());
            contarDatosMuestras(newValue,ui->horizontalSlider->upperValue());
            ui->labelRangoInf->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->horizontalSlider,QxtSpanSlider::upperValueChanged, [=](const int &newValue){
            const double tiempo=listaMuestras.at(newValue)->getTiempo();
            //reportes->moverLineaDerechaAngulos(tiempo);
            calcularEstadisticosMuestras(ui->horizontalSlider->lowerValue(),newValue);
            contarDatosMuestras(ui->horizontalSlider->lowerValue(),newValue);
            ui->labelRangoSup->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->pushButtonRestaurar,QPushButton::clicked,[=](){
//            reportes->moverLineaIzquierdaAngulos(0);
//            reportes->moverLineaDerechaAngulos(listaAngulos.last()->getTiempo());
            ui->horizontalSlider->setLowerValue(0);
            ui->horizontalSlider->setUpperPosition(listaMuestras.size()-1);
            ajustarRangosGraficoMuestras(0,listaMuestras.size()-1);

        });

        connect(ui->pushButtonAplicarRango,QPushButton::clicked,[=](){
            const int inicio=ui->horizontalSlider->lowerValue();
            const int fin=ui->horizontalSlider->upperValue();
            ajustarRangosGraficoMuestras(inicio,fin);
        });

        connect(ui->pushButtonRescalar,QPushButton::clicked,[=](){
            reportes->replotGraficoMuestras();
        });

        ui->horizontalSlider->setLowerValue(0);
        ui->horizontalSlider->setLowerPosition(0);
        ui->horizontalSlider->lowerValueChanged(0); //para que se añada automaticamente el slider izquierdo
        ui->horizontalSlider->setUpperValue(muestras);
        ui->horizontalSlider->setUpperPosition(muestras);
    }
}


void AnalisisGrafico::ajustarRangosGraficoMuestras(const int inicio, const int fin)
{
    reportes->vaciarGraficoMuestras();
    for (int var = inicio; var <= fin; ++var)
        reportes->agregarDatosGraficoMuestras(listaMuestras.at(var));

    reportes->replotGraficoMuestras();
}

void AnalisisGrafico::contarDatosMuestras(const int inicio, const int fin){
    const int terminos=fin+1-inicio;
    double frecuenciaMuestreo=(listaMuestras.at(fin)->getTiempo()-listaMuestras.at(inicio)->getTiempo());
    frecuenciaMuestreo/=(fin-inicio);
    ui->labelCantidadDatos->setText(QString("Datos totales: %1, Datos analizados actualmente: %2"
                                            "\nFrecuencia de muestreo obtenida %3 Hz")
                                            .arg(listaMuestras.size()).arg(terminos).arg(1/frecuenciaMuestreo));

}

void AnalisisGrafico::calcularEstadisticosMuestras(const int inicio, const int fin)
{
    double mediaAcX=0,mediaAcY=0,mediaAcZ=0,mediaGyX=0,mediaGyY=0,mediaGyZ=0;
    double varianAcX=0,varianAcY=0,varianAcZ=0,varianGyX=0,varianGyY=0,varianGyZ=0;
    double desvEstAcX=0,desvEstAcY=0,desvEstAcZ=0,desvEstGyX=0,desvEstGyY=0,desvEstGyZ=0;
    int terminos=fin+1-inicio;
    for (int var = inicio; var <= fin; ++var) {
        mediaAcX+=listaMuestras.at(var)->getAcX();
        mediaAcY+=listaMuestras.at(var)->getAcY();
        mediaAcZ+=listaMuestras.at(var)->getAcZ();
        mediaGyX+=listaMuestras.at(var)->getGyX();
        mediaGyY+=listaMuestras.at(var)->getGyY();
        mediaGyZ+=listaMuestras.at(var)->getGyZ();
    }
    mediaAcX/=terminos;
    mediaAcY/=terminos;
    mediaAcZ/=terminos;
    mediaGyX/=terminos;
    mediaGyY/=terminos;
    mediaGyZ/=terminos;

    for (int var = inicio; var <= fin; ++var) {
        varianAcX+=qPow((listaMuestras.at(var)->getAcX()-mediaAcX),2);
        varianAcY+=qPow((listaMuestras.at(var)->getAcY()-mediaAcY),2);
        varianAcZ+=qPow((listaMuestras.at(var)->getAcZ()-mediaAcZ),2);
        varianGyX+=qPow((listaMuestras.at(var)->getGyX()-mediaGyX),2);
        varianGyY+=qPow((listaMuestras.at(var)->getGyY()-mediaGyY),2);
        varianGyZ+=qPow((listaMuestras.at(var)->getGyZ()-mediaGyZ),2);
    }
    varianAcX/=terminos;
    varianAcY/=terminos;
    varianAcZ/=terminos;
    varianGyX/=terminos;
    varianGyY/=terminos;
    varianGyZ/=terminos;

    desvEstAcX=qSqrt(varianAcX);
    desvEstAcY=qSqrt(varianAcY);
    desvEstAcZ=qSqrt(varianAcZ);
    desvEstGyX=qSqrt(varianGyX);
    desvEstGyY=qSqrt(varianGyY);
    desvEstGyZ=qSqrt(varianGyZ);


    ui->labelMedia->setText(tr("Media AcX: %1 , Media AcY: %2 , Media AcZ: %3 \nMedia GyX: %4 , Media GyY: %5 , Media GyZ: %6")
                            .arg(mediaAcX).arg(mediaAcY).arg(mediaAcZ).arg(mediaGyX).arg(mediaGyY).arg(mediaGyZ));
    ui->labelDesvEst->setText(QString("Desviacion Estandar AcX: %1 , Desviación Estándar AcY: %2 , Desviación Estándar AcZ: %3 \nFaltan las del giro :p")
                              .arg(desvEstAcX).arg(desvEstAcY).arg(desvEstAcX));
}
