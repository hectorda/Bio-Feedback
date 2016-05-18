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
#include <sql.h>
#include <analisisgraficos.h>

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

private slots:
    void on_pushButtonPrueba1_clicked();
    void on_pushButtonPrueba2_clicked();
    void on_pushButtonPrueba3_clicked();
    void on_pushButtonPrueba4_clicked();
    void on_pushButtonIniciarPrueba_clicked();
    void on_pushButtonReiniciarPrueba_clicked();
    void on_pushButtonDetenerPrueba_clicked();
    void on_pushButtonConfPrueba_clicked();
    void on_pushButtonVolverInicio_clicked();
    void on_pushButtonGuardarImagen_clicked();
    void on_pushButtonGuardarMuestras_clicked();
    void on_pushButtonAnalizarGraficos_clicked();
    void on_comboBoxOrientacion_currentTextChanged(const QString &arg1);


    void iniciarPrueba();
    void mostrarResultados();
    void slotGraficarTiempoReal(Angulo *angulo);
    void marcarObjetivos(Angulo *angulo);
    void RangeGraphic(int Range);
    void ZoomGraphic(QWheelEvent *event);
    void contextMenuRequest(QPoint pos);
    void relacionAspectodelGrafico();
    bool eventFilter(QObject *obj, QEvent *event);
    void on_dockWidget_topLevelChanged(bool topLevel);
    void on_tabWidgetGrafico_Resultados_currentChanged(int index);
    void regresarInicio();
    void obtenerRaw(const double AcX, const double AcY, const double AcZ, const double GyX, const double GyY, const double GyZ);

    void on_lineEditRut_textChanged(const QString &arg1);

signals:
    void emitAnguloGraficoTiempoReal(Angulo*);
    void emitRawReporte(Raw*);
    void emitAnguloReporte(Angulo*);
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
    Reportes *reportes;
    QList<QCPItemEllipse*> listaObjetivos;
    QLabel *status;
    int pruebaNumero;
    int divisorFPS;
    double frecuenciaMuestreo;
    QCPCurve *lienzo;
    AjustesGrafico::Ajustes elementosdelGrafico;
    QCPItemEllipse *circuloExterior,*circuloInterior;
    SQL *db;

    void inicializar();
    void conexiones();
    void configurarArduino();
    void inicializarGrafico();
    void mostrarBotones();
    void ocultarBotones();
    void desactivarTabs();
    void activarTabs();
    void preguntarRegresarInicio();
    void obtenerAngulos(Raw* dato);
    void actualizarMensajeBarraEstado(const QString &message);
    void limpiarGrafico(QCustomPlot *grafico);
    void activarSpacerEntreBotones();
    void desactivarSpacerEntreBotones();
    void generarObjetivos();
};

#endif // MAINWINDOW_H
