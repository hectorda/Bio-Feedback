#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <muestra.h>
#include <ajustespuertoserial.h>

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial(QObject *parent = 0);
    Serial(QObject *parent,QSerialPort *serial);
    bool abrirPuertoSerial(AjustesPuertoSerial::Ajustes ajustesSerial);
    void cerrarPuertoSerial();

private:
    QSerialPort *serial;
    void conexiones();

signals:
    void datosLeidos(double,double,double,double,double,double);

public slots:
    void leerDatosSerial();
    void escribirDatosSerial(QString cadena);
};

#endif // SERIAL_H
