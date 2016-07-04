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
#include <paciente.h>
#include <objetivo.h>

class Prueba : public QObject
{
    Q_OBJECT
public:
    explicit Prueba(QObject *parent = 0);
    QVector<Muestra*> listaMuestras;
    QVector<Angulo*> listaAngulos;
    QVector<Desplazamiento*> listaDesplazamientos;
    QVector<Objetivo*> listaObjetivos;

    void setNombrePrueba(const QString nombre);
    void setCantidadObjetivos(const int cantidadObjetivos);
    void setAleatorios(const bool aleatorio);
    void setDetenerAlMarcarTodos(const bool detener);
    void setLimitarGrafico(const bool limitar);
    void setObjetivosEnOrden(const bool enorden);
    void setTiempoPrueba(const double tiempo);
    void setDivisorFPS();
    void setDivisorFPS(const int divisor);
    void setListaObjetivos(const QVector<Objetivo *> listaObjetivos);
    void setAlturaDispositivo(const double altura);
    void setOrientacion(const QString orientacion);
    void setFrecuenciaMuestreo(const double frecuencia);
    void setCadenaConfiguracion(const QString cadena);
    void setCantidadMuestras(const int cantidad);
    void setTiempoTotal(const double tiempo);

    void setAjustesGrafico(const AjustesGrafico::Ajustes ajustes);
    void setAjustesPuertoSerial(const AjustesPuertoSerial::Ajustes ajustes);

    QString getNombrePrueba();
    int getCantidadObjetivos();
    bool getAleatorios();
    bool getDetenerAlMarcarTodos();
    bool getLimitarGrafico();
    bool getObjetivosEnOrden();
    double getTiempoPrueba();
    int getDivisorFPS();
    QVector<Objetivo *> getListaObjetivos();
    double getAlturaDispositivo();
    QString getOrientacion();
    double getFrecuenciaMuestreo();
    QString getCadenaConfiguracion();
    int getCantidadMuestras();
    double getTiempoTotal();

    AjustesGrafico::Ajustes getAjustesGrafico();
    AjustesPuertoSerial::Ajustes getAjustesPuertoSerial();

    Paciente getPaciente() const;
    void setPaciente(const Paciente &value);

    void limpiarListas();
    void detenerTimerObjetivos();
    void exportar();
    bool importar();

signals:

private:
    QString nombrePrueba;
    int cantidadObjetivos;
    bool aleatorios;
    bool detenerAlMarcarTodos;
    bool limitarGrafico;
    bool objetivosEnOrden;
    double tiempoPrueba;
    int divisorFPS;
    double alturaDispositivo;
    QString orientacion;
    double frecuenciaMuestreo;
    QString cadenaConfiguracion;
    int cantidadMuestras;
    double tiempoTotal;
    Paciente paciente;

    AjustesGrafico::Ajustes ajustesGrafico;
    AjustesPuertoSerial::Ajustes ajustesPuertoSerial;
    AjustesSensores::Ajustes ajustesSensores;

public slots:

};

#endif // PRUEBA_H
