#ifndef PRUEBA_H
#define PRUEBA_H

#include <QObject>
#include <QMessageBox>
#include <qcustomplot.h>
#include <ajustesgrafico.h>
#include <ajustespuertoserial.h>
#include <ajustessensores.h>
#include <angulo.h>
#include <desplazamiento.h>
#include <Muestra.h>

class Prueba : public QObject
{
    Q_OBJECT
public:
    explicit Prueba(QObject *parent = 0);

signals:

private:
    int numeroPrueba;
    int cantidadObjetivos;
    bool aleatorios;
    bool detenerAlMarcarTodos;
    bool limitarGrafico;
    bool objetivosEnOrden;
    double tiempoPrueba;
    int divisorFPS;
    QList<QCPItemEllipse*> listaObjetivos;
    double alturaDispositivo;
    QString orientacion;
    double frecuenciaMuestreo;
    QString cadenaConfiguracion;
    int cantidadMuestras;
    double tiempoTotal;

    AjustesGrafico::Ajustes ajustesGrafico;
    AjustesPuertoSerial::Ajustes ajustesPuertoSerial;
    AjustesSensores::Ajustes ajustesSensores;

public slots:
    void setNumeroPrueba(const int numero);
    void setCantidadObjetivos(const int cantidadObjetivos);
    void setAleatorios(const bool aleatorio);
    void setDetenerAlMarcarTodos(const bool detener);
    void setLimitarGrafico(const bool limitar);
    void setObjetivosEnOrden(const bool enorden);
    void setTiempoPrueba(const double tiempo);
    void setDivisorFPS();
    void setDivisorFPS(const int divisor);
    void setListaObjetivos(const QList<QCPItemEllipse*> listaObjetivos);
    void setAlturaDispositivo(const double altura);
    void setOrientacion(const QString orientacion);
    void setFrecuenciaMuestreo(const double frecuencia);
    void setCadenaConfiguracion(const QString cadena);
    void setCantidadMuestras(const int cantidad);
    void setTiempoTotal(const double tiempo);

    void setAjustesGrafico(const AjustesGrafico::Ajustes ajustes);
    void setAjustesPuertoSerial(const AjustesPuertoSerial::Ajustes ajustes);

    int getNumeroPrueba();
    int getCantidadObjetivos();
    bool getAleatorios();
    bool getDetenerAlMarcarTodos();
    bool getLimitarGrafico();
    bool getObjetivosEnOrden();
    double getTiempoPrueba();
    int getDivisorFPS();
    QList<QCPItemEllipse *> getListaObjetivos();
    double getAlturaDispositivo();
    QString getOrientacion();
    double getFrecuenciaMuestreo();
    QString getCadenaConfiguracion();
    int getCantidadMuestras();
    double getTiempoTotal();

    AjustesGrafico::Ajustes getAjustesGrafico();
    AjustesPuertoSerial::Ajustes getAjustesPuertoSerial();
    void exportar(QList<Angulo *> listaAngulos, QList<Desplazamiento *> listaDesplazamientos, QList<Muestra*> listaMuestras);

};

#endif // PRUEBA_H
