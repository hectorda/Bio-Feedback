#ifndef ANALISISGRAFICO_H
#define ANALISISGRAFICO_H

#include <QDialog>
#include <qxtspanslider.h>
#include <reportes.h>
#include <angulo.h>
#include <desplazamiento.h>
#include <muestra.h>

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
    void setListaAngulos(QVector<Angulo *> LA);
    void setListaDesplazamientosProyeccion(QVector<Desplazamiento *> LD);
    void setListaDesplazamientosRecorridoCurvo(QVector<Desplazamiento *> LD);
    void setListaMuestras(QVector<Muestra *> LR);

    void ajustarRangosGraficoAngulos(const int inicio, const int fin);
    void ajustarRangosGraficoDesplazamientoProyeccion(const int inicio, const int fin);
    void ajustarRangosGraficoDesplazamientoRecorridoCurvo(const int inicio, const int fin);
    void ajustarRangosGraficoMuestras(const int inicio, const int fin);

    void contarDatosAngulos(const int inicio, const int fin);
    void contarDatosDesplazamientoProyeccion(const int inicio, const int fin);
    void contarDatosDesplazamientoRecorridoCurvo(const int inicio, const int fin);
    void contarDatosMuestras(const int inicio, const int fin);

    void calcularEstadisticosAngulos(const int inicio, const int fin);
    void calcularEstadisticosDesplazamientoProyeccion(const int inicio, const int fin);
    void calcularEstadisticosDesplazamientoRecorridoCurvo(const int inicio, const int fin);
    void calcularEstadisticosMuestras(const int inicio, const int fin);

    QImage obtenerImagenTablaEstadisticos();

private:
    Ui::AnalisisGrafico *ui;
    QVector<Angulo*> listaAngulos;
    QVector<Desplazamiento*> listaDesplazamientos;
    QVector<Muestra*> listaMuestras;
    Reportes *reportes;
    void MostrarCantidadDatos(const int datosTotales, const int datosAnalizados, const double frecuenciaMuestreo, const double tiempoTotal, const double tiempoAnalizado);
};

#endif // ANALISISGRAFICO_H
