#include "analisisgraficos.h"
#include "ui_analisisgraficos.h"

AnalisisGraficos::AnalisisGraficos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalisisGraficos)
{
    ui->setupUi(this);
    ui->horizontalSlider->setHandleMovementMode(QxtSpanSlider::NoOverlapping);
    ui->horizontalSlider->setLowerPosition(0);
    ui->horizontalSlider->setLowerValue(0);
    ui->horizontalSlider->setUpperValue(100);
    ui->horizontalSlider->setUpperValue(100);
}

AnalisisGraficos::~AnalisisGraficos()
{
    delete ui;
}
