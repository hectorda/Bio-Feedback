#ifndef REPORTES_H
#define REPORTES_H

#include <QObject>
#include <Muestra.h>
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
    Reportes(QObject *parent, QCustomPlot *graficoResultados, QCustomPlot *graficoAngulos,QCustomPlot *graficoDesplazamientoPro,QCustomPlot *graficoDesplazamientoReCur, QCustomPlot *graficoMuestras, QTableWidget *tablaAngulos, QTableWidget *tablaDesplazamientosP, QTableWidget *tablaDesplazamientosRC, QTableWidget *tablaMuestras, QTextEdit *textEditReporte);

    void guardarImagenGrafico(QCustomPlot *grafico,int ancho,int alto);
    void guardarAngulosEnArchivo(QVector<Angulo*> listaAngulos);
    void guardarDesplazamientosEnArchivo(QVector<Desplazamiento *> listaDesplazamientos, QString tipo);
    void guardarMuestrasEnArchivo(QVector<Muestra*> listaMuestras);
    void guardarInformeReportePDF();

    //Para dejar los espacios de reportes sin datos.
    void vaciarTablas();
    void vaciarGraficos();    
    void vaciarInformeReporte();
    void agregarDatosInformeReporteImagen(const QString busq, const QImage imagen);
    void agregarDatosInformeReportePlainText(const QString busq,const QString dato);
    void agregarDatosInformeReporteHTML(const QString busq, const QString dato);
    void setDatosTablaAngulos(QVector<Angulo *> listaAngulos);
    void setDatosTablasDesplazamientos(QVector<Desplazamiento *> listaDesplazamientos);
    void setDatosTablaMuestras(QVector<Muestra *> listaMuestras);
    void setDatosGraficoAngulos(QVector<Angulo *> listaAngulos);
    void setDatosGraficosDezplazamientos(QVector<Desplazamiento *> listaDesplazamientos);
    void setDatosGraficoMuestras(QVector<Muestra *> listaMuestras);
    void inicializarInformeReporte();

private:
    int presicion;
    QCustomPlot *graficoResultados;
    QCustomPlot *graficoAngulos;
    QCustomPlot *graficoDesplazamientosProyeccion;
    QCustomPlot *graficoDesplazamientosRecorridoCurvo;
    QCustomPlot *graficoMuestras;
    QTableWidget *tablaAngulos;
    QTableWidget *tablaDesplazamientosProyeccion;
    QTableWidget *tablaDesplazamientosRecorridoCurvo;
    QTableWidget *tablaMuestras;
    QTextEdit *textEditReporte;

    //Plotables del Rrafico Resultado
    QCPBars *cuadrantes;

    //Plotables de los Graficos de Angulos
    QCPGraph *graficoAnguloX;
    QCPGraph *graficoAnguloY;

    //Plotables de los Graficos de Desplazamientos
    QCPGraph *graficoDesplazamientoProX;
    QCPGraph *graficoDesplazamientoProY;
    QCPGraph *graficoDesplazamientoReCurX;
    QCPGraph *graficoDesplazamientoReCurY;

    //Item para el ajuste de los rangos del Grafico Angulos
    QCPItemLine *lineaIzqAnguloX;
    QCPItemLine *lineaDerAnguloX;
    QCPItemLine *lineaIzqAnguloY;
    QCPItemLine *lineaDerAnguloY;

    //Item para el ajuste de los rangos del Grafico Desplazamientos
    QCPItemLine *lineaIzqDesplazamientoProX;
    QCPItemLine *lineaDerDesplazamientoProX;
    QCPItemLine *lineaIzqDesplazamientoProY;
    QCPItemLine *lineaDerDesplazamientoProY;

    //Item para el ajuste de los rangos del Grafico Desplazamientos
    QCPItemLine *lineaIzqDesplazamientoReCurX;
    QCPItemLine *lineaDerDesplazamientoReCurX;
    QCPItemLine *lineaIzqDesplazamientoReCurY;
    QCPItemLine *lineaDerDesplazamientoReCurY;

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
    void inicializarGraficoDesplazamientosProyeccion();
    void inicializarGraficoDesplazamientosRecorridoCurvo();
    void inicializarGraficoMuestras();
signals:

public slots:
    void graficarResultados(QVector<Angulo*> listaAngulos);
    //Funciones para mover las lineas tanto izquierda y derechas de los graficos
    void moverLineasIzquierdaAngulos(const double newValue);
    void moverLineasDerechaAngulos(const double newValue);
    void moverLineasIzquierdaDesplazamientosProyeccion(const double newValue);
    void moverLineasDerechaDesplazamientosProyeccion(const double newValue);
    void moverLineasIzquierdaDesplazamientosRecorridoCurvo(const double newValue);
    void moverLineasDerechaDesplazamientosRecorridoCurvo(const double newValue);
    void moverLineasDerechaMuestras(const double newValue);
    void moverLineasIzquierdaMuestras(const double newValue);

    //Slots para agregar datos: Graficos
    void agregarDatosGraficoAngulos(Angulo *angulo);
    void agregarDatosGraficoDesplazamientoProyeccion(Desplazamiento *desp);
    void agregarDatosGraficoDesplazamientoRecorridoCurvo(Desplazamiento *desp);
    void agregarDatosGraficoMuestras(Muestra *datos);

    //Slots para agregar datos: Tablas
    void agregarFilaTablaAngulos(Angulo *angulo);
    void agregarFilaTablasDesplazamientos(Desplazamiento *desp);
    void agregarFilaTablaMuestras(Muestra *datos);

    //Slots para Limpiar
    void vaciarGraficoAngulos();
    void vaciarGraficosDesplazamientoProyeccion();
    void vaciarGraficosDesplazamientoRecorridoCurvo();
    void vaciarGraficoMuestras();

    //Para redibujar los graficos
    void replotGraficoAngulos();
    void replotGraficoDesplazamientoProyeccion();
    void replotGraficoDesplazamientoRecorridoCurvo();
    void replotGraficoMuestras();

};

#endif // REPORTES_H
