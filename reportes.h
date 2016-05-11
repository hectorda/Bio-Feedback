#ifndef REPORTES_H
#define REPORTES_H

#include <QObject>
#include <raw.h>
#include <angulo.h>
#include <qcustomplot.h>
#include <QFileDialog>
#include <QMessageBox>

class Reportes : public QObject
{
    Q_OBJECT
public:
    explicit Reportes(QObject *parent = 0);
    Reportes(QObject *parent,QCustomPlot *graficoResultados,QCustomPlot *graficoAngulos,QCustomPlot *graficoMuestras,QTableWidget *tablaAngulos,QTableWidget *tablaMuestras);

    void graficarResultados(QList<Angulo*> listaAngulos);

    void guardarImagenGrafico(QCustomPlot *grafico,int ancho,int alto);
    void guardarMuestrasEnArchivo(QList<Raw*> listaMuestras);
    void guardarAngulosEnArchivo(QList<Angulo*> listaAngulos);
    void vaciarTablas();
    void vaciarGraficos();

private:
    int presicion;

    QCustomPlot *graficoResultados;
    QCustomPlot *graficoAngulos;
    QCustomPlot *graficoMuestras;
    QTableWidget *tablaAngulos;
    QTableWidget *tablaMuestras;

    //Plotables de los Graficos del Angulo
    QCPGraph *graficoAnguloX;
    QCPGraph *graficoAnguloY;

    //Plotables de los Graficos de las Muestras
    QCPGraph *graficoAcX;
    QCPGraph *graficoAcY;
    QCPGraph *graficoAcZ;
    QCPGraph *graficoGyX;
    QCPGraph *graficoGyY;
    QCPGraph *graficoGyZ;

    //Funciones donde se crean los layouts y elementos de los graficos del reporte
    void inicializarGraficoAngulos();
    void inicializarGraficoMuestras();

    //Se
    void limpiarGrafico(QCustomPlot *grafico);
    void limpiarTabla(QTableWidget *tabla);


signals:

public slots:
    //Slots para agregar datos
    void agregarDatosGraficoMuestras(Raw *datos);
    void agregarDatosGraficoAngulos(Angulo *angulo);
    void agregarFilaTablaAngulos(Angulo *angulo);
    void agregarFilaTablaMuestras(Raw *datos);

};

#endif // REPORTES_H
