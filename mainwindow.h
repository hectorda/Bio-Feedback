#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QtMath>
#include <ajustespuertoserial.h>
#include <ajustessensores.h>
#include <qcustomplot.h>
#include <raw.h>

namespace Ui {
class MainWindow;
}

class AjustesPuertoSerial;
class AjustesSensores;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonIniciarPrueba_clicked();
    void on_pushButtonResultados_clicked();
    void on_pushButtonPrueba1_clicked();
    void on_pushButtonGuardarImagen_clicked();
    void on_pushButtonGuardarMuestras_clicked();
    void on_pushButtonReiniciarPrueba_clicked();
    void on_pushButtonDetenerPrueba_clicked();
    void on_pushButtonConfPrueba_clicked();

    void abrirPuertoSerial();
    void cerrarPuertoSerial();
    void leerDatosSerial();
    void slotDatosTiempoReal(Raw *data);
    void RangeGraphic(int Range);
    void ZoomGraphic(QWheelEvent *event);
    void contextMenuRequest(QPoint pos);
    void relacionAspectodelGrafico();
    bool eventFilter(QObject *obj, QEvent *event);
    void on_dockWidget_topLevelChanged(bool topLevel);
    void on_tabWidgetGrafico_Resultados_currentChanged(int index);
    void regresarInicio();

    void on_pushButtonVolverInicio_clicked();

signals:
    void emitdata(Raw*);
    void emitEscribirSerial(QString);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    AjustesPuertoSerial *ajustesSerial;
    AjustesSensores *ajustesSensores;
    QElapsedTimer cronometro;
    QString datosLeidosPuertoSerial;
    QList<Raw*> listaMuestras;
    QCPCurve *lienzo;
    QLabel *status;
    int cantidadMuestras;
    double anguloComplementario1,anguloComplementario2;

    void inicializar();
    void conexiones();
    void inicializarGrafico();
    void mostrarBotones();
    void desactivarTabs();
    void activarTabs();
    void generarTablaRaw();
    void generarGraficosRaw();
    void preguntarRegresarInicio();
    void obtenerAngulos(Raw* dato);
    void mostrarMensajeBarraEstado(const QString &message);
    void limpiarGrafico(QCustomPlot *grafico);
    void generarGraficoResultados(QCustomPlot *grafico);
};

#endif // MAINWINDOW_H
