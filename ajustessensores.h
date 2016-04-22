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

    QString getAjustes() const;

private slots:
    void on_spinBoxSampleRate_valueChanged(const QString &arg1);

private:
    Ui::AjustesSensores *ui;
    void inicializar();
    void conexiones();
    void llenarParametros();

};

#endif // AJUSTESSENSORES_H
