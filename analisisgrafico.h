#ifndef ANALISISGRAFICO_H
#define ANALISISGRAFICO_H

#include <QDialog>
#include <qxtspanslider.h>
#include <angulo.h>
#include <reportes.h>

namespace Ui {
class AnalisisGrafico;
}

class AnalisisGrafico : public QDialog
{
    Q_OBJECT

public:
    explicit AnalisisGrafico(QWidget *parent = 0);
    AnalisisGrafico(QWidget *parent, Reportes *reportes);
    ~AnalisisGrafico();    
    void setListaAngulos(QList<Angulo *> listaAngulos);

private:
    Ui::AnalisisGrafico *ui;
    QList<Angulo*> listaAngulos;
};

#endif // ANALISISGRAFICO_H
