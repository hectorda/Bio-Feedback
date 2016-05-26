#ifndef REPORTES_H
#define REPORTES_H

#include <QObject>
#include <muestra.h>
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
    void guardarMuestrasEnArchivo(QList<Muestra*> listaMuestras);

    //Para dejar los espacios de reportes sin datos.
    void vaciarTablas();
    void vaciarGraficos();    
    void setDatosTablaAngulos(QList<Angulo *> listaAngulos);
    void setDatosTablaDesplazamientos(QList<Desplazamiento *> listaDesplazamientos);
    void setDatosTablaMuestras(QList<Muestra *> listaMuestras);
    void setDatosGraficoAngulos(QList<Angulo *> listaAngulos);
    void setDatosGraficoDezplazamiento(QList<Desplazamiento *> listaDesplazamientos);
    void setDatosGraficoMuestras(QList<Muestra *> listaMuestras);

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
    QCPItemLine *lineaIzqAnguloX;
    QCPItemLine *lineaDerAnguloX;
    QCPItemLine *lineaIzqAnguloY;
    QCPItemLine *lineaDerAnguloY;

    //Item para el ajuste de los rangos del Grafico Desplazamientos
    QCPItemLine *lineaIzqDesplazamientoX;
    QCPItemLine *lineaDerDesplazamientoX;
    QCPItemLine *lineaIzqDesplazamientoY;
    QCPItemLine *lineaDerDesplazamientoY;

    //Item para el ajuste de los rangos del Grafico Muestras
    QCPItemLine *lineaIzqAcX;
    QCPItemLine *lineaDerAcX;
    QCPItemLine *lineaIzqAcY;
    QCPItemLine *lineaDerAcY;
    QCPItemLine *lineaIzqAcZ;
    QCPItemLine *lineaDerAcZ;
    QCPItemLine *lineaIzqGyX;
    QCPItemLine *lineaDerGyX;
    QCPItemLine *lineaIzqGyY;
    QCPItemLine *lineaDerGyY;
    QCPItemLine *lineaIzqGyZ;
    QCPItemLine *lineaDerGyZ;

    //Plotables de los Graficos de las Muestras
    QCPGraph *graficoAcX;
    QCPGraph *graficoAcY;
    QCPGraph *graficoAcZ;
    QCPGraph *graficoGyX;
    QCPGraph *graficoGyY;
    QCPGraph *graficoGyZ;

    //Funcion para agregar un ItemLine a un QAxisRect de un grafico
    //Se debe incializar el Parent de QCPItemLine antes de llamar la funcion.
    void agregarQCPItemLine(QCPItemLine *linea, QCustomPlot *grafico,QCPAxisRect *contenedor);

    //Funciones donde se crean los layouts y elementos de los graficos del reporte
    void inicializarGraficoResultados();
    void inicializarGraficoAngulos();
    void inicializarGraficoDesplazamientos();
    void inicializarGraficoMuestras();

signals:

public slots:
    void graficarResultados(QList<Angulo*> listaAngulos);
    //Funciones para mover las lineas tanto izquierda y derechas de los graficos
    void moverLineasIzquierdaAngulos(const double newValue);
    void moverLineasDerechaAngulos(const double newValue);
    void moverLineasIzquierdaDesplazamientos(const double newValue);
    void moverLineasDerechaDesplazamientos(const double newValue);
    void moverLineasDerechaMuestras(const double newValue);
    void moverLineasIzquierdaMuestras(const double newValue);

    //Slots para agregar datos: Graficos
    void agregarDatosGraficoAngulos(Angulo *angulo);
    void agregarDatosGraficoDesplazamientos(Desplazamiento *desp);
    void agregarDatosGraficoMuestras(Muestra *datos);

    //Slots para agregar datos: Tablas
    void agregarFilaTablaAngulos(Angulo *angulo);
    void agregarFilaTablaDesplazamientos(Desplazamiento *desp);
    void agregarFilaTablaMuestras(Muestra *datos);

    //Slots para Limpiar
    void vaciarGraficoAngulos();
    void vaciarGraficoDesplazamientos();
    void vaciarGraficoMuestras();

    //Para redibujar los graficos
    void replotGraficoAngulos();
    void replotGraficoDesplazamientos();
    void replotGraficoMuestras();

};

#endif // REPORTES_H
