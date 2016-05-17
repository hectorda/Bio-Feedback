#ifndef ANALISISGRAFICOS_H
#define ANALISISGRAFICOS_H

#include <QDialog>
#include <qxtspanslider.h>

namespace Ui {
class AnalisisGraficos;
}

class AnalisisGraficos : public QDialog
{
    Q_OBJECT

public:
    explicit AnalisisGraficos(QWidget *parent = 0);
    ~AnalisisGraficos();

private:
    Ui::AnalisisGraficos *ui;
};

#endif // ANALISISGRAFICOS_H
