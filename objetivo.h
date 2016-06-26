#ifndef OBJETIVO_H
#define OBJETIVO_H

#include <qcustomplot.h>
#include <QColor>
#include <QTimer>
#include <QObject>

class Objetivo:public QObject
{
public:
    Objetivo();
    Objetivo(QCPItemEllipse *cir, double centerx, double centery, int radioO, QColor sinMarcar);

    QCPItemEllipse *getCirculo() const;
    void setCirculo(QCPItemEllipse *value);

    int getRadioObjetivo() const;
    void setRadioObjetivo(int value);

    QColor getColorSinMarcar() const;
    void setColorSinMarcar(const QColor &value);

    void marcarObjetivo(QColor color);

    bool PertenecePuntoAlObjetivo(const double x, const double y);
    void iniciarParpadeo();
    void parpadeoCirculo();
    void pararTimer();

    ~Objetivo();

private:
    QCPItemEllipse *Circulo;
    int radioObjetivo;
    QColor colorSinMarcar;
    QTimer *timerParpadeo;

};

#endif // OBJETIVO_H
