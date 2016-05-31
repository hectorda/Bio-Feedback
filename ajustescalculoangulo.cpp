#include "ajustescalculoangulo.h"
#include "ui_ajustescalculoangulo.h"

AjustesCalculoAngulo::AjustesCalculoAngulo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjustesCalculoAngulo)
{
    ui->setupUi(this);
}

AjustesCalculoAngulo::~AjustesCalculoAngulo()
{
    delete ui;
}
