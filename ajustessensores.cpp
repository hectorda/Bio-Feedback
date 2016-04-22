#include "ajustessensores.h"
#include "ui_ajustessensores.h"

AjustesSensores::AjustesSensores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjustesSensores)
{
    inicializar();
    conexiones();
    llenarParametros();
}

AjustesSensores::~AjustesSensores()
{
    delete ui;
}


void AjustesSensores::inicializar(){

    ui->setupUi(this);

}

void AjustesSensores::conexiones()
{
    connect(ui->pushButtonOK, SIGNAL(clicked()),this, SLOT(aplicar()));
    connect(ui->pushButtonExit,SIGNAL(clicked()),this,SLOT(close()));
}

void AjustesSensores::llenarParametros()
{
    ui->comboBoxAscale->addItem(QStringLiteral("2G"), 0);
    ui->comboBoxAscale->addItem(QStringLiteral("4G"), 1);
    ui->comboBoxAscale->addItem(QStringLiteral("8G"), 2);
    ui->comboBoxAscale->addItem(QStringLiteral("16G"), 3);
    ui->comboBoxAscale->setCurrentIndex(0);

    ui->comboBoxGscale->addItem(QStringLiteral("250 dps"), 0);
    ui->comboBoxGscale->addItem(QStringLiteral("500 dps"), 1);
    ui->comboBoxGscale->addItem(QStringLiteral("1000 dps"), 2);
    ui->comboBoxGscale->addItem(QStringLiteral("2000 dps"), 3);
    ui->comboBoxGscale->setCurrentIndex(0);

    double SampleRate=8000.0/(ui->spinBoxSampleRate->value()+1);
    ui->labelSampleRate->setText(tr("Frecuencia de Muestreo= %1").arg(QString::number(SampleRate,'f',5)));
}

QString AjustesSensores::getAjustes() const
{
    return ajustesactuales;
}

void AjustesSensores::aplicar()
{
    ajustesactuales.clear();
    //if(ui->comboBoxAscale->currentIndex()!=0)//Si no es el valor por defecto
        ajustesactuales+="a"+QString::number(ui->comboBoxAscale->currentIndex());

    //if(ui->comboBoxGscale->currentIndex()!=0)
        ajustesactuales+="g"+QString::number(ui->comboBoxGscale->currentIndex());

    //if(ui->spinBoxDLPF->value()!=0)
        ajustesactuales.append("l"+QString::number(ui->spinBoxDLPF->value()));

    //if(ui->spinBoxSampleRate->value()!=79)
        ajustesactuales+="s"+QString::number(ui->spinBoxSampleRate->value());

     hide();

}

void AjustesSensores::on_spinBoxSampleRate_valueChanged(int arg1)
{
    if(ui->spinBoxDLPF->value()==0){
        double SampleRate=8000.0/(arg1+1);
        ui->labelSampleRate->setText(tr("Frecuencia de Muestreo= %1").arg(QString::number(SampleRate,'f',5)));
    }
    else{
        double SampleRate=1000.0/(arg1+1);
        ui->labelSampleRate->setText(tr("Frecuencia de Muestreo= %1").arg(QString::number(SampleRate,'f',5)));
    }
}

void AjustesSensores::on_spinBoxDLPF_valueChanged(int arg1)
{
    if(arg1==0){
        double SampleRate=8000.0/(ui->spinBoxSampleRate->value()+1);
        ui->labelSampleRate->setText(tr("Frecuencia de Muestreo= %1").arg(QString::number(SampleRate,'f',5)));
    }
    else{
        double SampleRate=1000.0/(ui->spinBoxSampleRate->value()+1);
        ui->labelSampleRate->setText(tr("Frecuencia de Muestreo= %1").arg(QString::number(SampleRate,'f',5)));
    }
}
