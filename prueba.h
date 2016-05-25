#ifndef PRUEBA_H
#define PRUEBA_H

#include <QObject>
#include <qcustomplot.h>
#include <ajustesgrafico.h>
#include <ajustespuertoserial.h>
#include <ajustessensores.h>

class Prueba : public QObject
{
    Q_OBJECT
public:
    explicit Prueba(QObject *parent = 0);

signals:

private:
    int numeroPrueba;
    int numeroObjetivos;
    bool aleatorios;
    bool detenerAlMarcarTodos;
    bool limitarGrafico;
    double tiempoPrueba;
    int divisorFPS;
    QList<QCPItemEllipse*> listaObjetivos;
    double alturaDispositivo;
    QString orientacion;
    double frecuenciaMuestreo;
    QString unidad;
    QString cadenaConfiguracion;
    int cantidadMuestras;
    double tiempoTotal;

    AjustesGrafico::Ajustes ajustesGrafico;
    AjustesPuertoSerial::Ajustes ajustesPuertoSerial;
    AjustesSensores::Ajustes ajustesSensores;

public slots:
    void setNumeroPrueba(const int numero);
    void setNumeroObjetivos(const int numeroObjetivos);
    void setAleatorios(const bool aleatorio);
    void setDetenerAlMarcarTodos(const bool detener);
    void setLimitarGrafico(const bool limitar);
    void setTiempoPrueba(const double tiempo);
    void setDivisorFPS();
    void setDivisorFPS(const int divisor);
    void setListaObjetivos(const QList<QCPItemEllipse*> listaObjetivos);
    void setAlturaDispositivo(const double altura);
    void setOrientacion(const QString orientacion);
    void setFrecuenciaMuestreo(const double frecuencia);
    void setUnidad(const QString unidad);
    void setCadenaConfiguracion(const QString cadena);
    void setCantidadMuestras(const int cantidad);
    void setTiempoTotal(const double tiempo);

    void setAjustesGrafico(const AjustesGrafico::Ajustes ajustes);
    void setAjustesPuertoSerial(const AjustesPuertoSerial::Ajustes ajustes);
    void setAjustesSensores(const AjustesSensores::Ajustes ajustes);

    int getNumeroPrueba();
    int getNumeroObjetivos();
    bool getAleatorios();
    bool getDetenerAlMarcarTodos();
    bool getLimitarGrafico();
    double getTiempoPrueba();
    int getDivisorFPS();
    QList<QCPItemEllipse *> getListaObjetivos();
    double getAlturaDispositivo();
    QString getOrientacion();
    double getFrecuenciaMuestreo();
    QString getUnidad();
    QString getCadenaConfiguracion();
    int getCantidadMuestras();
    double getTiempoTotal();

    AjustesGrafico::Ajustes getAjustesGrafico();
    AjustesPuertoSerial::Ajustes getAjustesPuertoSerial();
    AjustesSensores::Ajustes getAjustesSensores();

};

#endif // PRUEBA_H
