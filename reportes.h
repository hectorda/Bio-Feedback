#ifndef REPORTES_H
#define REPORTES_H

#include <QObject>
#include <raw.h>
#include <angulo.h>
#include <qcustomplot.h>

class Reportes : public QObject
{
    Q_OBJECT
public:
    explicit Reportes(QObject *parent = 0);
    void graficarResultados(QCustomPlot *grafico,QList<Angulo*> listaAngulos);
    void graficarMuestras(QCustomPlot *grafico, QList<Raw*> listaMuestras);
    void graficarAngulos(QCustomPlot *grafico, QList<Angulo*> listaAngulos);
    void tablaMuestras(QTableWidget *tabla,QList<Raw*> listaMuestras);
    void tablaAngulos(QTableWidget *tabla,QList<Angulo*> listaAngulos);

private:
    void limpiarGrafico(QCustomPlot *grafico);
    void limpiarTabla(QTableWidget *tabla);

signals:

public slots:
};

#endif // REPORTES_H