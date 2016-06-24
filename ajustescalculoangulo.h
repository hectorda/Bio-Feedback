#ifndef AJUSTESCALCULOANGULO_H
#define AJUSTESCALCULOANGULO_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AjustesCalculoAngulo;
}

class AjustesCalculoAngulo : public QDialog
{
    Q_OBJECT

public:
    explicit AjustesCalculoAngulo(QWidget *parent = 0);
    ~AjustesCalculoAngulo();
    QString filtro;
    double alpha;
    int tiempoCalibracion;

private slots:
    void on_comboBoxFiltroAngulo_currentTextChanged(const QString &arg1);
    void aplicar();

private:
    Ui::AjustesCalculoAngulo *ui;
    void inicializar();
    void conexiones();

};

#endif // AJUSTESCALCULOANGULO_H
