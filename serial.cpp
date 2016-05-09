#include "serial.h"

Serial::Serial(QObject *parent) : QObject(parent)
{

}

void Serial::abrirPuertoSerial(QSerialPort *serial, AjustesPuertoSerial::Ajustes ajustesSerial, QString ajustesSensores)
{
    serial->setPortName(ajustesSerial.portName);
    serial->setBaudRate(ajustesSerial.baudRate);
    QTextStream(stdout)<<"Baudios: "<< serial->baudRate()<<endl;
    QTextStream(stdout)<<"portName"<< serial->portName()<<endl;
    serial->setStopBits(QSerialPort::OneStop);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)){

        //serial->dataTerminalReadyChanged(true);
        //serial->requestToSendChanged(true);
        serial->clear();
        QTextStream(stdout)<<"Cadena de Configuracion: " <<ajustesSensores<<endl;
        serial->write("v0"+ajustesSensores.toLocal8Bit());

    } else {
        QMessageBox::critical(0, tr("Error"), serial->errorString());
    }
}

void Serial::cerrarPuertoSerial(QSerialPort *serial)
{
    if(serial->isOpen())
        serial->close();
}

Raw Serial::leerDatosSerial(QSerialPort *serial, const double tiempo)
{
    const QByteArray datosSerial = serial->readLine();
    QStringList linea=QString(datosSerial).split(" ");

    const double AcX=QString(linea.at(0)).toDouble();
    const double AcY=QString(linea.at(1)).toDouble();
    const double AcZ=QString(linea.at(2)).toDouble();
    const double GyX=QString(linea.at(3)).toDouble();
    const double GyY=QString(linea.at(4)).toDouble();
    const double GyZ=QString(linea.at(5)).toDouble();
    Raw dato=Raw(tiempo,AcX,AcY,AcZ,GyX,GyY,GyZ);

    return dato;
}
