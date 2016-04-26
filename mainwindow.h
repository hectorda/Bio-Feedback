#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <ajustespuertoserial.h>
#include <ajustessensores.h>
#include <qcustomplot.h>
#include <dato.h>

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
    void on_pushButtonRegresarInicio_clicked();
    void on_pushButtonReiniciarPrueba_clicked();
    void on_pushButtonDetenerPrueba_clicked();
    void on_pushButtonConfPrueba_clicked();

    void abrirPuertoSerial();
    void cerrarPuertoSerial();
    void leerDatosSerial();
    void slotDatosTiempoReal(Dato *data);
    void RangeGraphic(int Range);
    void ZoomGraphic(QWheelEvent *event);
    void contextMenuRequest(QPoint pos);
    void relacionAspectodelGrafico();
    bool eventFilter(QObject *obj, QEvent *event);
    void on_dockWidget_topLevelChanged(bool topLevel);
    void on_tabWidgetGrafico_Resultados_currentChanged(int index);


signals:
    void emitdata(Dato*);
    void emitEscribirSerial(QString);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    AjustesPuertoSerial *ajustesSerial;
    AjustesSensores *ajustesSensores;
    QElapsedTimer temporizador;
    QString datosLeidosPuertoSerial;
    QList<Dato*> listaMuestras;
    QCPCurve *lienzo;
    QLabel *status;
    int cantidadMuestras;

    void inicializar();
    void conexiones();
    void inicializarGrafico();
    void mostrarBotones();
    void mostrarMensajeBarraEstado(const QString &message);
    void limpiarGrafico(QCustomPlot *grafico);
    void generarGraficoResultados(QCustomPlot *grafico);
};

#endif // MAINWINDOW_H
