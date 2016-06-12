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
    ui->labelFiltroUsado->setText("Filtro usado: "+arg1);
    if(arg1.toLower().contains("sin filtro")){
        ui->doubleSpinBoxAlphaFC->hide();
        ui->labelAlpha->hide();
    }
    if(arg1.toLower().contains("kalman")){
        ui->doubleSpinBoxAlphaFC->hide();
        ui->labelAlpha->hide();
    }
    if(arg1.toLower().contains("complementario")){
        ui->doubleSpinBoxAlphaFC->show();
        ui->labelAlpha->show();
    }
}

void AjustesCalculoAngulo::aplicar()
{
    bool sinfiltro=ui->comboBoxFiltroAngulo->currentText().toLower().contains("sin filtro");
    if(sinfiltro){
        QMessageBox messageBox(QMessageBox::Warning,
                    "Calcular ángulo sin usar filtro?",
                    "Advertencia, el calcular el ángulo sin filtro es solo para efectos de pruebas"
                       "\n No se recomienda el uso en una prueba real."
                       "\n¿Desea usarlo de todos modos?",
                    QMessageBox::Yes | QMessageBox::No,
                    this);

        messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
        messageBox.setButtonText(QMessageBox::No, tr("Cancelar"));

        if (messageBox.exec() == QMessageBox::Yes){
            filtro=ui->comboBoxFiltroAngulo->currentText();
            alpha=ui->doubleSpinBoxAlphaFC->value();
            close();
        }
    }
    else
    {
        filtro=ui->comboBoxFiltroAngulo->currentText();
        alpha=ui->doubleSpinBoxAlphaFC->value();
        close();
    }
}
