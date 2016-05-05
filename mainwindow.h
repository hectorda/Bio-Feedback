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
    void on_pushButtonIniciarPrueba_clicked();
    void on_pushButtonPrueba1_clicked();
    void on_pushButtonGuardarImagen_clicked();
    void on_pushButtonGuardarMuestras_clicked();
    void on_pushButtonReiniciarPrueba_clicked();
    void on_pushButtonDetenerPrueba_clicked();
    void on_pushButtonConfPrueba_clicked();
    void on_pushButtonVolverInicio_clicked();

    void iniciarPrueba();
    void leerDatosSerial();
    void slotGraficarTiempoReal(Angulo *angulo);
    void RangeGraphic(int Range);
    void ZoomGraphic(QWheelEvent *event);
    void contextMenuRequest(QPoint pos);
    void relacionAspectodelGrafico();
    bool eventFilter(QObject *obj, QEvent *event);
    void on_dockWidget_topLevelChanged(bool topLevel);
    void on_tabWidgetGrafico_Resultados_currentChanged(int index);
    void regresarInicio();

signals:
    void emitAngulo(Angulo*);
    void emitEscribirSerial(QString);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    Serial *lecturaSerial;
    AjustesPuertoSerial *ajustesSerial;
    AjustesSensores *ajustesSensores;
    AjustesGrafico *ajustesGrafico;
    QElapsedTimer cronometro;
    QString datosLeidosPuertoSerial;
    QList<Raw*> listaMuestras;
    QList<Angulo*> listaAngulos;
    Reportes *reportes;
    QList<QCPItemEllipse*> listaObjetivos;
    QCPCurve *lienzo;
    QLabel *status;
    double anguloComplementario1,anguloComplementario2;
    int rObjetivo;

    void inicializar();
    void conexiones();
    void inicializarGrafico(const int rInterior, const int rExterior);
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
    void generarObjetivos(const int rExterior, const int distanciaCentro);
};

#endif // MAINWINDOW_H
