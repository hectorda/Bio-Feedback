#include "ajustessensores.h"
#include "ui_ajustessensores.h"

AjustesSensores::AjustesSensores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjustesSensores)
{
    inicializar();
    conexiones();
    llenarParametros();
    aplicar();
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
    ui->comboBoxAscale->addItem(QStringLiteral("±2G"), 0);
    ui->comboBoxAscale->addItem(QStringLiteral("±4G"), 1);
    ui->comboBoxAscale->addItem(QStringLiteral("±8G"), 2);
    ui->comboBoxAscale->addItem(QStringLiteral("±16G"), 3);
    ui->comboBoxAscale->setCurrentIndex(0);

    ui->comboBoxGscale->addItem(QStringLiteral("250 º/s"), 0);
    ui->comboBoxGscale->addItem(QStringLiteral("500 º/s"), 1);
    ui->comboBoxGscale->addItem(QStringLiteral("1000 º/s"), 2);
    ui->comboBoxGscale->addItem(QStringLiteral("2000 º/s"), 3);
    ui->comboBoxGscale->setCurrentIndex(0);

    double SampleRate=8000.0/(ui->spinBoxSampleRate->value()+1);
    mostrarFrecuenciaMuestreo(SampleRate);
    mostrarFiltroPasaBajo("Desactivado");
}

void AjustesSensores::mostrarFrecuenciaMuestreo(double SampleRate)
{
    const QString fSampleRate=tr("Frecuencia de Muestreo: %1 Hz").arg(QString::number(SampleRate,'f',2));
    ui->labelSampleRate->setText(fSampleRate);
    ui->lcdNumberSampleRate->display(QString::number(SampleRate,'f',2));
}

void AjustesSensores::mostrarFiltroPasaBajo(const QString DLPF)
{
    ui->labelDLPF->setText("Filtro Pasa-Bajo: "+DLPF);
}

QString AjustesSensores::getAjustes() const
{
    return ajustesactuales;
}

void AjustesSensores::aplicar()
{
    ajustesactuales.clear();
    ajustesactuales+="a"+QString::number(ui->comboBoxAscale->currentIndex());
    ajustesactuales+="g"+QString::number(ui->comboBoxGscale->currentIndex());
    ajustesactuales.append("l"+QString::number(ui->spinBoxDLPF->value()));
    ajustesactuales+="s"+QString::number(ui->spinBoxSampleRate->value());
    hide();
}

void AjustesSensores::on_spinBoxSampleRate_valueChanged(int arg1)
{
    if(ui->spinBoxDLPF->value()==0)
        mostrarFrecuenciaMuestreo(8000.0/(arg1+1));

    else
        mostrarFrecuenciaMuestreo(1000.0/(arg1+1));
}

void AjustesSensores::on_spinBoxDLPF_valueChanged(int arg1)
{
    if(arg1==0){
        mostrarFrecuenciaMuestreo(8000.0/(ui->spinBoxSampleRate->value()+1));
        mostrarFiltroPasaBajo("Desactivado");
    }
    else{
        mostrarFrecuenciaMuestreo(1000.0/(ui->spinBoxSampleRate->value()+1));
        mostrarFiltroPasaBajo("Activado");
    }
}
