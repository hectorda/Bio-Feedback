#ifndef AJUSTESSENSORES_H
#define AJUSTESSENSORES_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class AjustesSensores;
}

class AjustesSensores : public QDialog
{
    Q_OBJECT

public:    
    explicit AjustesSensores(QWidget *parent = 0);
    ~AjustesSensores();
    QString ajustesactuales;
    double frecuenciaMuestreo;
    QString getAjustes() const;
    double obtenerFrecuenciaMuestreo();


private slots:
    void on_spinBoxDLPF_valueChanged(int arg1);
    void on_spinBoxSampleRate_valueChanged(int arg1);
    void aplicar();

private:
    Ui::AjustesSensores *ui;
    void inicializar();
    void conexiones();
    void llenarParametros();
    void mostrarFrecuenciaMuestreo(double SampleRate);
    void mostrarFiltroPasaBajo(const QString DLPF);
};

#endif // AJUSTESSENSORES_H
