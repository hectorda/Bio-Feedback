#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QtMath>

//Clases Ventanas de Configuracion//
#include <ajustespuertoserial.h>
#include <ajustessensores.h>
#include <ajustesgrafico.h>
#include <analisisgrafico.h>
#include <ajustescalculoangulo.h>

//Clases para Objetos
#include <qcustomplot.h>
#include <serial.h>
#include <reportes.h>
#include <sql.h>
#include <prueba.h>
#include <objetivo.h>

//Ventana de Ayuda->acerca de ...
#include <acerca.h>
#include <dialogcarga.h>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //Slots de Botones
    void on_pushButtonReiniciarPrueba_clicked();
    void on_pushButtonConfPrueba_clicked();
    void on_pushButtonVolverInicio_clicked();
    void on_pushButtonGuardarImagen_clicked();
    void on_pushButtonGuardarMuestras_clicked();
    void on_pushButtonBuscarPaciente_clicked();
    void on_pushButtonVolverPrueba_clicked();

    void on_comboBoxOrientacion_currentTextChanged(const QString &arg1);    
    void on_lineEditRut_textChanged(const QString &arg1);
    void on_dockWidget_topLevelChanged(bool topLevel);
    void on_tabWidgetGrafico_Resultados_currentChanged(int index);

    void mostarElementosConfigurarPrueba();
    void irAConfigurarPrueba();
    void configurarArduino();
    void iniciarPrueba();
    void limpiarListasyOcultarBotones();
    void almacenarAjustesInterfazenObjetoPrueba();
    void desactivarActions();
    void activarActions();
    void mostrarResultados();
    void slotGraficarTiempoReal(const double x, const double y);
    void marcarObjetivos(const double x, const double y);
    void actualizarRangoGrafico(int Range);
    void contextMenuRequest(QPoint pos);
    void conectarActionsParaIrATabs();
    void relacionAspectodelGrafico();
    bool eventFilter(QObject *obj, QEvent *event);
    void regresarInicio();
    void obtenerRaw(const double AcX, const double AcY, const double AcZ, const double GyX, const double GyY, const double GyZ);
    void calibrar(const double AcX, const double AcY, const double AcZ, const double GyX, const double GyY, const double GyZ);
    void seleccionarDatoAGraficar(Angulo *angulo, Desplazamiento *desplazamiento);
    void on_stackedWidget_currentChanged(int arg1);

    void on_pushButtonEliminarDatosPaciente_clicked();

signals:
    void emitAnguloGraficoTiempoReal(const double x,const double y);
    void emitRawReporte(Muestra*);
    void emitAnguloReporte(Angulo*);
    void emitDesplazamientoReporte(Desplazamiento*);
    void emitGraficarResultados(QVector<Angulo*>);

private:
    Ui::MainWindow *ui;
    Serial *lectorSerial;
    AjustesPuertoSerial *ajustesSerial;
    AjustesSensores *ajustesSensores;
    AjustesGrafico *ajustesGrafico;
    AjustesCalculoAngulo *ajustesCalculoAngulo;
    QElapsedTimer cronometro;
    Reportes *reportes;
    Prueba *prueba;
    QCPPlotTitle *titulo;
    QCPCurve *lienzo;
    QCPItemEllipse *circuloExterior,*circuloInterior;
    AnalisisGrafico *analisisGraficoAngulos,*analisisGraficoMuestras,*analisisGraficoDesplazamientoProyeccion,*analisisGraficoDesplazamientoRecorridoCurvo;
    SQL *db;
    Angulo *objetoAngulo;
    bool calibrado;
    DialogCarga *dialogCarga;

    void inicializar();
    void conexiones();
    void inicializarGrafico();

    void mostrarBotonesPrueba();
    void ocultarBotonesPrueba();
    void desactivarTabs();
    void activarTabs();
    void preguntarRegresarInicio();
    void llenarInformeReporte();
    void actualizarMensajeBarraEstado(const QString &message);
    void limpiarGrafico(QCustomPlot *grafico);
    QString obtenerOrientacionSensor();

    void ocultarMostrarBotonesLabelTabGraficos(const QString &textoBoton, const QString &textoLabel);
    void ocultarMostrarBotonesLabelTabTabla(const QString &textoLabel);

    void activarSpacerEntreBotones();
    void desactivarSpacerEntreBotones();

    void generarObjetivos();
    bool PertenecePuntoAlObjetivo(const double x, const double y,QCPItemEllipse *P);
    void parpadeoCirculo(QCPItemEllipse *P);
    boolean verificarPermanenciaenObjetivo(double &tiempoInicialMarcado);

};

#endif // MAINWINDOW_H
