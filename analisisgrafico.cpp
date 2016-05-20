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
}

void AnalisisGrafico::setListaAngulos(QList<Angulo *> LA)
{
    this->listaAngulos=LA;

    if(this->listaAngulos.isEmpty())
        QTextStream stdout<<"No hay datos de Angulos a analizar"<<endl;

    else
    {
        const int muestras=listaAngulos.size()-1;
        ui->horizontalSlider->setMaximum(muestras);

        connect(ui->horizontalSlider,QxtSpanSlider::lowerValueChanged, [=](const int &newValue){
            const double tiempo=listaAngulos.at(newValue)->getTiempo();
            reportes->moverLineaIzquierdaAngulos(tiempo);
            calcularEstadisticos(newValue,ui->horizontalSlider->upperValue());
            contarDatos(newValue,ui->horizontalSlider->upperValue());
            ui->labelRangoInf->setText(QString::number(tiempo,'f',3));
        });

        connect(ui->horizontalSlider,QxtSpanSlider::upperValueChanged, [=](const int &newValue){
            const double tiempo=listaAngulos.at(newValue)->getTiempo();
            reportes->moverLineaDerechaAngulos(tiempo);
            calcularEstadisticos(ui->horizontalSlider->lowerValue(),newValue);
            contarDatos(ui->horizontalSlider->lowerValue(),newValue);
            ui->labelRangoSup->setText(QString::number(tiempo,'f',3));
        });

        connect(ui->pushButtonRestaurar,QPushButton::clicked,[=](){
            reportes->vaciarGraficoAngulos();
            reportes->moverLineaIzquierdaAngulos(0);
            reportes->moverLineaDerechaAngulos(listaAngulos.size());
            ui->horizontalSlider->setLowerValue(0);
            ui->horizontalSlider->setUpperPosition(listaAngulos.size()-1);

            foreach (Angulo *var, listaAngulos)
                reportes->agregarDatosGraficoAngulos(var);

            reportes->replotGraficoAngulos();
        });

        connect(ui->pushButtonAplicarRango,QPushButton::clicked,[=](){
            const int inicio=ui->horizontalSlider->lowerValue();
            const int fin=ui->horizontalSlider->upperValue();
            ajustarRangosGrafico(inicio,fin);
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


void AnalisisGrafico::ajustarRangosGrafico(const int inicio, const int fin)
{
    reportes->vaciarGraficoAngulos();
    for (int var = inicio; var <= fin; ++var)
        reportes->agregarDatosGraficoAngulos(listaAngulos.at(var));

    reportes->replotGraficoAngulos();
}

void AnalisisGrafico::contarDatos(const int inicio, const int fin){
    const int terminos=fin+1-inicio;
    double frecuenciaMuestreo=(listaAngulos.at(fin)->getTiempo()-listaAngulos.at(inicio)->getTiempo());
    frecuenciaMuestreo/=(fin-inicio);
    ui->labelCantidadDatos->setText(QString("Datos totales: %1, Datos analizados actualmente: %2"
                                            "\nFrecuencia de muestreo obtenida %3 Hz")
                                            .arg(listaAngulos.size()).arg(terminos).arg(1/frecuenciaMuestreo));

}

void AnalisisGrafico::calcularEstadisticos(const int inicio, const int fin)
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
