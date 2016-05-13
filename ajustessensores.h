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
    struct Ajustes{
        int divisorFrecuenciaMuestreo;
        int filtroPasaBajo;
        int configuracionAcelerometro;
        int configuracionGiroscopio;
    };

    explicit AjustesSensores(QWidget *parent = 0);
    ~AjustesSensores();
    AjustesSensores::Ajustes ajustesactuales;
    double frecuenciaMuestreo;
    QString getAjustesSensores() const;
    double obtenerFrecuenciaMuestreo();


private slots:
    void on_spinBoxDLPF_valueChanged(int arg1);
    void aplicar();
    void on_comboBoxFrecuenciaMuestreo_currentIndexChanged(const QString &arg1);

private:
    Ui::AjustesSensores *ui;
    void inicializar();
    void conexiones();
    void llenarParametros();
    void mostrarFrecuenciaMuestreo(const QString SampleRate);
    void mostrarFiltroPasaBajo(const QString DLPF);
};

#endif // AJUSTESSENSORES_H
