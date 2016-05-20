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

    ui->horizontalSlider->setHandleMovementMode(QxtSpanSlider::NoOverlapping);

    connect(ui->horizontalSlider,QxtSpanSlider::lowerValueChanged, [=](const int &newValue){
        reportes->moverLineaIzquierdaAngulos(newValue);
    });

    connect(ui->horizontalSlider,QxtSpanSlider::upperValueChanged, [=](const int &newValue){
        reportes->moverLineaDerechaAngulos(newValue);
    });

    connect(ui->pushButtonAplicarRango,QPushButton::clicked,[=](){
        reportes->vaciarGraficoAngulos();
        foreach (Angulo *var, listaAngulos)
            if(var->getTiempo()>=ui->horizontalSlider->lowerValue()/100.0 && var->getTiempo()<=ui->horizontalSlider->upperValue()/100.0)
                reportes->agregarDatosGraficoAngulos(var);

        reportes->replotGraficoAngulos();
    });

    connect(ui->pushButtonRestaurar,QPushButton::clicked,[=](){
        reportes->vaciarGraficoAngulos();
        reportes->moverLineaIzquierdaAngulos(0);
        reportes->moverLineaDerechaAngulos(listaAngulos.last()->getTiempo()*100.0);
        foreach (Angulo *var, listaAngulos)
                reportes->agregarDatosGraficoAngulos(var);

        reportes->replotGraficoAngulos();
    });

}

void AnalisisGrafico::setListaAngulos(QList<Angulo *> listaAngulos)
{
    this->listaAngulos=listaAngulos;
    const double tiempoPrueba=listaAngulos.last()->getTiempo();
    ui->horizontalSlider->setMaximum(tiempoPrueba*100.0);
    ui->horizontalSlider->setLowerValue(0);
    ui->horizontalSlider->setLowerPosition(0);
    ui->horizontalSlider->setUpperValue(tiempoPrueba*100.0);
    ui->horizontalSlider->setUpperPosition(tiempoPrueba*100.0);
}

AnalisisGrafico::~AnalisisGrafico()
{
    delete ui;
}
