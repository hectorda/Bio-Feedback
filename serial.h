#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <raw.h>
#include <ajustespuertoserial.h>
#include <QTextStream>
#include <QMessageBox>

class Serial : public QObject
{
    Q_OBJECT
public:
    QSerialPort *serial;
    explicit Serial(QObject *parent = 0);
    void abrirPuertoSerial(QSerialPort *serial,AjustesPuertoSerial::Ajustes ajustesSerial, QString ajustesSensores);
    void cerrarPuertoSerial(QSerialPort *serial);
    Raw leerDatosSerial(QSerialPort *serial,const double tiempo);

signals:

public slots:
};

#endif // SERIAL_H
