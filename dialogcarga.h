#ifndef DIALOGCARGA_H
#define DIALOGCARGA_H

#include <QDialog>
#include <QMovie>

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
    void iniciarMovie();
    void pararMovie();

private:
    Ui::DialogCarga *ui;
    QMovie *movie;
};

#endif // DIALOGCARGA_H
