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
}

DialogCarga::~DialogCarga()
{
    delete ui;
}

void DialogCarga::setTextoCarga(QString &texto)
{
    ui->labelCarga->setText(texto);
}

void DialogCarga::iniciarMovie()
{
    movie->start();
}

void DialogCarga::pararMovie()
{
    movie->stop();
}
