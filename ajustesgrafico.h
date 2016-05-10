#ifndef AJUSTESGRAFICO_H
#define AJUSTESGRAFICO_H

#include <QDialog>
#include <qcustomplot.h>

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
        QColor colorObjetivoSinMarcar;
        QColor colorObjetivoMarcado;
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

private slots:
    void aplicar();
    void on_spinBoxRObjetivo_valueChanged(int arg1);
    void on_spinBoxRExterior_valueChanged(int arg1);
    void on_spinBoxRInterior_valueChanged(int arg1);
    void on_comboBoxColorObjetivosSinMarcar_currentIndexChanged(int index);
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // AJUSTESGRAFICO_H
