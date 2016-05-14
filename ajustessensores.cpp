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
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void AjustesSensores::conexiones()
{
    connect(ui->pushButtonOK, SIGNAL(clicked()),this, SLOT(aplicar()));
    connect(ui->pushButtonExit,SIGNAL(clicked()),this,SLOT(close()));
}

void AjustesSensores::llenarParametros()
{
    for (int var = 0; var < 256; ++var)
        ui->comboBoxFrecuenciaMuestreo->addItem(QString::number(8000.0/(var+1),'f',2),var);

    ui->comboBoxFrecuenciaMuestreo->setCurrentIndex(39);

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

    frecuenciaMuestreo=ui->comboBoxFrecuenciaMuestreo->currentText().toDouble();
    mostrarFrecuenciaMuestreo(QString::number(frecuenciaMuestreo,'f',2));
    mostrarFiltroPasaBajo("Desactivado");
}

void AjustesSensores::mostrarFrecuenciaMuestreo(const QString SampleRate)
{
    const QString fSampleRate=tr("Frecuencia de Muestreo: %1 Hz").arg(SampleRate);
    ui->labelSampleRate->setText(fSampleRate);
    ui->lcdNumberSampleRate->display(SampleRate);
}

void AjustesSensores::mostrarFiltroPasaBajo(const QString DLPF)
{
    ui->labelDLPF->setText("Filtro Pasa-Bajo: "+DLPF);
}

QString AjustesSensores::getAjustesSensores() const
{
    QString cadenaAjustes;
    cadenaAjustes.append("g"+QString::number(ajustesactuales.configuracionGiroscopio));
    cadenaAjustes.append("a"+QString::number(ajustesactuales.configuracionAcelerometro));
    cadenaAjustes.append("l"+QString::number(ajustesactuales.filtroPasaBajo));
    cadenaAjustes.append("s"+QString::number(ajustesactuales.divisorFrecuenciaMuestreo));

    return cadenaAjustes;
}

double AjustesSensores::obtenerFrecuenciaMuestreo()
{
    if(ajustesactuales.filtroPasaBajo==0)
        frecuenciaMuestreo=8000.0/(ajustesactuales.divisorFrecuenciaMuestreo+1);

    else
        frecuenciaMuestreo=(1000.0/(ajustesactuales.divisorFrecuenciaMuestreo+1));

    return frecuenciaMuestreo;
}

void AjustesSensores::aplicar()
{
    if(ui->comboBoxFrecuenciaMuestreo->currentText().toDouble()>275){
        QMessageBox messageBox(QMessageBox::Warning,
                    "Frecuencia de Muestreo muy alta?",
                    "Advertencia, el usar una frecuencia de muestreo superior a 275"
                       "\n no garantiza el correcto funcionamiento."
                       "\n¿Desea configurarla de todos modos?",
                    QMessageBox::Yes | QMessageBox::No,
                    this);

        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.setButtonText(QMessageBox::No, tr("Cancelar"));

        if (messageBox.exec() == QMessageBox::Yes){
            ajustesactuales.configuracionGiroscopio=ui->comboBoxGscale->currentIndex();
            ajustesactuales.configuracionAcelerometro=ui->comboBoxAscale->currentIndex();
            ajustesactuales.filtroPasaBajo = ui->spinBoxDLPF->value();
            ajustesactuales.divisorFrecuenciaMuestreo = ui->comboBoxFrecuenciaMuestreo->currentIndex();
            hide();

        }
    }
    else{
        ajustesactuales.configuracionGiroscopio=ui->comboBoxGscale->currentIndex();
        ajustesactuales.configuracionAcelerometro=ui->comboBoxAscale->currentIndex();
        ajustesactuales.filtroPasaBajo = ui->spinBoxDLPF->value();
        ajustesactuales.divisorFrecuenciaMuestreo = ui->comboBoxFrecuenciaMuestreo->currentIndex();
        hide();
    }
}

void AjustesSensores::on_spinBoxDLPF_valueChanged(int arg1)
{
    const int indiceCBFrecuenciaMuestreo=ui->comboBoxFrecuenciaMuestreo->currentIndex();
    if(arg1==0){
        ui->comboBoxFrecuenciaMuestreo->clear();
        for (int var = 0; var < 256; ++var)
            ui->comboBoxFrecuenciaMuestreo->addItem(QString::number(8000.0/(var+1),'f',2),var);

        ui->comboBoxFrecuenciaMuestreo->setCurrentIndex(indiceCBFrecuenciaMuestreo);
        mostrarFrecuenciaMuestreo(ui->comboBoxFrecuenciaMuestreo->currentText());
        mostrarFiltroPasaBajo("Desactivado");
    }
    else{
        ui->comboBoxFrecuenciaMuestreo->clear();
        for (int var = 0; var < 256; ++var)
            ui->comboBoxFrecuenciaMuestreo->addItem(QString::number(1000.0/(var+1),'f',2),var);

        ui->comboBoxFrecuenciaMuestreo->setCurrentIndex(indiceCBFrecuenciaMuestreo);
        mostrarFrecuenciaMuestreo(ui->comboBoxFrecuenciaMuestreo->currentText());
        if(arg1==1)
            mostrarFiltroPasaBajo("Ac:184Hz Gy:188Hz ");
        if(arg1==2)
            mostrarFiltroPasaBajo("Ac: 94Hz Gy: 98Hz  ");
        if(arg1==3)
            mostrarFiltroPasaBajo("Ac: 44Hz Gy: 42Hz ");
        if(arg1==4)
            mostrarFiltroPasaBajo("Ac: 21Hz Gy: 20Hz ");
        if(arg1==5)
            mostrarFiltroPasaBajo("Ac: 10Hz Gy: 10Hz ");
        if(arg1==6)
            mostrarFiltroPasaBajo("Ac:  5Hz Gy:  5Hz ");
    }
}

void AjustesSensores::on_comboBoxFrecuenciaMuestreo_currentIndexChanged(const QString &arg1)
{
    mostrarFrecuenciaMuestreo(arg1);
}
