#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <ajustespuertoserial.h>
#include <qcustomplot.h>
#include <dato.h>

namespace Ui {
class MainWindow;
}

class AjustesPuertoSerial;

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

    void abrirPuertoSerial();
    void closeSerialPort();
    void leerDatosSerial();
    void escribirDatosSerial();
    void slotDatosTiempoReal(Dato *data);
    void xRangeGraphic(int xRange);
    void yRangeGraphic(int yRange);
    void ZoomGraphic(QWheelEvent *event);



signals:
    void emitdata(Dato*);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    AjustesPuertoSerial *ajustesSerial;
    QElapsedTimer temporizador;
    QString datosLeidosPuertoSerial;
    QList<Dato*> listaMuestras;
    QCPCurve *lienzo;
    QLabel *status;
    int cantidadMuestras;

    void inicializar();
    void conexiones();
    void init_graph();
    void showStatusMessage(const QString &message);
};

#endif // MAINWINDOW_H
