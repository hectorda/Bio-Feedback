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
    void abrirPuertoSerial(AjustesPuertoSerial::Ajustes ajustesSerial, QString ajustesSensores);
    void cerrarPuertoSerial();

private:
    void inicializar();
    void conexiones();

signals:
    void datosLeidos(double,double,double,double,double,double);
    void emitEscribirSerial(QString);

public slots:
    void leerDatosSerial();
    void escribirDatosSerial(QString cadena);
};

#endif // SERIAL_H
