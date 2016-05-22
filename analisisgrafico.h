#ifndef ANALISISGRAFICO_H
#define ANALISISGRAFICO_H

#include <QDialog>
#include <qxtspanslider.h>
#include <reportes.h>
#include <angulo.h>
#include <desplazamiento.h>
#include <raw.h>

namespace Ui {
class AnalisisGrafico;
}

class AnalisisGrafico : public QDialog
{
    Q_OBJECT

public:
    explicit AnalisisGrafico(QWidget *parent = 0);
    AnalisisGrafico(QWidget *parent, Reportes *reportes);
    ~AnalisisGrafico();    
    void setListaAngulos(QList<Angulo *> LA);
    void setListaDesplazamientos(QList<Desplazamiento *> LD);
    void setListaMuestras(QList<Raw *> LR);

    void ajustarRangosGraficoAngulos(const int inicio, const int fin);
    void ajustarRangosGraficoDesplazamientos(const int inicio, const int fin);
    void ajustarRangosGraficoMuestras(const int inicio, const int fin);

    void contarDatosAngulos(const int inicio, const int fin);
    void contarDatosDesplazamientos(const int inicio, const int fin);
    void contarDatosMuestras(const int inicio, const int fin);

    void calcularEstadisticosAngulos(const int inicio, const int fin);
    void calcularEstadisticosDesplazamientos(const int inicio, const int fin);
    void calcularEstadisticosMuestras(const int inicio, const int fin);

private:
    Ui::AnalisisGrafico *ui;
    QList<Angulo*> listaAngulos;
    QList<Desplazamiento*> listaDesplazamientos;
    QList<Raw*> listaMuestras;
    Reportes *reportes;
    void MostrarCantidadDatos(const int datosTotales, const int datosAnalizados, const double frecuenciaMuestreo, const double tiempoTotal, const double tiempoAnalizado);
};

#endif // ANALISISGRAFICO_H
