#ifndef DIALOGCARGA_H
#define DIALOGCARGA_H

#include <QDialog>
#include <QMovie>
#include <QTimer>

namespace Ui {
class DialogCarga;
}

class DialogCarga : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCarga(QWidget *parent = 0);
    ~DialogCarga();

    void setTextoCarga(QString &texto);
    void iniciar(double tiempo);
    void parar();

private:
    Ui::DialogCarga *ui;
    QMovie *movie;
    QTimer *timer;
    double porcentaje;

private slots:
    void actualizarPorcentaje();

};

#endif // DIALOGCARGA_H
