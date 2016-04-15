#ifndef AJUSTESPUERTOSERIAL_H
#define AJUSTESPUERTOSERIAL_H

#include <QDialog>
#include <QSerialPort>

namespace Ui {
class AjustesPuertoSerial;
}
class QIntValidator;
class AjustesPuertoSerial : public QDialog
{
    Q_OBJECT

public:
    struct Ajustes {
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
    };

    Ajustes getAjustes() const;

public:
    explicit AjustesPuertoSerial(QWidget *parent = 0);
    ~AjustesPuertoSerial();

private:
    Ui::AjustesPuertoSerial *ui;
    Ajustes ajustesActuales;
    QIntValidator *intValidator;

private:
    void llenarParametros();
    void informacionPuertos();
    void actualizarAjustes();
    void inicializar();
    void conexiones();

private slots:
    void mostrarInformacionPuerto(int idx);
    void aplicar();

};

#endif // AJUSTESPUERTOSERIAL_H
