#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "qserialportinfo.h"
#include "qdebug.h"

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
        ui->comboBoxPortList->addItem(info.portName());
    }
    //ui->labelBaudRate->setText("Baudios: 115200");
     init_Connections();

    fillPortsParameters();
    fillPortsInfo();

    updateSettings();

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::init_Connections()
{

    connect(ui->pushButtonOK, SIGNAL(clicked()),this, SLOT(apply()));
    connect(ui->comboBoxPortList, SIGNAL(currentIndexChanged(int)),this, SLOT(showPortInfo(int)));

    connect(ui->pushButtonExit,SIGNAL(clicked()),this,SLOT(close()));

}


SettingsDialog::Settings SettingsDialog::getCurrentSettings() const
{
    return currentSettings;
}

void SettingsDialog::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    QStringList list = ui->comboBoxPortList->itemData(idx).toStringList();
    ui->labelDescription->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->labelManufacturer->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->labelSerialNumber->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
//    ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
//    ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
//    ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SettingsDialog::apply()
{
    updateSettings();
    hide();
}


void SettingsDialog::fillPortsParameters()
{
    ui->comboBoxBaudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->comboBoxBaudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->comboBoxBaudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->comboBoxBaudRate->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->comboBoxBaudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->comboBoxBaudRate->setCurrentIndex(4);

    ui->comboBoxDataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->comboBoxDataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->comboBoxDataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->comboBoxDataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->comboBoxDataBits->setCurrentIndex(3);

    ui->comboBoxParity->addItem(tr("None"), QSerialPort::NoParity);
    ui->comboBoxParity->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->comboBoxParity->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->comboBoxParity->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->comboBoxParity->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->comboBoxBits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->comboBoxBits->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->comboBoxBits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->comboBoxFlowControl->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->comboBoxFlowControl->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->comboBoxFlowControl->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SettingsDialog::fillPortsInfo()
{
    ui->comboBoxPortList->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->comboBoxPortList->addItem(list.first(), list);
    }
}

void SettingsDialog::updateSettings()
{
    currentSettings.portName = ui->comboBoxPortList->currentText();

    if (ui->comboBoxBaudRate->currentIndex() == 4) {
        currentSettings.baudRate = ui->comboBoxBaudRate->currentText().toInt();
    } else {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->comboBoxBaudRate->itemData(ui->comboBoxBaudRate->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->comboBoxDataBits->itemData(ui->comboBoxDataBits->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->comboBoxDataBits->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->comboBoxParity->itemData(ui->comboBoxParity->currentIndex()).toInt());
    currentSettings.stringParity = ui->comboBoxParity->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->comboBoxBits->itemData(ui->comboBoxBits->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->comboBoxBits->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->comboBoxFlowControl->itemData(ui->comboBoxFlowControl->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->comboBoxFlowControl->currentText();

}
