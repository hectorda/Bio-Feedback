#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "qserialportinfo.h"
#include "qdebug.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    serial=new QSerialPort(this);
    //file.setFileName("out.txt");

    //ui->tiempo->setValidator(new QIntValidator(0,200,this));
    foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
        ui->portNameCB->addItem(info.portName());
    }
    ui->baudRate->setText("Baudios: 115200");
    ui->baudRateCB->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateCB->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->baudRateCB->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateCB->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateCB->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    init_Connections();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::init_Connections()
{
    connect(ui->pushButtonOK,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->pushButtonExit,SIGNAL(clicked()),this,SLOT(close()));

}
