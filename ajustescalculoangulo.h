#ifndef AJUSTESCALCULOANGULO_H
#define AJUSTESCALCULOANGULO_H

#include <QDialog>

namespace Ui {
class AjustesCalculoAngulo;
}

class AjustesCalculoAngulo : public QDialog
{
    Q_OBJECT

public:
    explicit AjustesCalculoAngulo(QWidget *parent = 0);
    ~AjustesCalculoAngulo();

private:
    Ui::AjustesCalculoAngulo *ui;
};

#endif // AJUSTESCALCULOANGULO_H
