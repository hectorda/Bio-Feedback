#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSerialPort>

namespace Ui {
class SettingsDialog;
}
class QIntValidator;
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct Settings {
        QString portName;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    Settings getCurrentSettings() const;

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;
    Settings currentSettings;
    QIntValidator *intValidator;

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();
    void init_Connections();

private slots:
    void showPortInfo(int idx);
    void apply();




};

#endif // SETTINGSDIALOG_H
