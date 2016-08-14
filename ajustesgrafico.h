#ifndef AJUSTESGRAFICO_H
#define AJUSTESGRAFICO_H

#include <QDialog>
#include <qcustomplot.h>
#include <QMessageBox>

namespace Ui {
class AjustesGrafico;
}

class AjustesGrafico : public QDialog
{
    Q_OBJECT

public:
    struct Ajustes{
        int RadioInterior;
        int RadioExterior;
        int RadioObjetivo;
        int FPS;
        QString Unidad;
        QString CalculoDesplazamiento;
        QColor colorObjetivoSinMarcar;
        QColor colorObjetivoMarcado;
        int tiempoParaMarcar;
    };

    explicit AjustesGrafico(QWidget *parent = 0);
    ~AjustesGrafico();

    AjustesGrafico::Ajustes getAjustes();
    void llenarParametrosComboBox();

private:
    Ui::AjustesGrafico *ui;
    AjustesGrafico::Ajustes ajustesActuales;
    QCPItemEllipse *objetivo;
    QCPItemEllipse *circuloExterior;
    QCPItemEllipse *circuloInterior;
    void inicializar();
    void conexiones();
    void graficarObjetivo(const int rObjetivo, const QColor color);
    void relacionAspecto(QCustomPlot *grafico);
    void graficarRepresentacionGrafico(const int rInterior, const int rExterior);
    bool event(QEvent *event);
    bool validar();

private slots:
    void aplicar();
    void on_spinBoxRObjetivo_valueChanged(int arg1);
    void on_spinBoxRExterior_valueChanged(int arg1);
    void on_spinBoxRInterior_valueChanged(int arg1);
    void on_comboBoxColorObjetivosSinMarcar_currentIndexChanged(int index);
    bool eventFilter(QObject *obj, QEvent *event);
    void on_comboBoxColorObjetivosMarcados_currentIndexChanged(int index);
    void on_comboBoxUnidadGrafico_currentIndexChanged(const QString &arg1);
};

#endif // AJUSTESGRAFICO_H
