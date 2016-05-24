#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QtMath>
#include <ajustespuertoserial.h>
#include <ajustessensores.h>
#include <ajustesgrafico.h>
#include <qcustomplot.h>
#include <raw.h>
#include <angulo.h>
#include <filtro_kalman.h>
#include <serial.h>
#include <reportes.h>
#include <desplazamiento.h>
#include <sql.h>
#include <analisisgrafico.h>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void calibrarDispositivos();

    QString obtenerOrientacionSensor();
private slots:
    //Slots de Botones
    void on_pushButtonPrueba1_clicked();
    void on_pushButtonPrueba2_clicked();
    void on_pushButtonPrueba3_clicked();
    void on_pushButtonPrueba4_clicked();
    void on_pushButtonReiniciarPrueba_clicked();
    void on_pushButtonConfPrueba_clicked();
    void on_pushButtonVolverInicio_clicked();
    void on_pushButtonGuardarImagen_clicked();
    void on_pushButtonGuardarMuestras_clicked();
    void on_pushButtonBuscarPaciente_clicked();

    void on_comboBoxOrientacion_currentTextChanged(const QString &arg1);    
    void on_lineEditRut_textChanged(const QString &arg1);
    void on_dockWidget_topLevelChanged(bool topLevel);
    void on_tabWidgetGrafico_Resultados_currentChanged(int index);

    void iniciarPrueba();
    void configurarArduino();
    void limpiarListasyOcultarBotones();
    void mostrarResultados();
    void slotGraficarTiempoReal(const double x, const double y);
    void marcarObjetivos(const double x, const double y);
    void RangeGraphic(int Range);
    void ZoomGraphic(QWheelEvent *event);
    void contextMenuRequest(QPoint pos);
    void relacionAspectodelGrafico();
    bool eventFilter(QObject *obj, QEvent *event);
    void regresarInicio();
    void obtenerRaw(const double AcX, const double AcY, const double AcZ, const double GyX, const double GyY, const double GyZ);
    //Para guardar e importar en formato.
    void exportar();
    void importar();

    void on_pushButtonVolverPrueba_clicked();

signals:
    void emitAnguloGraficoTiempoReal(const double x,const double y);
    void emitRawReporte(Raw*);
    void emitAnguloReporte(Angulo*);
    void emitDesplazamientoReporte(Desplazamiento*);
    void emitGraficarResultados(QList<Angulo*>);

private:
    Ui::MainWindow *ui;
    Serial *lectorSerial;
    AjustesPuertoSerial *ajustesSerial;
    AjustesSensores *ajustesSensores;
    AjustesGrafico *ajustesGrafico;
    QElapsedTimer cronometro;
    QString datosLeidosPuertoSerial;
    QList<Raw*> listaMuestras;
    QList<Angulo*> listaAngulos;
    QList<Desplazamiento*> listaDesplazamientos;
    Reportes *reportes;
    QList<QCPItemEllipse*> listaObjetivos;
    QLabel *status;
    int pruebaNumero;
    int divisorFPS;
    double frecuenciaMuestreo;
    QCPPlotTitle *titulo;
    QCPCurve *lienzo;
    AjustesGrafico::Ajustes elementosdelGrafico;
    QCPItemEllipse *circuloExterior,*circuloInterior;
    AnalisisGrafico *analisisGraficoAngulos,*analisisGraficoMuestras,*analisisGraficoDesplazamientos;
    SQL *db;

    void inicializar();
    void conexiones();
    void inicializarGrafico();

    void mostrarBotonesPrueba();
    void ocultarBotonesPrueba();
    void desactivarTabs();
    void activarTabs();
    void preguntarRegresarInicio();
    void obtenerAngulos(Raw* dato);
    void obtenerDesplazamiento(Angulo *angulo);
    void actualizarMensajeBarraEstado(const QString &message);
    void limpiarGrafico(QCustomPlot *grafico);

    void ocultarMostrarBotonesLabelTabGraficos(const QString &textoBoton, const QString &textoLabel);
    void ocultarMostrarBotonesLabelTabTabla(const QString &textoLabel);

    void activarSpacerEntreBotones();
    void desactivarSpacerEntreBotones();
    void generarObjetivos();
    bool PertenecePuntoAlObjetivo(const double x, const double y,QCPItemEllipse *P);
    void parpadeoCirculo(QCPItemEllipse *P);

};

#endif // MAINWINDOW_H
