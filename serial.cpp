#include "serial.h"
#include <QTimer>

Serial::Serial(QObject *parent) : QObject(parent)
{
    conexiones();
}

Serial::Serial(QObject *parent, QSerialPort *serial) : QObject(parent),serial(serial)
{
    conexiones();
}

void Serial::inicializar()
{

}

void Serial::conexiones()
{
    connect(serial,SIGNAL(readyRead()),this,SLOT(leerDatosSerial()));
}


bool Serial::abrirPuertoSerial(AjustesPuertoSerial::Ajustes ajustesSerial)
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
        //serial->clear();
        serial->setDataTerminalReady(true);
        serial->setRequestToSend(true);
        return true;
    } else {
        //QMessageBox::critical(0, tr("Error"), serial->errorString());
        return false;
    }
    return false;
}

void Serial::cerrarPuertoSerial()
{
    if(serial->isOpen())
        serial->close();
}

void Serial::leerDatosSerial()
{
    while (serial->canReadLine()){
        const QByteArray datosSerial = serial->readLine();
        QStringList linea=QString(datosSerial).split(" ");
        if(linea.size()==6){
            const double AcX=QString(linea.at(0)).toDouble();
            const double AcY=QString(linea.at(1)).toDouble();
            const double AcZ=QString(linea.at(2)).toDouble();
            const double GyX=QString(linea.at(3)).toDouble();
            const double GyY=QString(linea.at(4)).toDouble();
            const double GyZ=QString(linea.at(5)).toDouble();
            emit datosLeidos(AcX,AcY,AcZ,GyX,GyY,GyZ);
        }
    }
}


void Serial::escribirDatosSerial(QString cadena)
{
    serial->clear();
    serial->write(cadena.toLocal8Bit());

}
