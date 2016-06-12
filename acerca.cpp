#include "acerca.h"
#include "ui_acerca.h"

Acerca::Acerca(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Acerca)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->labelBuild->setText(+"Compilado: " __DATE__  " a las: " __TIME__);
    ui->labelQtVersion->setText("Compilado en: " QT_VERSION_STR);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(close()));
    this->setFixedSize(QSize(410,200));
}

Acerca::~Acerca()
{
    delete ui;
}
