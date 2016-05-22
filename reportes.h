#ifndef REPORTES_H
#define REPORTES_H

#include <QObject>
#include <raw.h>
#include <angulo.h>
#include <desplazamiento.h>
#include <qcustomplot.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <qxtspanslider.h>

class Reportes : public QObject
{
    Q_OBJECT
public:
    explicit Reportes(QObject *parent = 0);
    Reportes(QObject *parent,QCustomPlot *graficoResultados,QCustomPlot *graficoAngulos,QCustomPlot *graficoDesplazamientos,QCustomPlot *graficoMuestras,QTableWidget *tablaAngulos,QTableWidget *tablaDesplazamientos,QTableWidget *tablaMuestras);

    void guardarImagenGrafico(QCustomPlot *grafico,int ancho,int alto);
    void guardarAngulosEnArchivo(QList<Angulo*> listaAngulos);
    void guardarDesplazamientosEnArchivo(QList<Desplazamiento *> listaDesplazamientos);
    void guardarMuestrasEnArchivo(QList<Raw*> listaMuestras);

    //Para dejar los espacios de reportes sin datos.
    void vaciarTablas();
    void vaciarGraficos();

private:
    int presicion;
    QCustomPlot *graficoResultados;
    QCustomPlot *graficoAngulos;
    QCustomPlot *graficoDesplazamientos;
    QCustomPlot *graficoMuestras;
    QTableWidget *tablaAngulos;
    QTableWidget *tablaDesplazamientos;
    QTableWidget *tablaMuestras;

    //Plotables del Rrafico Resultado
    QCPBars *cuadrantes;

    //Plotables de los Graficos de Angulos
    QCPGraph *graficoAnguloX;
    QCPGraph *graficoAnguloY;

    //Plotables de los Graficos de Desplazamientos
    QCPGraph *graficoDesplazamientoX;
    QCPGraph *graficoDesplazamientoY;

    //Item para el ajuste de los rangos del Grafico Angulos
    QCPItemLine *lineaIzq1Angulos;
    QCPItemLine *lineaIzq2Angulos;
    QCPItemLine *lineaDer1Angulos;
    QCPItemLine *lineaDer2Angulos;

    //Item para el ajuste de los rangos del Grafico Desplazamientos
    QCPItemLine *lineaIzq1Desplazamientos;
    QCPItemLine *lineaIzq2Desplazamientos;
    QCPItemLine *lineaDer1Desplazamientos;
    QCPItemLine *lineaDer2Desplazamientos;

    //Plotables de los Graficos de las Muestras
    QCPGraph *graficoAcX;
    QCPGraph *graficoAcY;
    QCPGraph *graficoAcZ;
    QCPGraph *graficoGyX;
    QCPGraph *graficoGyY;
    QCPGraph *graficoGyZ;

    //Funciones donde se crean los layouts y elementos de los graficos del reporte
    void inicializarGraficoResultados();
    void inicializarGraficoAngulos();
    void inicializarGraficoDesplazamientos();
    void inicializarGraficoMuestras();

signals:

public slots:
    void graficarResultados(QList<Angulo*> listaAngulos);

    void moverLineasIzquierdaAngulos(const double newValue);
    void moverLineasDerechaAngulos(const double newValue);
    void moverLineasIzquierdaDesplazamientos(const double newValue);
    void moverLineasDerechaDesplazamientos(const double newValue);

    //Slots para agregar datos
    void agregarDatosGraficoAngulos(Angulo *angulo);
    void agregarDatosGraficoDesplazamientos(Desplazamiento *desp);
    void agregarDatosGraficoMuestras(Raw *datos);

    void agregarFilaTablaAngulos(Angulo *angulo);
    void agregarFilaTablaDesplazamientos(Desplazamiento *desp);
    void agregarFilaTablaMuestras(Raw *datos);

    //Slots para Limpiar
    void vaciarGraficoAngulos();
    void vaciarGraficoDesplazamientos();
    void vaciarGraficoMuestras();

    void replotGraficoAngulos();
    void replotGraficoDesplazamientos();
    void replotGraficoMuestras();

};

#endif // REPORTES_H
