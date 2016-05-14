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
    Serial(QObject *parent,QSerialPort *serial);
    bool abrirPuertoSerial(AjustesPuertoSerial::Ajustes ajustesSerial);
    void cerrarPuertoSerial();

private:
    void inicializar();
    void conexiones();
    QByteArray buffer;

signals:
    void datosLeidos(double,double,double,double,double,double);

public slots:
    void leerDatosSerial();
    void escribirDatosSerial(QString cadena);
};

#endif // SERIAL_H
