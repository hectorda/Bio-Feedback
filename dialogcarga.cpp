#include "dialogcarga.h"
#include "ui_dialogcarga.h"

DialogCarga::DialogCarga(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCarga)
{
    ui->setupUi(this);
     setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    movie = new QMovie(":/images/Loading.gif");
    //QString texto=QString("Actualizando configuracion de sensores\nPuerto: %1\nFrecuencia Muestreo: %2 Hz").arg(ajustesSerial->getAjustes().portName).arg(prueba->getFrecuenciaMuestreo());
    ui->labelMovie->setMovie(movie);
    movie->setScaledSize(QSize(50,50));
    timer=new QTimer();
    porcentaje=0;
    connect(timer,SIGNAL(timeout()),this,SLOT(actualizarPorcentaje()));
}

DialogCarga::~DialogCarga()
{
    delete ui;
}

void DialogCarga::setTextoCarga(QString &texto)
{
    ui->labelCarga->setText(texto);
}

void DialogCarga::iniciar(double tiempo)
{
    porcentaje=0;
    timer->start(tiempo/100.0);
    movie->start();
}

void DialogCarga::actualizarPorcentaje()
{
    porcentaje+=1;
    ui->progressBar->setValue(porcentaje);
}

void DialogCarga::pararMovie()
{
    movie->stop();
    timer->stop();
}
