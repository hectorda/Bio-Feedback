#ifndef REPORTES_H
#define REPORTES_H

#include <QObject>
#include <raw.h>
#include <angulo.h>
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
    Reportes(QObject *parent,QCustomPlot *graficoResultados,QCustomPlot *graficoAnguloX,QCustomPlot *graficoAnguloY,QCustomPlot *graficoMuestras,QTableWidget *tablaAngulos,QTableWidget *tablaMuestras);

    void guardarImagenGrafico(QCustomPlot *grafico,int ancho,int alto);
    void guardarMuestrasEnArchivo(QList<Raw*> listaMuestras);
    void guardarAngulosEnArchivo(QList<Angulo*> listaAngulos);

    //Para dejar los espacios de reportes sin datos.
    void vaciarTablas();
    void vaciarGraficos();

    void analizarGraficosAngulos(QWidget *parent, double tiempoPrueba, QList<Angulo *> listaAngulos);
    void eliminarDialogAnalisis();

private:
    int presicion;
    QDialog *QDialogAnalisis;

    QCustomPlot *graficoResultados;
    QCustomPlot *graficoAnguloX;
    QCustomPlot *graficoAnguloY;
    QCustomPlot *graficoMuestras;
    QTableWidget *tablaAngulos;
    QTableWidget *tablaMuestras;

    //Plotables del Rrafico Resultado
    QCPBars *cuadrantes;

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
    void inicializarGraficoMuestras();

signals:

public slots:
    //Slots para agregar datos
    void agregarDatosGraficoMuestras(Raw *datos);
    void agregarDatosGraficoAngulos(Angulo *angulo);
    void agregarFilaTablaAngulos(Angulo *angulo);
    void agregarFilaTablaMuestras(Raw *datos);
    void graficarResultados(QList<Angulo*> listaAngulos);

};

#endif // REPORTES_H
