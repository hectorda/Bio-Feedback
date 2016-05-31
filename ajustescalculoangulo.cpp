#include "ajustescalculoangulo.h"
#include "ui_ajustescalculoangulo.h"

AjustesCalculoAngulo::AjustesCalculoAngulo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjustesCalculoAngulo)
{
    inicializar();
    conexiones();
    aplicar();
}

AjustesCalculoAngulo::~AjustesCalculoAngulo()
{
    delete ui;
}

void AjustesCalculoAngulo::inicializar()
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->comboBoxFiltroAngulo->addItem(QString("Sin Filtro"),0);
    ui->comboBoxFiltroAngulo->addItem(QString("Filtro Kalman"),1);
    ui->comboBoxFiltroAngulo->addItem(QString("Filtro Complementario"),2);
    ui->comboBoxFiltroAngulo->setCurrentIndex(1);
}

void AjustesCalculoAngulo::conexiones()
{
    connect(ui->pushButtonCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pushButtonOK,SIGNAL(clicked(bool)),this,SLOT(aplicar()));
}

void AjustesCalculoAngulo::on_comboBoxFiltroAngulo_currentTextChanged(const QString &arg1)
{
    ui->labelFiltroUsado->setText(arg1);
    if(arg1.toLower().contains("sin filtro")){
        ui->doubleSpinBoxAlphaFC->hide();
    }
    if(arg1.toLower().contains("kalman")){
        ui->doubleSpinBoxAlphaFC->hide();
    }
    if(arg1.toLower().contains("complementario")){
        ui->doubleSpinBoxAlphaFC->show();
    }
}

void AjustesCalculoAngulo::aplicar()
{
    filtro=ui->comboBoxFiltroAngulo->currentText();
    alpha=ui->doubleSpinBoxAlphaFC->value();
    close();
}
